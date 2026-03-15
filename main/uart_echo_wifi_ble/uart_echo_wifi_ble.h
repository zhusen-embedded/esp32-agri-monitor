#ifndef _UART_ECHO_WIFI_BLE_H
#define _UART_ECHO_WIFI_BLE_H

#include "driver/uart.h"
#include "cJSON.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_http_client.h"
#include "esp_netif.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/semphr.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "esp_mac.h"
#include "esp_system.h"

#define ECHO_TASK_STACK_SIZE    3072
extern char esp32_id[13];
extern const char *model;

// 传感器数据结构
typedef struct {
    float moisture;
    float temperature;
    float conductivity;
    float ph;
    float nitrogen;
    float phosphorus;
    float potassium;
    float salinity;
} sensor_data_t;

// 函数声明
void wifi_init_sta(void);  // 添加WiFi初始化函数声明
void echo_task(void *arg); // 添加串口任务函数声明
void network_task(void *arg); // 添加网络任务函数声明
void get_esp32_id(char *esp32_id, size_t len); // 添加获取ID函数声明
float convert_moisture(uint16_t raw_value);
float convert_conductivity(uint16_t raw_value);
float convert_ph(uint16_t raw_value);
float convert_salinity(uint16_t raw_value);
float convert_temperature(uint16_t raw_value);
bool get_sensor_data(sensor_data_t *data, bool *has_new_data);
void set_sensor_low_power_mode(bool enabled);
bool get_sensor_low_power_mode(void);

#endif // _UART_ECHO_WIFI_BLE_H
