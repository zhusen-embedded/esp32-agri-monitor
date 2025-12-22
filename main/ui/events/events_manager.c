// events_manager.c

#include "events_manager.h"
#include "events_temp_get.h"
#include "ble_sitting_wifi/ble_wifi_page.h"
#include "../generated/events_init.h"
#include <stdio.h>
static void wifi_menu_event_handler(lv_event_t *e)
{
    printf("==> ble_wifi_provisioning_start second\n");
    on_wifi_page_enter(e); // 启动 BLE 配网（含防抖）
}
void init_custom_events(lv_ui *ui)
{
    printf("Initializing custom events...\n");
    if (ui->sitting_scr_menu_1_cont_1) {
        printf("==> ble_wifi_provisioning_start_first\n");
        lv_obj_add_event_cb(ui->sitting_scr_menu_1_cont_1, wifi_menu_event_handler, LV_EVENT_CLICKED, NULL);
    } else {
        printf("Error: ui->sitting_scr_menu_1_cont_1 is NULL!\n");
    }
    start_sensor_data_updates(ui);
    
    printf("Custom events initialized\n");
}