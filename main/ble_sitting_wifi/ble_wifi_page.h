#ifndef BLE_WIFI_PAGE_H
#define BLE_WIFI_PAGE_H

#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Callback for entering the WiFi page
 * 
 * @param e LVGL event
 */
void on_wifi_page_enter(lv_event_t *e);

/**
 * @brief Callback for exiting the WiFi page
 * 
 * @param e LVGL event
 */
void on_wifi_page_exit(lv_event_t *e);

#ifdef __cplusplus
}
#endif

#endif // BLE_WIFI_PAGE_H
