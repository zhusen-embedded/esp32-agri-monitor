#ifndef BLE_SITTING_WIFI_H
#define BLE_SITTING_WIFI_H

#include "esp_err.h"
#include <stdbool.h>
#include <stdint.h>

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

/**
 * @brief Force re-provisioning of BLE WiFi
 * 
 * @return esp_err_t ESP_OK on success
 */
esp_err_t ble_wifi_force_reprovision(void);

/**
 * @brief Try to connect to the saved WiFi configuration once with timeout
 *
 * @param timeout_ms Max wait time in milliseconds
 * @return esp_err_t ESP_OK on connected, ESP_ERR_TIMEOUT on timeout, other errors on setup failure
 */
esp_err_t ble_wifi_try_connect_saved(uint32_t timeout_ms);

/**
 * @brief Query whether a BLE client is currently connected
 *
 * @return true if connected
 * @return false otherwise
 */
bool ble_wifi_is_client_connected(void);

#ifdef __cplusplus
}
#endif

#endif // BLE_SITTING_WIFI_H
