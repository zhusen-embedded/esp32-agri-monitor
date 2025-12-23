#ifndef BLE_SITTING_WIFI_H
#define BLE_SITTING_WIFI_H

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Start BLE WiFi provisioning
 * 
 * @return esp_err_t ESP_OK on success
 */
esp_err_t ble_wifi_provisioning_start(void);

/**
 * @brief Stop BLE WiFi provisioning
 * 
 * @return esp_err_t ESP_OK on success
 */
esp_err_t ble_wifi_provisioning_stop(void);

#ifdef __cplusplus
}
#endif

#endif // BLE_SITTING_WIFI_H
