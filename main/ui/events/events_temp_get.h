// events_temp_get.h

#ifndef EVENTS_TEMP_GET_H
#define EVENTS_TEMP_GET_H

#include "../generated/gui_guider.h"
#include "lvgl.h"
#include "uart_echo_wifi_ble.h"
#ifdef __cplusplus
extern "C" {
#endif

// 启动传感器数据自动更新
void start_sensor_data_updates(lv_ui *ui);

// 动态设置传感器数据显示刷新周期
void set_sensor_update_interval_ms(uint32_t interval_ms);

// 强制立即更新传感器显示
void force_sensor_display_update(lv_ui *ui);

#ifdef __cplusplus
}
#endif

#endif /* EVENTS_TEMP_GET_H */