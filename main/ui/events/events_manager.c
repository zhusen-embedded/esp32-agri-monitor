// events_manager.c

#include "events_manager.h"
#include "events_temp_get.h"
#include "ble_sitting_wifi/ble_wifi_page.h"
#include "ble_sitting_wifi/ble_sitting_wifi.h"
#include "../generated/events_init.h"
#include <stdio.h>
#include "esp_event.h"
#include "esp_mac.h"
#include "esp_netif.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "lvgl.h"
#include <stdint.h>

static lv_ui *s_ui_ctx = NULL;
static lv_timer_t *s_wifi_timer = NULL;
static bool s_wifi_evt_registered = false;
static lv_obj_t *s_bound_wifi_menu_cont = NULL;
static lv_obj_t *s_bound_cb_1 = NULL;
static lv_obj_t *s_bound_cb_2 = NULL;
static lv_obj_t *s_bound_cb_3 = NULL;
static lv_obj_t *s_bound_cb_4 = NULL;
static lv_obj_t *s_bound_cb_5 = NULL;
static lv_obj_t *s_bound_btn_reset = NULL;
static lv_obj_t *s_bound_btn_reprovision = NULL;
static lv_obj_t *s_bound_sw_power = NULL;

static void wifi_menu_event_handler(lv_event_t *e);
extern void set_lvgl_task_delay_ms(uint32_t delay_ms);

typedef enum {
    HOME_ITEM_TEMPERATURE = 0,
    HOME_ITEM_HUMIDITY,
    HOME_ITEM_PH,
    HOME_ITEM_NPK,
    HOME_ITEM_LIGHT,
} home_item_t;

typedef enum {
    WIFI_UI_IDLE = 0,
    WIFI_UI_CONNECTING,
    WIFI_UI_CONNECTED,
    WIFI_UI_AUTH_FAIL,
    WIFI_UI_FAIL,
} wifi_ui_state_t;

static wifi_ui_state_t s_wifi_ui_state = WIFI_UI_IDLE;

static void update_provisioning_qrcode(void)
{
    if (!s_ui_ctx || !s_ui_ctx->sitting_scr_qrcode_1) {
        return;
    }

    uint8_t mac[6] = {0};
    if (esp_read_mac(mac, ESP_MAC_WIFI_STA) != ESP_OK) {
        return;
    }

    char dev_name[20] = {0};
    snprintf(dev_name, sizeof(dev_name), "TEMP_%02X%02X%02X", mac[3], mac[4], mac[5]);

    char qr_data[128] = {0};
    int qr_len = snprintf(qr_data, sizeof(qr_data),
                          "{\"ver\":\"v1\",\"name\":\"%s\",\"pop\":\"abcd1234\",\"transport\":\"ble\"}",
                          dev_name);
    if (qr_len <= 0 || qr_len >= (int)sizeof(qr_data)) {
        return;
    }

    lv_qrcode_update(s_ui_ctx->sitting_scr_qrcode_1, qr_data, (uint32_t)qr_len);
}

static bool is_wifi_connected(void)
{
    esp_netif_ip_info_t ip_info = {0};
    esp_netif_t *netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
    if (netif && esp_netif_get_ip_info(netif, &ip_info) == ESP_OK) {
        return ip_info.ip.addr != 0;
    }
    return false;
}

