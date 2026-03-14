#include "uart_echo_wifi_ble/uart_echo_wifi_ble.h"
#include "events_temp_get.h"
#include "esp_log.h"
#include "mqtt_client.h"
#include "esp_crt_bundle.h"

char esp32_id[13]; // 定义全局变量
const char *model = "ESP32_S3_H_V1"; // 定义全局变量

/*串口配置 */
#define ECHO_TEST_TXD 17
#define ECHO_TEST_RXD 18
#define ECHO_TEST_RTS -1
#define ECHO_TEST_CTS -1

#define ECHO_UART_PORT_NUM      1
#define ECHO_UART_BAUD_RATE     4800
#define ECHO_TASK_STACK_SIZE    3072

#define BUF_SIZE (1024)


// 日志标签定义
static const char *TAG = "UART TEST";

// 将 UART1 收发的数据镜像到 UART0 控制台
static void log_uart1_bytes(const char *prefix, const uint8_t *data, int len)
{
    if (data == NULL || len <= 0) {
        return;
    }
    ESP_LOGI(TAG, "%s (len=%d)", prefix, len);
    ESP_LOG_BUFFER_HEXDUMP(TAG, data, len, ESP_LOG_INFO);
}

/*滑动窗口滤波*/
// 滑动窗口大小定义
#define SLIDING_WINDOW_SIZE 5
// 定义传感器数据滑动窗口结构体
typedef struct {
    sensor_data_t data_window[SLIDING_WINDOW_SIZE];
    int count;
    int index;
} sensor_sliding_window_t;

// 真正的极端值阈值定义（明显超出物理可能的值）
#define NITROGEN_EXTREME_MAX 1000.0    // mg/kg，远超正常范围
#define PHOSPHORUS_EXTREME_MAX 500.0   // mg/kg，远超正常范围
#define POTASSIUM_EXTREME_MAX 2000.0   // mg/kg，远超正常范围
#define MOISTURE_EXTREME_MIN -10.0     // 百分比，不应为负
#define MOISTURE_EXTREME_MAX 110.0     // 百分比，不应超过100
#define PH_EXTREME_MIN 0.0             // pH值不应小于0
#define PH_EXTREME_MAX 14.0            // pH值不应大于14
#define TEMPERATURE_EXTREME_MIN -50.0  // °C，极低温度
#define TEMPERATURE_EXTREME_MAX 100.0  // °C，极高温度
#define SALINITY_EXTREME_MAX 50.0      // dS/m，过高盐分
// 全局滑动窗口实例
static sensor_sliding_window_t sliding_windows = {0};

