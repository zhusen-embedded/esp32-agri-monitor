// events_temp_get.c

#include "events_temp_get.h"
#include "../generated/gui_guider.h"
#include "lvgl.h"
#include <stdio.h>
#include "../uart_echo_wifi_ble/uart_echo_wifi_ble.h"

static lv_timer_t *s_sensor_timer = NULL;

static void update_npk_chart(lv_ui *ui, const sensor_data_t *sensor_data)
{
    if (!ui || !sensor_data || !ui->more_scr_chart_1 || !ui->more_scr_chart_1_0) {
        return;
    }

    int n_value = (int)sensor_data->nitrogen;
    int p_value = (int)sensor_data->phosphorus;
    int k_value = (int)sensor_data->potassium;

    if (n_value < 0) n_value = 0;
    if (p_value < 0) p_value = 0;
    if (k_value < 0) k_value = 0;

    int max_value = n_value;
    if (p_value > max_value) max_value = p_value;
    if (k_value > max_value) max_value = k_value;
    if (max_value < 100) {
        max_value = 100;
    } else {
        max_value = max_value + max_value / 5; // 留 20% 头部空间
    }

    lv_chart_set_range(ui->more_scr_chart_1, LV_CHART_AXIS_PRIMARY_Y, 0, max_value);
    lv_chart_set_value_by_id(ui->more_scr_chart_1, ui->more_scr_chart_1_0, 0, n_value);
    lv_chart_set_value_by_id(ui->more_scr_chart_1, ui->more_scr_chart_1_0, 1, p_value);
    lv_chart_set_value_by_id(ui->more_scr_chart_1, ui->more_scr_chart_1_0, 2, k_value);
    lv_chart_refresh(ui->more_scr_chart_1);

    printf("Updated more_scr_chart_1: N=%d, P=%d, K=%d\n", n_value, p_value, k_value);
}

