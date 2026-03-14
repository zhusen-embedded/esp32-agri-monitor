// events_manager.c

#include "events_manager.h"
#include "events_temp_get.h"
#include "ble_sitting_wifi/ble_wifi_page.h"
#include "../generated/events_init.h"
#include <stdio.h>
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "lvgl.h"

static lv_ui *s_ui_ctx = NULL;
static lv_timer_t *s_wifi_timer = NULL;
static bool s_wifi_evt_registered = false;

typedef enum {
    WIFI_UI_IDLE = 0,
    WIFI_UI_CONNECTING,
    WIFI_UI_CONNECTED,
    WIFI_UI_AUTH_FAIL,
    WIFI_UI_FAIL,
} wifi_ui_state_t;

static wifi_ui_state_t s_wifi_ui_state = WIFI_UI_IDLE;

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

static void wifi_status_timer_cb(lv_timer_t *timer)
{
    (void)timer;
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
            lv_obj_clear_flag(s_ui_ctx->sitting_scr_wif_conn_load, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(s_ui_ctx->sitting_scr_spinner_1, LV_OBJ_FLAG_HIDDEN);
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
    if (ui->sitting_scr_menu_1_cont_1) {
        printf("==> ble_wifi_provisioning_start_first\n");
        lv_obj_add_event_cb(ui->sitting_scr_menu_1_cont_1, wifi_menu_event_handler, LV_EVENT_CLICKED, NULL);
    } else {
        printf("Error: ui->sitting_scr_menu_1_cont_1 is NULL!\n");
    }
    start_sensor_data_updates(ui);

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