// 比较函数用于排序
static int compare_float(const void *a, const void *b) {
    float fa = *(const float*)a;
    float fb = *(const float*)b;
    return (fa > fb) - (fa < fb);
}
// 检查数据是否为极端值
static bool is_extreme_value(sensor_data_t *data) {
    // 检查明显的错误值（负值或超出物理极限）
    if (data->nitrogen < 0.0 || data->nitrogen > NITROGEN_EXTREME_MAX ||
        data->phosphorus < 0.0 || data->phosphorus > PHOSPHORUS_EXTREME_MAX ||
        data->potassium < 0.0 || data->potassium > POTASSIUM_EXTREME_MAX ||
        data->moisture < MOISTURE_EXTREME_MIN || data->moisture > MOISTURE_EXTREME_MAX ||
        data->ph < PH_EXTREME_MIN || data->ph > PH_EXTREME_MAX ||
        data->temperature < TEMPERATURE_EXTREME_MIN || data->temperature > TEMPERATURE_EXTREME_MAX ||
        data->salinity < 0.0 || data->salinity > SALINITY_EXTREME_MAX) {
        
        ESP_LOGW(TAG, "Extreme value detected:");
        ESP_LOGW(TAG, "N: %.1f, P: %.1f, K: %.1f, Moisture: %.1f, pH: %.1f, Temp: %.1f, Salinity: %.1f",
                 data->nitrogen, data->phosphorus, data->potassium, 
                 data->moisture, data->ph, data->temperature, data->salinity);
        return true;
    }
    return false;
}
// 将新数据添加到滑动窗口
static void add_to_sliding_window(sensor_data_t *new_data) {
    // 如果是极端值，则不添加到窗口中
    if (is_extreme_value(new_data)) {
        ESP_LOGW(TAG, "Extreme value detected, skipping filter update");
        ESP_LOGW(TAG, "N: %.1f, P: %.1f, K: %.1f, Moisture: %.1f, pH: %.1f, Temp: %.1f, Salinity: %.1f",
                 new_data->nitrogen, new_data->phosphorus, new_data->potassium, 
                 new_data->moisture, new_data->ph, new_data->temperature, new_data->salinity);
        return;
    }

    // 添加数据到窗口
    sliding_windows.data_window[sliding_windows.index] = *new_data;
    
    if (sliding_windows.count < SLIDING_WINDOW_SIZE) {
        sliding_windows.count++;
    }
    
    sliding_windows.index = (sliding_windows.index + 1) % SLIDING_WINDOW_SIZE;
}
// 计算中位数
static float calculate_median(float *values, int count) {
    if (count == 0) return 0.0;
    
    // 创建副本进行排序
    float *sorted_values = malloc(count * sizeof(float));
    if (!sorted_values) return values[0]; // 内存分配失败，返回第一个值
    
    memcpy(sorted_values, values, count * sizeof(float));
    qsort(sorted_values, count, sizeof(float), compare_float);
    
    float median;
    if (count % 2 == 0) {
        // 偶数个元素，取中间两个的平均值
        median = (sorted_values[count/2 - 1] + sorted_values[count/2]) / 2.0;
    } else {
        // 奇数个元素，取中间元素
        median = sorted_values[count/2];
    }
    
    free(sorted_values);
    return median;
}
// 应用滑动窗口中位数滤波
static sensor_data_t apply_median_filter(sensor_data_t *new_data) {
    // 将新数据添加到滑动窗口
    add_to_sliding_window(new_data);
    
    // 如果窗口为空，直接返回原数据
    if (sliding_windows.count == 0) {
        return *new_data;
    }
    
    // 提取各参数的历史数据
    float moisture_values[SLIDING_WINDOW_SIZE];
    float temperature_values[SLIDING_WINDOW_SIZE];
    float conductivity_values[SLIDING_WINDOW_SIZE];
    float ph_values[SLIDING_WINDOW_SIZE];
    float nitrogen_values[SLIDING_WINDOW_SIZE];
    float phosphorus_values[SLIDING_WINDOW_SIZE];
    float potassium_values[SLIDING_WINDOW_SIZE];
    float salinity_values[SLIDING_WINDOW_SIZE];  // 添加盐分数组
    
    int valid_count = sliding_windows.count;
    for (int i = 0; i < valid_count; i++) {
        moisture_values[i] = sliding_windows.data_window[i].moisture;
        temperature_values[i] = sliding_windows.data_window[i].temperature;
        conductivity_values[i] = sliding_windows.data_window[i].conductivity;
        ph_values[i] = sliding_windows.data_window[i].ph;
        nitrogen_values[i] = sliding_windows.data_window[i].nitrogen;
        phosphorus_values[i] = sliding_windows.data_window[i].phosphorus;
        potassium_values[i] = sliding_windows.data_window[i].potassium;
        salinity_values[i] = sliding_windows.data_window[i].salinity;  // 添加盐分值
    }
    
    // 计算各参数的中位数
    sensor_data_t filtered_data = {0};
    filtered_data.moisture = calculate_median(moisture_values, valid_count);
    filtered_data.temperature = calculate_median(temperature_values, valid_count);
    filtered_data.conductivity = calculate_median(conductivity_values, valid_count);
    filtered_data.ph = calculate_median(ph_values, valid_count);
    filtered_data.nitrogen = calculate_median(nitrogen_values, valid_count);
    filtered_data.phosphorus = calculate_median(phosphorus_values, valid_count);
    filtered_data.potassium = calculate_median(potassium_values, valid_count);
    filtered_data.salinity = calculate_median(salinity_values, valid_count);  // 添加盐分中位数计算
    
    ESP_LOGI(TAG, "Filtered data - N: %.1f, P: %.1f, K: %.1f, Salinity: %.1f", 
             filtered_data.nitrogen, filtered_data.phosphorus, filtered_data.potassium, filtered_data.salinity);
    
    return filtered_data;
}