// 更新传感器数据显示的函数
void update_sensor_display(lv_ui *ui)
{
    // 加强空指针检查
    if (ui == NULL) {
        printf("Error: ui is NULL\n");
        return;
    }
    char buffer[64];   
    // 获取传感器数据
    sensor_data_t sensor_data;
    bool has_new_data = false;
    
    if (get_sensor_data(&sensor_data, &has_new_data)) {
        if (has_new_data) {
            printf("Updating sensor display with new data...\n");

            bool npk_visible = (ui->screen_jia_bar != NULL) &&
                               !lv_obj_has_flag(ui->screen_jia_bar, LV_OBJ_FLAG_HIDDEN);
            bool humidity_visible = (ui->screen_water_temp != NULL) &&
                                    !lv_obj_has_flag(ui->screen_water_temp, LV_OBJ_FLAG_HIDDEN);
            
            // 更新钾显示 (screen_jia_bar)
            if (ui->screen_jia_bar) {
                int potassium_value = (int)sensor_data.potassium;
                if (potassium_value < 0) potassium_value = 0;
                if (potassium_value > 600) potassium_value = 600;
                if (!npk_visible) {
                    lv_obj_add_flag(ui->screen_danger, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_add_flag(ui->screen_warring, LV_OBJ_FLAG_HIDDEN);
                } else if (potassium_value < 10) {
                    lv_obj_clear_flag(ui->screen_danger, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_add_flag(ui->screen_warring, LV_OBJ_FLAG_HIDDEN);
                } else if (potassium_value < 50 || potassium_value > 500) {
                    lv_obj_add_flag(ui->screen_danger, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_clear_flag(ui->screen_warring, LV_OBJ_FLAG_HIDDEN);
                } else {
                    lv_obj_clear_flag(ui->screen_danger, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_clear_flag(ui->screen_warring, LV_OBJ_FLAG_HIDDEN);
                }
                lv_bar_set_value(ui->screen_jia_bar, potassium_value, LV_ANIM_ON);
                printf("Updated screen_jia_bar to %d\n", potassium_value);
            }
            
            // 更新氮显示 (screen_bar_1)
            if (ui->screen_bar_1) {
                int nitrogen_value = (int)sensor_data.nitrogen;
                if (nitrogen_value < 0) nitrogen_value = 0;
                if (nitrogen_value > 200) nitrogen_value = 200;
                if (!npk_visible) {
                    lv_obj_add_flag(ui->screen_danger_3, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_add_flag(ui->screen_waring_3, LV_OBJ_FLAG_HIDDEN);
                } else if (nitrogen_value <= 5) {
                    lv_obj_clear_flag(ui->screen_danger_3, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_add_flag(ui->screen_waring_3, LV_OBJ_FLAG_HIDDEN);
                } else if (nitrogen_value <= 20 || 150 <= nitrogen_value) {
                    lv_obj_add_flag(ui->screen_danger_3, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_clear_flag(ui->screen_waring_3, LV_OBJ_FLAG_HIDDEN);
                } else {
                    lv_obj_clear_flag(ui->screen_danger_3, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_clear_flag(ui->screen_waring_3, LV_OBJ_FLAG_HIDDEN);
                }
                lv_bar_set_value(ui->screen_bar_1, nitrogen_value, LV_ANIM_ON);
                printf("Updated screen_bar_1 to %d\n", nitrogen_value);
            }
            
            // 更新磷显示 (screen_ling_bar)
            if (ui->screen_ling_bar) {
                int phosphorus_value = (int)sensor_data.phosphorus;
                if (phosphorus_value < 0) phosphorus_value = 0;
                if (phosphorus_value > 50) phosphorus_value = 50;
                if (!npk_visible) {
                    lv_obj_add_flag(ui->screen_danger_2, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_add_flag(ui->screen_waring_2, LV_OBJ_FLAG_HIDDEN);
                } else if (phosphorus_value < 10) {
                    lv_obj_clear_flag(ui->screen_danger_2, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_add_flag(ui->screen_waring_2, LV_OBJ_FLAG_HIDDEN);
                } else if (phosphorus_value <= 2 || phosphorus_value > 80) {
                    lv_obj_add_flag(ui->screen_danger_2, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_clear_flag(ui->screen_waring_2, LV_OBJ_FLAG_HIDDEN);
                } else {
                    lv_obj_clear_flag(ui->screen_danger_2, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_clear_flag(ui->screen_waring_2, LV_OBJ_FLAG_HIDDEN);
                }
                lv_bar_set_value(ui->screen_ling_bar, phosphorus_value, LV_ANIM_ON);
                printf("Updated screen_ling_bar to %d\n", phosphorus_value);
            }
            
            // 更新湿度显示 (screen_water_temp)
            if (ui->screen_water_temp) {
                snprintf(buffer, sizeof(buffer), "湿度: %.1f%%", sensor_data.moisture);
                lv_label_set_text(ui->screen_water_temp, buffer);
                if (!humidity_visible) {
                    lv_obj_add_flag(ui->screen_danger_4, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_add_flag(ui->screen_waring_4, LV_OBJ_FLAG_HIDDEN);
                } else if (sensor_data.moisture < 10) {
                    lv_obj_clear_flag(ui->screen_danger_4, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_add_flag(ui->screen_waring_4, LV_OBJ_FLAG_HIDDEN);
                } else if (sensor_data.moisture < 30 || 95 <= sensor_data.moisture) {
                    lv_obj_add_flag(ui->screen_danger_4, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_clear_flag(ui->screen_waring_4, LV_OBJ_FLAG_HIDDEN);
                } else {
                    lv_obj_clear_flag(ui->screen_danger_4, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_clear_flag(ui->screen_waring_4, LV_OBJ_FLAG_HIDDEN);
                }
                printf("Updated screen_water_temp to %s\n", buffer);
            }
            
            // 更新PH值显示 (screen_ph_temp)
            if (ui->screen_ph_temp) {
                snprintf(buffer, sizeof(buffer), "PH: %.1f", sensor_data.ph);
                lv_label_set_text(ui->screen_ph_temp, buffer);
                printf("Updated screen_ph_temp to %s\n", buffer);
            }
            
            // 更新温度显示 (screen_label_2)
            if (ui->screen_label_2) {
                snprintf(buffer, sizeof(buffer), "温度: %.1f度", sensor_data.temperature);
                lv_label_set_text(ui->screen_label_2, buffer);
                printf("Updated screen_label_2 to %s\n", buffer);
            }

            // 更新 more_scr 的 N/P/K 图表
            update_npk_chart(ui, &sensor_data);
            
            printf("Sensor display updated successfully\n");
        } else {
            printf("No new sensor data available\n");
        }
    } else {
        printf("Failed to get sensor data\n");
    }
}

// 定时器回调函数 - 定期检查并更新传感器数据
static void sensor_update_timer_cb(lv_timer_t *timer)
{
    printf("Sensor update timer callback triggered\n");
    lv_ui *ui = (lv_ui *)lv_timer_get_user_data(timer);
    if (ui) {
        update_sensor_display(ui);
    } else {
        printf("UI pointer is NULL in timer callback\n");
    }
}

// 启动传感器数据更新定时器
void start_sensor_data_updates(lv_ui *ui)
{
    printf("Starting sensor data updates with 5-second interval...\n");

    if (s_sensor_timer == NULL) {
        // 创建一个定时器，每5000ms检查并更新传感器数据
        s_sensor_timer = lv_timer_create(sensor_update_timer_cb, 5000, ui);
        if (s_sensor_timer == NULL) {
            printf("Failed to create sensor update timer\n");
            return;
        }
    } else {
        lv_timer_set_user_data(s_sensor_timer, ui);
        lv_timer_set_period(s_sensor_timer, 5000);
    }
    
    printf("Sensor data update timer started successfully with 5-second interval\n");
}

void set_sensor_update_interval_ms(uint32_t interval_ms)
{
    if (s_sensor_timer == NULL) {
        return;
    }

    if (interval_ms < 500) {
        interval_ms = 500;
    }
    lv_timer_set_period(s_sensor_timer, interval_ms);
    printf("Sensor UI update interval set to %lu ms\n", (unsigned long)interval_ms);
}

// 手动触发一次传感器数据显示更新
void force_sensor_display_update(lv_ui *ui)
{
    printf("Forcing sensor display update...\n");
    update_sensor_display(ui);
}


