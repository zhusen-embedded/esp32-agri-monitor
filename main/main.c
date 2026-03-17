#include <stdio.h>
#include "lvgl.h"
#include "esp_lv_adapter.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_ili9341.h"
#include "esp_lcd_touch.h"
#include "esp_lcd_touch_ft6x36.h"
#include "driver/spi_master.h"
#include "driver/i2c.h"
#include "driver/gpio.h"
#include "ui/ui.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "uart_echo_wifi_ble/uart_echo_wifi_ble.h"
#include "events_temp_get.h"
#include "events_manager.h"
#include "ble_sitting_wifi/ble_sitting_wifi.h"
#include "esp_psram.h" // 或者 esp_heap_caps.h

static volatile uint32_t s_lvgl_task_delay_ms = 10;

// 继电器控制引脚映射
#define RELAY_PUMP_GPIO      GPIO_NUM_15
#define RELAY_LIGHT_GPIO     GPIO_NUM_16
#define RELAY_ACTIVE_LEVEL   0
#define RELAY_INACTIVE_LEVEL 1

static esp_err_t relay_init(void)
{
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << RELAY_PUMP_GPIO) | (1ULL << RELAY_LIGHT_GPIO),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    ESP_ERROR_CHECK(gpio_config(&io_conf));

    // 默认关闭，避免上电误动作。
    gpio_set_level(RELAY_PUMP_GPIO, RELAY_INACTIVE_LEVEL);
    gpio_set_level(RELAY_LIGHT_GPIO, RELAY_INACTIVE_LEVEL);

    printf("Relay initialized: pump=GPIO15, light=GPIO16, active_level=%d\n", RELAY_ACTIVE_LEVEL);
    return ESP_OK;
}

void relay_set_pump(bool on)
{
    gpio_set_level(RELAY_PUMP_GPIO, on ? RELAY_ACTIVE_LEVEL : RELAY_INACTIVE_LEVEL);
}

void relay_set_light(bool on)
{
    gpio_set_level(RELAY_LIGHT_GPIO, on ? RELAY_ACTIVE_LEVEL : RELAY_INACTIVE_LEVEL);
}

void set_lvgl_task_delay_ms(uint32_t delay_ms)
{
    if (delay_ms < 5) {
        delay_ms = 5;
    }
    if (delay_ms > 200) {
        delay_ms = 200;
    }
    s_lvgl_task_delay_ms = delay_ms;
    printf("LVGL task delay set to %lu ms\n", (unsigned long)s_lvgl_task_delay_ms);
}
// 显示屏引脚定义
#define LCD_HOST SPI2_HOST
#define LCD_PIN_SCLK     11
#define LCD_PIN_MOSI     12
#define LCD_PIN_CS       10
#define LCD_PIN_DC       13
#define LCD_PIN_RST      9
#define LCD_PIN_BL       5

#define I2C_NUM             I2C_NUM_0
#define I2C_SCL_PIN         4
#define I2C_SDA_PIN         5
#define TOUCH_RST_PIN      6
#define I2C_FREQ_HZ         200000
#define I2C_TOUCH_ADDR      0x38

// 屏幕参数
#define LCD_H_RES   320
#define LCD_V_RES   240

// 任务优先级和堆栈大小
#define LVGL_TASK_STACK_SIZE    (8192)
#define LVGL_TASK_PRIORITY      (configMAX_PRIORITIES - 1)  // 高优先级
#define SENSOR_TASK_PRIORITY    (tskIDLE_PRIORITY + 5)
#define NETWORK_TASK_PRIORITY   (tskIDLE_PRIORITY + 4)

// 全局句柄
static esp_lcd_panel_handle_t panel_handle = NULL;
static esp_lcd_panel_io_handle_t io_handle = NULL;
static esp_lcd_touch_handle_t tp_handle = NULL;
static SemaphoreHandle_t xGuiSemaphore = NULL;
static esp_lcd_panel_io_handle_t tp_io_handle = NULL;

// LVGL任务函数
static void __attribute__((unused)) lvgl_task(void *pvParameter)
{
    while (1) {
        // 获取互斥锁
        if (xSemaphoreTake(xGuiSemaphore, (TickType_t)10) == pdTRUE) {
            // 处理LVGL任务
            lv_task_handler();
            // 释放互斥锁
            xSemaphoreGive(xGuiSemaphore);
        }
        // 短暂延迟以允许其他任务运行
        vTaskDelay(pdMS_TO_TICKS(s_lvgl_task_delay_ms));
    }
}