//wifi_int**********************************
#define WIFI_SSID "Redmi K60 Ultra"
#define WIFI_PASSWORD "88888888"

#define MQTT_BROKER_URI "mqtts://e31aa9b0.ala.cn-hangzhou.emqxsl.cn:8883"
#define MQTT_USERNAME "admin"
#define MQTT_PASSWORD "123456"
#define MQTT_TOPIC_BASE "soil"
//***************************************
float convert_moisture(uint16_t raw_value) { return (float)raw_value / 10.0; }
float convert_conductivity(uint16_t raw_value) { return (float)raw_value; }
float convert_ph(uint16_t raw_value) { return (float)raw_value / 10.0; }
float convert_salinity(uint16_t raw_value) { return (float)raw_value / 10.0; }  // 添加盐分转换函数
float convert_temperature(uint16_t raw_value) {
    if (raw_value & 0x8000) {
        uint16_t temp = ~raw_value + 1;
        return -((float)temp / 10.0);
    } else {
        return (float)raw_value / 10.0;
    }
}
//wifi_begin----------------------------------------------
static EventGroupHandle_t wifi_event_group;
const int WIFI_CONNECTED_BIT = BIT0;

static void event_handler(void* arg, esp_event_base_t event_base,
                         int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        ESP_LOGI(TAG, "WiFi disconnected, trying to reconnect...");
        esp_wifi_connect();
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "Got IP:" IPSTR, IP2STR(&event->ip_info.ip));
        xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
    }
}
void wifi_init_sta(void)
{
    wifi_event_group = xEventGroupCreate();
    
    esp_netif_init();
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // 注册事件处理
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASSWORD,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
            // .sae_pwe_h2e = WPA3_SAE_PWE_BOTH,
        },
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    // ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "wifi初始化结束.");
}
//wifi是否连接判断函数
static bool is_network_ready(void)
{
    esp_netif_ip_info_t ip_info;
    esp_netif_t* netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
    if (netif && esp_netif_get_ip_info(netif, &ip_info) == ESP_OK) {
        if (ip_info.ip.addr != 0) {
            ESP_LOGI(TAG, "网络已就绪，IP: " IPSTR, IP2STR(&ip_info.ip));
            return true;
        }
    }
    ESP_LOGW(TAG, "网络未就绪");
    return false;
}
//wifi事件创建
//http-json格式-----------------------------------------
static char* sensor_data_to_json(sensor_data_t *sensor_data)
{
    cJSON *root = cJSON_CreateObject();
    if (root == NULL) {
        ESP_LOGE(TAG, "Failed to create JSON object");
        return NULL;
    }
    cJSON_AddStringToObject(root, "model", model);
    cJSON_AddStringToObject(root, "esp32_id", esp32_id);
    cJSON_AddNumberToObject(root, "moisture", sensor_data->moisture);
    cJSON_AddNumberToObject(root, "temperature", sensor_data->temperature);
    cJSON_AddNumberToObject(root, "conductivity", sensor_data->conductivity);
    cJSON_AddNumberToObject(root, "ph", sensor_data->ph);
    cJSON_AddNumberToObject(root, "nitrogen", sensor_data->nitrogen);
    cJSON_AddNumberToObject(root, "phosphorus", sensor_data->phosphorus);
    cJSON_AddNumberToObject(root, "potassium", sensor_data->potassium);
    cJSON_AddNumberToObject(root, "salinity", sensor_data->salinity);

    char *json_string = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    
    return json_string;
}
static esp_mqtt_client_handle_t mqtt_client = NULL;
static bool mqtt_connected = false;

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;

    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            mqtt_connected = true;
            ESP_LOGI(TAG, "MQTT connected");
            break;
        case MQTT_EVENT_DISCONNECTED:
            mqtt_connected = false;
            ESP_LOGW(TAG, "MQTT disconnected");
            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGE(TAG, "MQTT error");
            break;
        default:
            break;
    }
}

