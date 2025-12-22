#include "ble_sitting_wifi/ble_sitting_wifi.h"
#include "lvgl.h"
#include <stdint.h>
#include <stdbool.h>
#include <sys/time.h>

static uint64_t last_ble_action_time = 0;
#define BLE_DEBOUNCE_MS 1000

static uint64_t get_time_ms(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (uint64_t)tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

static void ble_wifi_start_safe(void) {
    printf("==> ble_wifi_provisioning_start four\n");
    uint64_t now = get_time_ms();
    if (now - last_ble_action_time > BLE_DEBOUNCE_MS) {
        ble_wifi_provisioning_start();
        last_ble_action_time = now;
    }
}

static void ble_wifi_stop_safe(void) {
    uint64_t now = get_time_ms();
    if (now - last_ble_action_time > BLE_DEBOUNCE_MS) {
        ble_wifi_provisioning_stop();
        last_ble_action_time = now;
    }
}

// 在 WiFi 设置页面加载事件中调用
void on_wifi_page_enter(lv_event_t *e) {
    printf("==> ble_wifi_provisioning_start three\n");
    ble_wifi_start_safe();
}

// 在 WiFi 设置页面卸载/离开事件中调用
void on_wifi_page_exit(lv_event_t *e) {
    ble_wifi_stop_safe();
}