// 初始化触摸屏
static esp_err_t init_touch(void)
{
    printf("Initializing touch screen...\n");
    
    // 配置I2C总线参数
    i2c_config_t i2c_conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_SDA_PIN,
        .scl_io_num = I2C_SCL_PIN,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_FREQ_HZ,
    };
    // i2c_set_timeout(I2C_NUM, 0xFFFFF); // 移除可能导致错误的超时设置
    printf("Configuring I2C bus on SDA=%d, SCL=%d\n", I2C_SDA_PIN, I2C_SCL_PIN);
    ESP_ERROR_CHECK(i2c_param_config(I2C_NUM, &i2c_conf));
    
    esp_err_t ret = i2c_driver_install(I2C_NUM, I2C_MODE_MASTER, 0, 0, 0);
    if (ret != ESP_OK) {
        printf("Failed to install I2C driver: %d\n", ret);
        return ret;
    }

    // 扫描I2C总线上的设备
    printf("Scanning I2C bus for devices...\n");
    bool device_found = false;
    uint8_t found_addr = 0;
    
    // 存储实际使用的触摸屏地址
    uint8_t touch_addr = (device_found) ? found_addr : (uint8_t)(I2C_TOUCH_ADDR & 0x7F);
    
    // 使用FT6X36专用的IO配置宏
    esp_lcd_panel_io_i2c_config_t io_config = ESP_LCD_TOUCH_IO_I2C_FT6x36_CONFIG();
    
    // 设置设备地址
    io_config.dev_addr = (uint32_t)touch_addr;
    
    // 创建IO句柄
    ret = esp_lcd_new_panel_io_i2c((esp_lcd_i2c_bus_handle_t)I2C_NUM, &io_config, &tp_io_handle);
    if (ret != ESP_OK) {
        printf("Failed to create panel IO for touch: %d\n", ret);
        return ret;
    }

    // 配置触摸屏参数
    esp_lcd_touch_config_t tp_cfg = {
        .x_max = LCD_V_RES,
        .y_max = LCD_H_RES,
        .rst_gpio_num = -1,  // 不使用复位引脚
        .int_gpio_num = -1,  // 不使用中断引脚
        .levels = {
            .reset = 0,
            .interrupt = 0,
        },
        .flags = {
            .swap_xy = 1,
            .mirror_x = 1,
            .mirror_y = 0,
        },
    };

    // 创建FT6X36触摸屏驱动句柄
    printf("Creating FT6X36 touch driver with address 0x%02X...\n", touch_addr);
    ret = esp_lcd_touch_new_i2c_ft6x36(tp_io_handle, &tp_cfg, &tp_handle);
    if (ret != ESP_OK) {
        printf("Failed to initialize FT6X36 touch controller: %d\n", ret);
        return ret;
    }
    
    printf("Touch screen initialized successfully\n");
    return ESP_OK;
}

// 初始化显示驱动
static esp_err_t init_display(void)
{
    // 配置 SPI 总线
    spi_bus_config_t buscfg = {
        .sclk_io_num = LCD_PIN_SCLK,
        .mosi_io_num = LCD_PIN_MOSI,
        .miso_io_num = -1,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = LCD_H_RES * LCD_V_RES * sizeof(uint16_t),
    };
    ESP_ERROR_CHECK(spi_bus_initialize(LCD_HOST, &buscfg, SPI_DMA_CH_AUTO));

    // 配置 LCD IO
    esp_lcd_panel_io_spi_config_t io_config = {
        .cs_gpio_num = LCD_PIN_CS,
        .dc_gpio_num = LCD_PIN_DC,
        .spi_mode = 0,
        .pclk_hz = 40 * 1000 * 1000,
        .trans_queue_depth = 10,
        .on_color_trans_done = NULL,
        .user_ctx = NULL,
        .lcd_cmd_bits = 8,
        .lcd_param_bits = 8,
        .flags = {
            .dc_low_on_data = 0,
            .octal_mode = 0,
            .lsb_first = 0,
            .cs_high_active = 0,
        }
    };

    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_HOST, &io_config, &io_handle));

    // 配置 ILI9341 面板
    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = LCD_PIN_RST,
        .rgb_endian = LCD_RGB_ENDIAN_BGR,
        .bits_per_pixel = 16,
        .flags = {
            .reset_active_high = 0,
        }
    };

    ESP_ERROR_CHECK(esp_lcd_new_panel_ili9341(io_handle, &panel_config, &panel_handle));
    
    // 添加延时确保硬件稳定
    vTaskDelay(pdMS_TO_TICKS(50));
    
    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_invert_color(panel_handle, true));
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));
    
    // 初始化背光
    gpio_set_direction(LCD_PIN_BL, GPIO_MODE_OUTPUT);
    gpio_set_level(LCD_PIN_BL, 1);
    ESP_ERROR_CHECK(esp_lcd_panel_swap_xy(panel_handle, 1));
    return ESP_OK;
}