static void mqtt_start(void)
{
    if (mqtt_client != NULL) {
        return;
    }

    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = MQTT_BROKER_URI,
        .broker.verification.crt_bundle_attach = esp_crt_bundle_attach,
        .credentials.username = MQTT_USERNAME,
        .credentials.authentication.password = MQTT_PASSWORD,
        .session.keepalive = 60,
    };

    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
    if (mqtt_client == NULL) {
        ESP_LOGE(TAG, "Failed to init MQTT client");
        return;
    }

    esp_mqtt_client_register_event(mqtt_client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(mqtt_client);
}

static esp_err_t mqtt_publish_sensor_data(sensor_data_t *sensor_data)
{
    if (mqtt_client == NULL || !mqtt_connected) {
        return ESP_ERR_INVALID_STATE;
    }

    char *json_data = sensor_data_to_json(sensor_data);
    if (json_data == NULL) {
        ESP_LOGE(TAG, "Failed to create JSON data");
        return ESP_FAIL;
    }

    char topic[96];
    snprintf(topic, sizeof(topic), "%s/%s/data", MQTT_TOPIC_BASE, esp32_id);

    int msg_id = esp_mqtt_client_publish(mqtt_client, topic, json_data, 0, 1, 0);
    free(json_data);

    if (msg_id < 0) {
        ESP_LOGE(TAG, "MQTT publish failed");
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "MQTT publish ok, msg_id=%d", msg_id);
    return ESP_OK;
}
//wifi_end----------------------------------------------
static sensor_data_t latest_sensor_data = {0};
static bool new_data_available = false;
static SemaphoreHandle_t data_mutex = NULL;
//串口初始化
void echo_task(void *arg)
{
    /* Configure parameters of an UART driver,
     * communication pins and install the driver */
    uart_config_t uart_config = {
        .baud_rate = ECHO_UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    int intr_alloc_flags = 0;

#if CONFIG_UART_ISR_IN_IRAM
    intr_alloc_flags = ESP_INTR_FLAG_IRAM;
#endif

    ESP_ERROR_CHECK(uart_driver_install(ECHO_UART_PORT_NUM, BUF_SIZE * 2, 0, 0, NULL, intr_alloc_flags));
    ESP_ERROR_CHECK(uart_param_config(ECHO_UART_PORT_NUM, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(ECHO_UART_PORT_NUM, ECHO_TEST_TXD, ECHO_TEST_RXD, ECHO_TEST_RTS, ECHO_TEST_CTS));

    // Configure a temporary buffer for the incoming data
    uint8_t *data = (uint8_t *) malloc(BUF_SIZE);
    uint8_t request_data[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x08, 0x44, 0x0C}; 
    while (1) {
        // Read data from the UART
       uart_write_bytes(ECHO_UART_PORT_NUM, (const char *) request_data, sizeof(request_data));
        log_uart1_bytes("UART1 TX", request_data, sizeof(request_data));
        // 增加等待时间让自动转换完成
        vTaskDelay(40 / portTICK_PERIOD_MS);  
        int len = uart_read_bytes(ECHO_UART_PORT_NUM, data, (BUF_SIZE - 1), 20 / portTICK_PERIOD_MS);
        if (len > 0) {
            log_uart1_bytes("UART1 RX", data, len);
        }
        // Write data back to the UART
        vTaskDelay(40 / portTICK_PERIOD_MS);  
        if (len>=19&&data[0]==0x01&&data[1]==0x03&&data[2]==0x10) {  // 修改长度检查以适应新增的盐分数据
            data[len] = '\0';
            uint16_t moisture_raw = (data[3] << 8) | data[4];     // 含水率原始值
            uint16_t temperature_raw = (data[5] << 8) | data[6];  // 温度值原始值
            uint16_t conductivity_raw = (data[7] << 8) | data[8]; // 电导率原始值
            uint16_t ph_raw = (data[9] << 8) | data[10];          // PH值原始值
            uint16_t nitrogen_raw = (data[11] << 8) | data[12];   // 氮含量原始值
            uint16_t phosphorus_raw = (data[13] << 8) | data[14]; // 磷含量原始值
            uint16_t potassium_raw = (data[15] << 8) | data[16];  // 钾含量原始值
            uint16_t salinity_raw = (data[17] << 8) | data[18];   // 盐分原始值
            
            sensor_data_t sensor_data = {
                .ph = convert_ph(ph_raw),
                .moisture = convert_moisture(moisture_raw),
                .temperature = convert_temperature(temperature_raw),
                .conductivity = convert_conductivity(conductivity_raw),
                .nitrogen = nitrogen_raw,
                .phosphorus = phosphorus_raw,
                .potassium = potassium_raw,
                .salinity = convert_salinity(salinity_raw)  // 添加盐分转换
            };
            
            sensor_data_t filtered_sensor_data = apply_median_filter(&sensor_data);
            
            //发送数据结束
            printf("=== 传感器数据 ===\n");
            printf("含水率: %.1f%%\n", filtered_sensor_data.moisture);
            printf("温度值: %.1f°C\n", filtered_sensor_data.temperature);
            printf("电导率: %.0f us/cm\n", filtered_sensor_data.conductivity);
            printf("PH值: %.1f\n", filtered_sensor_data.ph);
            printf("氮含量: %.1f mg/kg\n", filtered_sensor_data.nitrogen);
            printf("磷含量: %.1f mg/kg\n", filtered_sensor_data.phosphorus);
            printf("钾含量: %.1f mg/kg\n", filtered_sensor_data.potassium);
             printf("盐分: %.1f dS/m\n", filtered_sensor_data.salinity);  // 添加盐分显示
            printf("==================\n");
            // 在 echo_task 函数中，当传感器数据更新时调用 trigger_sensor_callbacks
            // 找到数据处理完成的部分，添加回调触发：
            if (data_mutex != NULL) {
                if (xSemaphoreTake(data_mutex, portMAX_DELAY) == pdTRUE) {
                    latest_sensor_data = filtered_sensor_data;
                    new_data_available = true;
                    ESP_LOGI("SENSOR", "Sensor data updated and marked as available\n");
                    xSemaphoreGive(data_mutex);
                }
            }
        //尝试发送数据(阻塞)
         vTaskDelay(1000 / portTICK_PERIOD_MS);  //每秒判断采集一次
        }
    }
}

// 网络发送任务----------------------------------
void network_task(void *arg)
{
    data_mutex = xSemaphoreCreateMutex();
    static uint32_t last_send_time = 0;
    const uint32_t send_interval = 10000; // 10秒
    
    while(1) {
        if (is_network_ready()) {
            mqtt_start();
        }

        if(is_network_ready() && new_data_available) {
            uint32_t current_time = xTaskGetTickCount() * portTICK_PERIOD_MS;
            // 检查是否到了发送时间
            if(current_time - last_send_time >= send_interval || last_send_time == 0) {
                // 获取数据并发送
                if(xSemaphoreTake(data_mutex, portMAX_DELAY) == pdTRUE) {
                    esp_err_t ret = mqtt_publish_sensor_data(&latest_sensor_data);
                    if (ret == ESP_OK) {
                        ESP_LOGI(TAG, "MQTT数据发送成功");
                        last_send_time = current_time; // 更新发送时间
                    } else {
                        ESP_LOGE(TAG, "MQTT数据发送失败: %s", esp_err_to_name(ret));
                    }
                    new_data_available = false;
                    xSemaphoreGive(data_mutex);
                }
            }
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

// 提供给外部访问传感器数据的函数
bool get_sensor_data(sensor_data_t *data, bool *has_new_data)
{
    // 检查输入参数
    if (data == NULL || has_new_data == NULL) {
        return false;
    }
    
    // 检查互斥锁是否存在
    if (data_mutex == NULL) {
        return false;
    }
    
    // 获取数据
    if (xSemaphoreTake(data_mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
        *data = latest_sensor_data;
        *has_new_data = new_data_available;
        
        // 重置新数据标志
        new_data_available = false;
        
        xSemaphoreGive(data_mutex);
        return true;
    }
    
    return false;
}
void get_esp32_id(char *esp32_id, size_t len)
{
    uint8_t mac[6];
    esp_read_mac(mac, ESP_MAC_WIFI_STA);
    snprintf(esp32_id, len, "%02X%02X%02X%02X%02X%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}