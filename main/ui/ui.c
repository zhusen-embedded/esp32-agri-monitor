#include "ui.h"
#include "ui/generated/gui_guider.h"
#include "ui/generated/events_init.h"
#include "ui/generated/widgets_init.h"
#include "ui/custom/custom.h"
/**********************
 **********************/
lv_ui guider_ui;
void ui_init(void)
{
    // 初始化屏幕删除标志
    init_scr_del_flag(&guider_ui);
    
    // 初始化键盘
    init_keyboard(&guider_ui);
    
    // 设置主屏幕
    setup_scr_screen(&guider_ui);
    setup_scr_sitting_scr(&guider_ui);          // 屏幕二
    setup_scr_more_scr(&guider_ui);      // 其他页面
    // 初始化事件
    events_init(&guider_ui);
    
    // 初始化自定义功能
    custom_init(&guider_ui);
    // 加载屏幕
    lv_screen_load(guider_ui.screen);
}