// 初始化 LVGL 适配器
static esp_err_t init_lvgl_adapter(void)
{
    // 初始化适配器
    esp_lv_adapter_config_t adapter_cfg = ESP_LV_ADAPTER_DEFAULT_CONFIG();
    ESP_ERROR_CHECK(esp_lv_adapter_init(&adapter_cfg));

    // 注册显示设备
    esp_lv_adapter_display_config_t display_cfg = 
        ESP_LV_ADAPTER_DISPLAY_SPI_WITHOUT_PSRAM_DEFAULT_CONFIG(
            panel_handle, 
            io_handle, 
            LCD_V_RES, 
            LCD_H_RES, 
            ESP_LV_ADAPTER_ROTATE_90
        );

    lv_display_t *display = esp_lv_adapter_register_display(&display_cfg);
    if (display == NULL) {
        printf("Failed to register display\n");
        return ESP_FAIL;
    }

    // 如果触摸屏已初始化，则注册触摸设备
    if (tp_handle != NULL) {
        esp_lv_adapter_touch_config_t touch_cfg = {
            .disp = display,
            .handle = tp_handle,
            .scale = {
                .x = 1.0f,
                .y = 1.0f,
            },
        };
        
        lv_indev_t *indev = esp_lv_adapter_register_touch(&touch_cfg);
        if (indev == NULL) {
            printf("Failed to register touch input device\n");
            return ESP_FAIL;
        }
    }

    return ESP_OK;
}

// 系统初始化函数
static esp_err_t system_init(void)
{
    // 初始化NV存储
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    return ESP_OK;
}

void app_main(void)
{
    printf("Hello world! Initializing system...\n");
    
    // 打印内存信息
    printf("Total free heap: %lu bytes\n", (unsigned long)esp_get_free_heap_size());
    printf("Total free PSRAM: %lu bytes\n", (unsigned long)heap_caps_get_free_size(MALLOC_CAP_SPIRAM));
    
    xGuiSemaphore = xSemaphoreCreateMutex();
    // 系统初始化
    esp_err_t err = system_init();
    if (err != ESP_OK) {
        printf("Failed to initialize system components: %d\n", err);
        return;
    }

    err = relay_init();
    if (err != ESP_OK) {
        printf("Failed to initialize relay: %d\n", err);
        return;
    }

    get_esp32_id(esp32_id, sizeof(esp32_id));
    esp_log_level_set("lcd_panel.io.i2c", ESP_LOG_NONE); // 只显示警告及以上
    esp_log_level_set("FT6x36", ESP_LOG_NONE);           // 只显示警告及以上
    // 初始化显示
    err = init_display();
    if (err != ESP_OK) {
        printf("Failed to initialize display: %d\n", err);
        return;
    }
    printf("Display initialized\n");
    
    printf("BLE provisioning started\n");
    ble_wifi_provisioning_start(); // 启动蓝牙自定义配网
    ble_wifi_force_reprovision(); // 强制重新配网
    // 初始化触摸屏
    err = init_touch();
    if (err != ESP_OK) {
        printf("Failed to initialize touch: %d\n", err);
        // 不返回，因为即使触摸失败也应尝试显示
    } else {
        printf("Touch initialized\n");
    }
    
    // 初始化 LVGL 适配器
    err = init_lvgl_adapter();
    if (err != ESP_OK) {
        printf("Failed to initialize LVGL adapter: %d\n", err);
        return;
    }
    printf("LVGL adapter initialized\n");
    
    // 初始化UI
    ui_init();
    printf("UI initialized\n");
    // 初始化自定义事件处理程序
    init_custom_events(&guider_ui);
    printf("Custom events initialized\n");
    // 创建传感器任务
    BaseType_t sensor_task_result = xTaskCreate(
        echo_task, 
        "uart_echo_task", 
        ECHO_TASK_STACK_SIZE, 
        NULL, 
        SENSOR_TASK_PRIORITY, 
        NULL
    );
    
    if (sensor_task_result != pdPASS) {
        printf("Failed to create sensor task\n");
    }
    
    // 创建网络任务
    BaseType_t network_task_result = xTaskCreate(
        network_task, 
        "network_task", 
        4096, 
        NULL, 
        NETWORK_TASK_PRIORITY, 
        NULL
    );
    
    if (network_task_result != pdPASS) {
        printf("Failed to create network task\n");
    }
    
    // 创建LVGL任务
    BaseType_t lvgl_task_result = xTaskCreate(
        lvgl_task, 
        "LVGL", 
        LVGL_TASK_STACK_SIZE, 
        NULL, 
        LVGL_TASK_PRIORITY, 
        NULL
    );
    
    if (lvgl_task_result != pdPASS) {
        printf("Failed to create LVGL task\n");
    }
    printf("aippd_v6.8.1\n");
    printf("All tasks created successfully\n");
    
    // 主任务可以做其他事情
    while(1) {
        // 这里可以添加其他应用程序逻辑
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}