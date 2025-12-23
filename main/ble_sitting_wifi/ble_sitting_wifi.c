#include "ble_sitting_wifi.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "wifi_provisioning/manager.h"
#include "wifi_provisioning/scheme_ble.h"

static const char *TAG = "BLE_PROV";

// 配网完成事件回调
static void wifi_prov_event_handler(void* arg, esp_event_base_t event_base,
                                    int32_t event_id, void* event_data)
{
    switch (event_id) {
        case WIFI_PROV_START:
            ESP_LOGI(TAG, "Provisioning started");
            break;
        case WIFI_PROV_CRED_RECV: {
            wifi_sta_config_t *sta_cfg = (wifi_sta_config_t *)event_data;
            ESP_LOGI(TAG, "Received SSID: %s, Password: %s", sta_cfg->ssid, sta_cfg->password);
            break;
        }
        case WIFI_PROV_CRED_SUCCESS:
            ESP_LOGI(TAG, "Provisioning successful");
            break;
        case WIFI_PROV_CRED_FAIL:
            ESP_LOGI(TAG, "Provisioning failed");
            break;
        case WIFI_PROV_END:
            ESP_LOGI(TAG, "Provisioning ended");
            break;
        default:
            break;
    }   
}

esp_err_t ble_wifi_provisioning_start(void)
{
    // // 初始化 NVS
    // printf("==> ble_wifi_provisioning_start five\n");
    // esp_err_t err = nvs_flash_init();
    // if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    //     ESP_ERROR_CHECK(nvs_flash_erase());
    //     err = nvs_flash_init();
    // }
    // ESP_ERROR_CHECK(err);

    // // 初始化 TCP/IP、事件循环
    // ESP_ERROR_CHECK(esp_netif_init());
    // ESP_ERROR_CHECK(esp_event_loop_create_default());
    // 初始化 WiFi
    // wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    // ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_FLASH));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());

    // 注册配网事件
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_PROV_EVENT, ESP_EVENT_ANY_ID, &wifi_prov_event_handler, NULL));

    // 配网参数
    wifi_prov_mgr_config_t config = {
        .scheme = wifi_prov_scheme_ble,
        .scheme_event_handler = WIFI_PROV_SCHEME_BLE_EVENT_HANDLER_FREE_BTDM
    };

    // 初始化配网管理器
    ESP_ERROR_CHECK(wifi_prov_mgr_init(config));

    // 检查是否已配网
    bool provisioned = false;
    ESP_ERROR_CHECK(wifi_prov_mgr_is_provisioned(&provisioned));
    if (!provisioned) {
        // 启动 BLE 配网
        uint8_t eth_mac[6];
        esp_wifi_get_mac(WIFI_IF_STA, eth_mac);
        char service_name[32];
        snprintf(service_name, sizeof(service_name), "BLE_PROV_%02X%02X%02X%02X%02X%02X",
                 eth_mac[0], eth_mac[1], eth_mac[2], eth_mac[3], eth_mac[4], eth_mac[5]);

        const char *service_key = NULL;
        ESP_LOGI(TAG, "Starting BLE provisioning with name: %s", service_name);
        ESP_ERROR_CHECK(wifi_prov_mgr_start_provisioning(
            WIFI_PROV_SECURITY_1, NULL, service_name, service_key));
    } else {
        ESP_LOGI(TAG, "Already provisioned, starting WiFi STA");
        wifi_prov_mgr_deinit();
    }
    return ESP_OK;
}

esp_err_t ble_wifi_provisioning_stop(void)
{
    ESP_LOGI(TAG, "Stopping BLE provisioning");
    wifi_prov_mgr_deinit();
    return ESP_OK;
}