static void update_wifi_icon(void)
{
    if (!s_ui_ctx || !s_ui_ctx->screen_wifi_off || !s_ui_ctx->screen_wifi_on) {
        return;
    }

    if (is_wifi_connected()) {
        lv_obj_add_flag(s_ui_ctx->screen_wifi_off, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(s_ui_ctx->screen_wifi_on, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_clear_flag(s_ui_ctx->screen_wifi_off, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(s_ui_ctx->screen_wifi_on, LV_OBJ_FLAG_HIDDEN);
    }
}

static void set_obj_visible(lv_obj_t *obj, bool visible)
{
    if (!obj) {
        return;
    }

    if (visible) {
        lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
    }
}

static bool is_obj_visible(lv_obj_t *obj)
{
    return obj != NULL && !lv_obj_has_flag(obj, LV_OBJ_FLAG_HIDDEN);
}

static void set_home_item_visible(home_item_t item, bool visible)
{
    if (!s_ui_ctx) {
        return;
    }

    switch (item) {
        case HOME_ITEM_TEMPERATURE:
            set_obj_visible(s_ui_ctx->screen_label_2, visible);
            break;
        case HOME_ITEM_HUMIDITY:
            set_obj_visible(s_ui_ctx->screen_water_temp, visible);
            set_obj_visible(s_ui_ctx->screen_waring_4, visible);
            set_obj_visible(s_ui_ctx->screen_danger_4, visible);
            break;
        case HOME_ITEM_PH:
            set_obj_visible(s_ui_ctx->screen_ph_temp, visible);
            break;
        case HOME_ITEM_NPK:
            set_obj_visible(s_ui_ctx->screen_jia_bar, visible);
            set_obj_visible(s_ui_ctx->screen_ling_bar, visible);
            set_obj_visible(s_ui_ctx->screen_bar_1, visible);
            set_obj_visible(s_ui_ctx->screen_jia, visible);
            set_obj_visible(s_ui_ctx->screen_ling, visible);
            set_obj_visible(s_ui_ctx->screen_dan, visible);
            set_obj_visible(s_ui_ctx->screen_warring, visible);
            set_obj_visible(s_ui_ctx->screen_waring_2, visible);
            set_obj_visible(s_ui_ctx->screen_waring_3, visible);
            set_obj_visible(s_ui_ctx->screen_danger, visible);
            set_obj_visible(s_ui_ctx->screen_danger_2, visible);
            set_obj_visible(s_ui_ctx->screen_danger_3, visible);
            break;
        case HOME_ITEM_LIGHT:
            set_obj_visible(s_ui_ctx->screen_label_1, visible);
            break;
        default:
            break;
    }
}

static bool get_home_item_visible(home_item_t item)
{
    if (!s_ui_ctx) {
        return true;
    }

    switch (item) {
        case HOME_ITEM_TEMPERATURE:
            return is_obj_visible(s_ui_ctx->screen_label_2);
        case HOME_ITEM_HUMIDITY:
            return is_obj_visible(s_ui_ctx->screen_water_temp);
        case HOME_ITEM_PH:
            return is_obj_visible(s_ui_ctx->screen_ph_temp);
        case HOME_ITEM_NPK:
            return is_obj_visible(s_ui_ctx->screen_jia_bar) ||
                   is_obj_visible(s_ui_ctx->screen_ling_bar) ||
                   is_obj_visible(s_ui_ctx->screen_bar_1);
        case HOME_ITEM_LIGHT:
            return is_obj_visible(s_ui_ctx->screen_label_1);
        default:
            return true;
    }
}

static void sync_checkbox_with_home_item(lv_obj_t *checkbox, home_item_t item)
{
    if (!checkbox) {
        return;
    }

    if (get_home_item_visible(item)) {
        lv_obj_add_state(checkbox, LV_STATE_CHECKED);
    } else {
        lv_obj_clear_state(checkbox, LV_STATE_CHECKED);
    }
}

static void home_item_checkbox_event_handler(lv_event_t *e)
{
    if (lv_event_get_code(e) != LV_EVENT_VALUE_CHANGED) {
        return;
    }

    lv_obj_t *target = lv_event_get_target(e);
    home_item_t item = (home_item_t)(uintptr_t)lv_event_get_user_data(e);
    bool visible = lv_obj_has_state(target, LV_STATE_CHECKED);
    set_home_item_visible(item, visible);
}

static void apply_power_save_mode(bool enabled)
{
    set_sensor_low_power_mode(enabled);
    set_sensor_update_interval_ms(enabled ? 15000 : 5000);
    set_lvgl_task_delay_ms(enabled ? 40 : 10);
    printf("Power save mode %s\n", enabled ? "ON" : "OFF");
}

static void power_save_switch_event_handler(lv_event_t *e)
{
    if (lv_event_get_code(e) != LV_EVENT_VALUE_CHANGED) {
        return;
    }

    lv_obj_t *sw = lv_event_get_target(e);
    bool enabled = lv_obj_has_state(sw, LV_STATE_CHECKED);
    apply_power_save_mode(enabled);
}

static void reset_system_event_handler(lv_event_t *e)
{
    if (lv_event_get_code(e) != LV_EVENT_CLICKED) {
        return;
    }

    printf("System reset requested by UI button\n");
    esp_restart();
}

static void reprovision_event_handler(lv_event_t *e)
{
    if (lv_event_get_code(e) != LV_EVENT_CLICKED) {
        return;
    }

    printf("Force reprovision requested: clear saved WiFi and reboot\n");
    esp_err_t err = ble_wifi_force_reprovision();
    if (err != ESP_OK) {
        printf("ble_wifi_force_reprovision failed: %s\n", esp_err_to_name(err));
    }
    esp_restart();
}

static void init_home_item_settings(lv_ui *ui)
{
    if (!ui) {
        return;
    }

    set_home_item_visible(HOME_ITEM_TEMPERATURE, true);
    set_home_item_visible(HOME_ITEM_HUMIDITY, true);
    set_home_item_visible(HOME_ITEM_PH, true);
    set_home_item_visible(HOME_ITEM_NPK, true);
    set_home_item_visible(HOME_ITEM_LIGHT, true);

    if (ui->sitting_scr_cb_1) {
        lv_obj_add_state(ui->sitting_scr_cb_1, LV_STATE_CHECKED);
    }
    if (ui->sitting_scr_cb_2) {
        lv_obj_add_state(ui->sitting_scr_cb_2, LV_STATE_CHECKED);
    }
    if (ui->sitting_scr_cb_3) {
        lv_obj_add_state(ui->sitting_scr_cb_3, LV_STATE_CHECKED);
    }
    if (ui->sitting_scr_cb_4) {
        lv_obj_add_state(ui->sitting_scr_cb_4, LV_STATE_CHECKED);
    }
    if (ui->sitting_scr_cb_5) {
        lv_obj_add_state(ui->sitting_scr_cb_5, LV_STATE_CHECKED);
    }
}

static void bind_sitting_screen_events_if_needed(void)
{
    if (!s_ui_ctx) {
        return;
    }

    bool recreated = false;

    if (s_ui_ctx->sitting_scr_menu_1_cont_1 && s_bound_wifi_menu_cont != s_ui_ctx->sitting_scr_menu_1_cont_1) {
        lv_obj_add_event_cb(s_ui_ctx->sitting_scr_menu_1_cont_1, wifi_menu_event_handler, LV_EVENT_CLICKED, NULL);
        s_bound_wifi_menu_cont = s_ui_ctx->sitting_scr_menu_1_cont_1;
        recreated = true;
    }

    if (s_ui_ctx->sitting_scr_cb_1 && s_bound_cb_1 != s_ui_ctx->sitting_scr_cb_1) {
        lv_obj_add_event_cb(s_ui_ctx->sitting_scr_cb_1, home_item_checkbox_event_handler, LV_EVENT_VALUE_CHANGED, (void *)(uintptr_t)HOME_ITEM_TEMPERATURE);
        s_bound_cb_1 = s_ui_ctx->sitting_scr_cb_1;
        recreated = true;
    }
    if (s_ui_ctx->sitting_scr_cb_2 && s_bound_cb_2 != s_ui_ctx->sitting_scr_cb_2) {
        lv_obj_add_event_cb(s_ui_ctx->sitting_scr_cb_2, home_item_checkbox_event_handler, LV_EVENT_VALUE_CHANGED, (void *)(uintptr_t)HOME_ITEM_HUMIDITY);
        s_bound_cb_2 = s_ui_ctx->sitting_scr_cb_2;
        recreated = true;
    }
    if (s_ui_ctx->sitting_scr_cb_3 && s_bound_cb_3 != s_ui_ctx->sitting_scr_cb_3) {
        lv_obj_add_event_cb(s_ui_ctx->sitting_scr_cb_3, home_item_checkbox_event_handler, LV_EVENT_VALUE_CHANGED, (void *)(uintptr_t)HOME_ITEM_PH);
        s_bound_cb_3 = s_ui_ctx->sitting_scr_cb_3;
        recreated = true;
    }
    if (s_ui_ctx->sitting_scr_cb_4 && s_bound_cb_4 != s_ui_ctx->sitting_scr_cb_4) {
        lv_obj_add_event_cb(s_ui_ctx->sitting_scr_cb_4, home_item_checkbox_event_handler, LV_EVENT_VALUE_CHANGED, (void *)(uintptr_t)HOME_ITEM_NPK);
        s_bound_cb_4 = s_ui_ctx->sitting_scr_cb_4;
        recreated = true;
    }
    if (s_ui_ctx->sitting_scr_cb_5 && s_bound_cb_5 != s_ui_ctx->sitting_scr_cb_5) {
        lv_obj_add_event_cb(s_ui_ctx->sitting_scr_cb_5, home_item_checkbox_event_handler, LV_EVENT_VALUE_CHANGED, (void *)(uintptr_t)HOME_ITEM_LIGHT);
        s_bound_cb_5 = s_ui_ctx->sitting_scr_cb_5;
        recreated = true;
    }

    if (s_ui_ctx->sitting_scr_btn_1 && s_bound_btn_reset != s_ui_ctx->sitting_scr_btn_1) {
        lv_obj_add_event_cb(s_ui_ctx->sitting_scr_btn_1, reset_system_event_handler, LV_EVENT_CLICKED, NULL);
        s_bound_btn_reset = s_ui_ctx->sitting_scr_btn_1;
        recreated = true;
    }

    if (s_ui_ctx->sitting_scr_btn_2 && s_bound_btn_reprovision != s_ui_ctx->sitting_scr_btn_2) {
        lv_obj_add_event_cb(s_ui_ctx->sitting_scr_btn_2, reprovision_event_handler, LV_EVENT_CLICKED, NULL);
        s_bound_btn_reprovision = s_ui_ctx->sitting_scr_btn_2;
        recreated = true;
    }

    if (s_ui_ctx->sitting_scr_sw_1 && s_bound_sw_power != s_ui_ctx->sitting_scr_sw_1) {
        lv_obj_add_event_cb(s_ui_ctx->sitting_scr_sw_1, power_save_switch_event_handler, LV_EVENT_VALUE_CHANGED, NULL);
        s_bound_sw_power = s_ui_ctx->sitting_scr_sw_1;
        recreated = true;
    }

    if (recreated) {
        update_provisioning_qrcode();
        init_home_item_settings(s_ui_ctx);
        sync_checkbox_with_home_item(s_ui_ctx->sitting_scr_cb_1, HOME_ITEM_TEMPERATURE);
        sync_checkbox_with_home_item(s_ui_ctx->sitting_scr_cb_2, HOME_ITEM_HUMIDITY);
        sync_checkbox_with_home_item(s_ui_ctx->sitting_scr_cb_3, HOME_ITEM_PH);
        sync_checkbox_with_home_item(s_ui_ctx->sitting_scr_cb_4, HOME_ITEM_NPK);
        sync_checkbox_with_home_item(s_ui_ctx->sitting_scr_cb_5, HOME_ITEM_LIGHT);
        if (s_ui_ctx->sitting_scr_sw_1) {
            bool power_mode = get_sensor_low_power_mode();
            if (power_mode) {
                lv_obj_add_state(s_ui_ctx->sitting_scr_sw_1, LV_STATE_CHECKED);
            } else {
                lv_obj_clear_state(s_ui_ctx->sitting_scr_sw_1, LV_STATE_CHECKED);
            }
            apply_power_save_mode(power_mode);
        }
        printf("Sitting screen events rebound\n");
    }
}

static void wifi_status_timer_cb(lv_timer_t *timer)
{
    (void)timer;
    bind_sitting_screen_events_if_needed();
    update_wifi_icon();
}

static void apply_wifi_status_ui(void)
{
    if (!s_ui_ctx || !s_ui_ctx->sitting_scr_wif_conn_load || !s_ui_ctx->sitting_scr_spinner_1 ||
        !s_ui_ctx->sitting_scr_wifi_conn_unsucc || !s_ui_ctx->sitting_scr_wifi_conn_succ ||
        !s_ui_ctx->sitting_scr_wifi_Incorrect_password) {
        return;
    }

    lv_obj_add_flag(s_ui_ctx->sitting_scr_wif_conn_load, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(s_ui_ctx->sitting_scr_spinner_1, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(s_ui_ctx->sitting_scr_wifi_conn_unsucc, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(s_ui_ctx->sitting_scr_wifi_conn_succ, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(s_ui_ctx->sitting_scr_wifi_Incorrect_password, LV_OBJ_FLAG_HIDDEN);

    switch (s_wifi_ui_state) {
        case WIFI_UI_CONNECTING:
            if (ble_wifi_is_client_connected()) {
                lv_obj_clear_flag(s_ui_ctx->sitting_scr_wif_conn_load, LV_OBJ_FLAG_HIDDEN);
                lv_obj_clear_flag(s_ui_ctx->sitting_scr_spinner_1, LV_OBJ_FLAG_HIDDEN);
            }
            break;
        case WIFI_UI_CONNECTED:
            lv_obj_clear_flag(s_ui_ctx->sitting_scr_wifi_conn_succ, LV_OBJ_FLAG_HIDDEN);
            break;
        case WIFI_UI_AUTH_FAIL:
            lv_obj_clear_flag(s_ui_ctx->sitting_scr_wifi_Incorrect_password, LV_OBJ_FLAG_HIDDEN);
            break;
        case WIFI_UI_FAIL:
            lv_obj_clear_flag(s_ui_ctx->sitting_scr_wifi_conn_unsucc, LV_OBJ_FLAG_HIDDEN);
            break;
        case WIFI_UI_IDLE:
        default:
            break;
    }
}

static void wifi_status_ui_async_cb(void *arg)
{
    (void)arg;
    apply_wifi_status_ui();
}

static void set_wifi_ui_state(wifi_ui_state_t state)
{
    s_wifi_ui_state = state;
    lv_async_call(wifi_status_ui_async_cb, NULL);
}

static void wifi_status_event_handler(void *arg, esp_event_base_t event_base,
                                      int32_t event_id, void *event_data)
{
    (void)arg;
    if (event_base == WIFI_EVENT) {
        switch (event_id) {
            case WIFI_EVENT_STA_START:
            case WIFI_EVENT_STA_CONNECTED:
                set_wifi_ui_state(WIFI_UI_CONNECTING);
                break;
            case WIFI_EVENT_STA_DISCONNECTED: {
                wifi_event_sta_disconnected_t *disc = (wifi_event_sta_disconnected_t *)event_data;
                if (disc && disc->reason == WIFI_REASON_AUTH_FAIL) {
                    set_wifi_ui_state(WIFI_UI_AUTH_FAIL);
                } else {
                    set_wifi_ui_state(WIFI_UI_FAIL);
                }
                break;
            }
            default:
                break;
        }
        return;
    }

    if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        set_wifi_ui_state(WIFI_UI_CONNECTED);
    }
}
static void wifi_menu_event_handler(lv_event_t *e)
{
    printf("==> ble_wifi_provisioning_start second\n");
    on_wifi_page_enter(e); // 启动 BLE 配网（含防抖）
}
void init_custom_events(lv_ui *ui)
{
    printf("Initializing custom events...\n");
    s_ui_ctx = ui;
    start_sensor_data_updates(ui);
    bind_sitting_screen_events_if_needed();
    update_provisioning_qrcode();

    update_wifi_icon();
    if (s_wifi_timer == NULL) {
        s_wifi_timer = lv_timer_create(wifi_status_timer_cb, 1000, NULL);
    }

    if (!s_wifi_evt_registered) {
        ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_status_event_handler, NULL));
        ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_status_event_handler, NULL));
        s_wifi_evt_registered = true;
    }

    apply_wifi_status_ui();
    
    printf("Custom events initialized\n");
}