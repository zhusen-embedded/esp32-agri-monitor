// events_temp_get.c

#include "events_temp_get.h"
#include "../generated/gui_guider.h"
#include "lvgl.h"
#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "cJSON.h"
#include "../uart_echo_wifi_ble/uart_echo_wifi_ble.h"
#include "../uart_echo_wifi_ble/usb_pi_link.h"

static const char *TAG_UI = "UI";

static lv_timer_t *s_sensor_timer = NULL;
static lv_timer_t *s_ai_timer = NULL;
static lv_obj_t *s_ai_result_label = NULL;

static void update_ai_advice_display(lv_ui *ui)
{
    if (!ui || !ui->more_scr_more_ss_tab_1) {
        return;
    }

    if (s_ai_result_label == NULL) {
        s_ai_result_label = lv_label_create(ui->more_scr_more_ss_tab_1);
        lv_obj_set_pos(s_ai_result_label, 12, 12);
        lv_obj_set_size(s_ai_result_label, 290, 190);
        lv_label_set_long_mode(s_ai_result_label, LV_LABEL_LONG_WRAP);
        lv_obj_set_style_text_align(s_ai_result_label, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(s_ai_result_label, lv_color_hex(0x1f2937), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(s_ai_result_label, &lv_font_YouSheYuFeiTeJianKangTi_2_16, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_all(s_ai_result_label, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(s_ai_result_label, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    }

    char raw[256];
    char display[320];
    raw[0] = '\0';
    display[0] = '\0';

    if (!usb_pi_link_get_latest(raw, sizeof(raw))) {
        snprintf(display, sizeof(display), "等待树莓派推理结果...\n\n示例：\n{\"type\":\"infer\",\"label\":\"tomato\",\"conf\":0.93}");
        lv_label_set_text(s_ai_result_label, display);
        return;
    }

    cJSON *root = cJSON_Parse(raw);
    if (root != NULL) {
        cJSON *label = cJSON_GetObjectItemCaseSensitive(root, "label");
        cJSON *conf = cJSON_GetObjectItemCaseSensitive(root, "conf");
        cJSON *type = cJSON_GetObjectItemCaseSensitive(root, "type");

        const char *label_text = cJSON_IsString(label) ? label->valuestring : raw;
        const char *type_text = cJSON_IsString(type) ? type->valuestring : "infer";
        double conf_value = cJSON_IsNumber(conf) ? conf->valuedouble : -1.0;

        if (conf_value >= 0.0) {
            snprintf(display, sizeof(display),
                     "Pi infer result\n\nType: %.24s\nLabel: %.48s\nConf: %.2f\n\nRaw: %.160s",
                     type_text, label_text, conf_value, raw);
        } else {
            snprintf(display, sizeof(display),
                     "Pi infer result\n\nType: %.24s\nLabel: %.48s\n\nRaw: %.160s",
                     type_text, label_text, raw);
        }
        cJSON_Delete(root);
    } else {
        snprintf(display, sizeof(display), "Pi infer result\n\nRaw: %.200s", raw);
    }

    lv_label_set_text(s_ai_result_label, display);
}

static const char *get_light_level_text(float lux)
{
    if (lux < 50.0f) {
        return "极暗";
    }
    if (lux < 500.0f) {
        return "偏暗";
    }
    if (lux < 5000.0f) {
        return "适宜";
    }
    if (lux < 20000.0f) {
        return "偏强";
    }
    return "过强";
}

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

    ESP_LOGD(TAG_UI, "Updated more_scr_chart_1: N=%d, P=%d, K=%d", n_value, p_value, k_value);
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
            ESP_LOGD(TAG_UI, "Updating sensor display with new data...");

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
                ESP_LOGD(TAG_UI, "Updated screen_jia_bar to %d", potassium_value);
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
                ESP_LOGD(TAG_UI, "Updated screen_bar_1 to %d", nitrogen_value);
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
                ESP_LOGD(TAG_UI, "Updated screen_ling_bar to %d", phosphorus_value);
            }
            
            // 更新湿度显示 (screen_water_temp)
            if (ui->screen_water_temp) {
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
                snprintf(buffer, sizeof(buffer), "湿度: %.1f%%", sensor_data.moisture);
                lv_label_set_text(ui->screen_water_temp, buffer);
                ESP_LOGD(TAG_UI, "Updated screen_water_temp to %s", buffer);
            }
            
            // 更新PH值显示 (screen_ph_temp)
            if (ui->screen_ph_temp) {
                snprintf(buffer, sizeof(buffer), "PH: %.1f", sensor_data.ph);
                lv_label_set_text(ui->screen_ph_temp, buffer);
                ESP_LOGD(TAG_UI, "Updated screen_ph_temp to %s", buffer);
            }
            
            // 更新温度显示 (screen_label_2)
            if (ui->screen_label_2) {
                snprintf(buffer, sizeof(buffer), "温度: %.1f度", sensor_data.temperature);
                lv_label_set_text(ui->screen_label_2, buffer);
                ESP_LOGD(TAG_UI, "Updated screen_label_2 to %s", buffer);
            }

            // 更新光照显示 (screen_label_1)
            if (ui->screen_label_1) {
                snprintf(buffer, sizeof(buffer), "光照: %s", get_light_level_text(sensor_data.light));
                lv_label_set_text(ui->screen_label_1, buffer);
                ESP_LOGD(TAG_UI, "Updated screen_label_1 to %s", buffer);
            }

            // 更新 more_scr 的 N/P/K 图表
            update_npk_chart(ui, &sensor_data);
            ESP_LOGD(TAG_UI, "Sensor display updated successfully");
        } else {
            ESP_LOGD(TAG_UI, "No new sensor data available");
        }
    } else {
        ESP_LOGD(TAG_UI, "Failed to get sensor data");
    }
}

// 定时器回调函数 - 定期检查并更新传感器数据
static void sensor_update_timer_cb(lv_timer_t *timer)
{
    ESP_LOGD(TAG_UI, "Sensor update timer callback triggered");
    lv_ui *ui = (lv_ui *)lv_timer_get_user_data(timer);
    if (ui) {
        update_sensor_display(ui);
    } else {
        ESP_LOGD(TAG_UI, "UI pointer is NULL in timer callback");
    }
}

static void ai_update_timer_cb(lv_timer_t *timer)
{
    lv_ui *ui = (lv_ui *)lv_timer_get_user_data(timer);
    if (ui) {
        update_ai_advice_display(ui);
    }
}

// 启动传感器数据更新定时器
void start_sensor_data_updates(lv_ui *ui)
{
    ESP_LOGI(TAG_UI, "Starting sensor data updates with 5-second interval...");

    if (s_sensor_timer == NULL) {
        // 创建一个定时器，每5000ms检查并更新传感器数据
        s_sensor_timer = lv_timer_create(sensor_update_timer_cb, 5000, ui);
        if (s_sensor_timer == NULL) {
            ESP_LOGE(TAG_UI, "Failed to create sensor update timer");
            return;
        }
    } else {
        lv_timer_set_user_data(s_sensor_timer, ui);
        lv_timer_set_period(s_sensor_timer, 5000);
    }

    if (s_ai_timer == NULL) {
        s_ai_timer = lv_timer_create(ai_update_timer_cb, 1000, ui);
        if (s_ai_timer == NULL) {
            ESP_LOGE(TAG_UI, "Failed to create AI update timer");
        }
    } else {
        lv_timer_set_user_data(s_ai_timer, ui);
        lv_timer_set_period(s_ai_timer, 1000);
    }

    update_ai_advice_display(ui);
    
    ESP_LOGD(TAG_UI, "Sensor data update timer started successfully with 5-second interval");
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
    ESP_LOGI(TAG_UI, "Sensor UI update interval set to %lu ms", (unsigned long)interval_ms);
}

// 手动触发一次传感器数据显示更新
void force_sensor_display_update(lv_ui *ui)
{
    ESP_LOGD(TAG_UI, "Forcing sensor display update...");
    update_sensor_display(ui);
}


