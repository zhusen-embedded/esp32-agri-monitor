/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "widgets_init.h"
#include "custom.h"



void setup_scr_sitting_scr(lv_ui *ui)
{
    //Write codes sitting_scr
    ui->sitting_scr = lv_obj_create(NULL);
    lv_obj_set_size(ui->sitting_scr, 320, 240);
    lv_obj_set_scrollbar_mode(ui->sitting_scr, LV_SCROLLBAR_MODE_OFF);
    lv_obj_remove_flag(ui->sitting_scr, LV_OBJ_FLAG_SCROLLABLE);

    //Write style for sitting_scr, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->sitting_scr, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes sitting_scr_menu_1
    ui->sitting_scr_menu_1 = lv_menu_create(ui->sitting_scr);
    lv_obj_set_pos(ui->sitting_scr_menu_1, 0, -2);
    lv_obj_set_size(ui->sitting_scr_menu_1, 320, 240);
    lv_obj_set_scrollbar_mode(ui->sitting_scr_menu_1, LV_SCROLLBAR_MODE_OFF);
    lv_obj_remove_flag(ui->sitting_scr_menu_1, LV_OBJ_FLAG_SCROLLABLE);

    //Create sidebar page for menu sitting_scr_menu_1
    ui->sitting_scr_menu_1_sidebar_page = lv_menu_page_create(ui->sitting_scr_menu_1, "设置");
    lv_menu_set_sidebar_page(ui->sitting_scr_menu_1, ui->sitting_scr_menu_1_sidebar_page);
    lv_obj_set_scrollbar_mode(ui->sitting_scr_menu_1_sidebar_page, LV_SCROLLBAR_MODE_OFF);

    //Create subpage for sitting_scr_menu_1
    lv_obj_t * sitting_scr_menu_1_subpage_1 = lv_menu_page_create(ui->sitting_scr_menu_1, NULL);
    ui->sitting_scr_menu_1_subpage_1_cont = lv_menu_cont_create(sitting_scr_menu_1_subpage_1);
    lv_obj_set_layout(ui->sitting_scr_menu_1_subpage_1_cont, LV_LAYOUT_NONE);
    ui->sitting_scr_menu_1_cont_1 = lv_menu_cont_create(ui->sitting_scr_menu_1_sidebar_page);
    ui->sitting_scr_menu_1_label_1 = lv_label_create(ui->sitting_scr_menu_1_cont_1);
    lv_label_set_text(ui->sitting_scr_menu_1_label_1, "wifi");
    lv_obj_set_size(ui->sitting_scr_menu_1_label_1, LV_PCT(100), LV_SIZE_CONTENT);
    lv_label_set_long_mode(ui->sitting_scr_menu_1_label_1, LV_LABEL_LONG_CLIP);
    lv_obj_set_scrollbar_mode(sitting_scr_menu_1_subpage_1, LV_SCROLLBAR_MODE_OFF);
    lv_menu_set_load_page_event(ui->sitting_scr_menu_1, ui->sitting_scr_menu_1_cont_1, sitting_scr_menu_1_subpage_1);

    //Create subpage for sitting_scr_menu_1
    lv_obj_t * sitting_scr_menu_1_subpage_2 = lv_menu_page_create(ui->sitting_scr_menu_1, NULL);
    ui->sitting_scr_menu_1_subpage_2_cont = lv_menu_cont_create(sitting_scr_menu_1_subpage_2);
    lv_obj_set_layout(ui->sitting_scr_menu_1_subpage_2_cont, LV_LAYOUT_NONE);
    ui->sitting_scr_menu_1_cont_2 = lv_menu_cont_create(ui->sitting_scr_menu_1_sidebar_page);
    ui->sitting_scr_menu_1_label_2 = lv_label_create(ui->sitting_scr_menu_1_cont_2);
    lv_label_set_text(ui->sitting_scr_menu_1_label_2, "屏幕设置");
    lv_obj_set_size(ui->sitting_scr_menu_1_label_2, LV_PCT(100), LV_SIZE_CONTENT);
    lv_label_set_long_mode(ui->sitting_scr_menu_1_label_2, LV_LABEL_LONG_CLIP);
    lv_obj_set_scrollbar_mode(sitting_scr_menu_1_subpage_2, LV_SCROLLBAR_MODE_OFF);
    lv_menu_set_load_page_event(ui->sitting_scr_menu_1, ui->sitting_scr_menu_1_cont_2, sitting_scr_menu_1_subpage_2);

    //Create subpage for sitting_scr_menu_1
    lv_obj_t * sitting_scr_menu_1_subpage_3 = lv_menu_page_create(ui->sitting_scr_menu_1, NULL);
    ui->sitting_scr_menu_1_subpage_3_cont = lv_menu_cont_create(sitting_scr_menu_1_subpage_3);
    lv_obj_set_layout(ui->sitting_scr_menu_1_subpage_3_cont, LV_LAYOUT_NONE);
    ui->sitting_scr_menu_1_cont_3 = lv_menu_cont_create(ui->sitting_scr_menu_1_sidebar_page);
    ui->sitting_scr_menu_1_label_3 = lv_label_create(ui->sitting_scr_menu_1_cont_3);
    lv_label_set_text(ui->sitting_scr_menu_1_label_3, "功能");
    lv_obj_set_size(ui->sitting_scr_menu_1_label_3, LV_PCT(100), LV_SIZE_CONTENT);
    lv_label_set_long_mode(ui->sitting_scr_menu_1_label_3, LV_LABEL_LONG_CLIP);
    lv_obj_set_scrollbar_mode(sitting_scr_menu_1_subpage_3, LV_SCROLLBAR_MODE_OFF);
    lv_menu_set_load_page_event(ui->sitting_scr_menu_1, ui->sitting_scr_menu_1_cont_3, sitting_scr_menu_1_subpage_3);

    //Write style for sitting_scr_menu_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_radius(ui->sitting_scr_menu_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->sitting_scr_menu_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->sitting_scr_menu_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->sitting_scr_menu_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->sitting_scr_menu_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for sitting_scr_menu_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_margin_hor(ui->sitting_scr_menu_1_sidebar_page, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_margin_ver(ui->sitting_scr_menu_1_sidebar_page, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->sitting_scr_menu_1_sidebar_page, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->sitting_scr_menu_1_sidebar_page, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->sitting_scr_menu_1_sidebar_page, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->sitting_scr_menu_1_sidebar_page, lv_color_hex(0xf6f6f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->sitting_scr_menu_1_sidebar_page, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_sitting_scr_menu_1_extra_option_btns_main_default
    static lv_style_t style_sitting_scr_menu_1_extra_option_btns_main_default;
    ui_init_style(&style_sitting_scr_menu_1_extra_option_btns_main_default);

    lv_style_set_text_color(&style_sitting_scr_menu_1_extra_option_btns_main_default, lv_color_hex(0x151212));
    lv_style_set_text_font(&style_sitting_scr_menu_1_extra_option_btns_main_default, &lv_font_YouSheYuFeiTeJianKangTi_2_16);
    lv_style_set_text_opa(&style_sitting_scr_menu_1_extra_option_btns_main_default, 255);
    lv_style_set_text_align(&style_sitting_scr_menu_1_extra_option_btns_main_default, LV_TEXT_ALIGN_CENTER);
    lv_style_set_pad_top(&style_sitting_scr_menu_1_extra_option_btns_main_default, 10);
    lv_style_set_pad_bottom(&style_sitting_scr_menu_1_extra_option_btns_main_default, 10);
    lv_obj_add_style(ui->sitting_scr_menu_1_cont_3, &style_sitting_scr_menu_1_extra_option_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_style(ui->sitting_scr_menu_1_cont_2, &style_sitting_scr_menu_1_extra_option_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_style(ui->sitting_scr_menu_1_cont_1, &style_sitting_scr_menu_1_extra_option_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_CHECKED for &style_sitting_scr_menu_1_extra_option_btns_main_checked
    static lv_style_t style_sitting_scr_menu_1_extra_option_btns_main_checked;
    ui_init_style(&style_sitting_scr_menu_1_extra_option_btns_main_checked);

    lv_style_set_text_color(&style_sitting_scr_menu_1_extra_option_btns_main_checked, lv_color_hex(0x1910bc));
    lv_style_set_text_font(&style_sitting_scr_menu_1_extra_option_btns_main_checked, &lv_font_YouSheYuFeiTeJianKangTi_2_12);
    lv_style_set_text_opa(&style_sitting_scr_menu_1_extra_option_btns_main_checked, 255);
    lv_style_set_text_align(&style_sitting_scr_menu_1_extra_option_btns_main_checked, LV_TEXT_ALIGN_CENTER);
    lv_style_set_border_width(&style_sitting_scr_menu_1_extra_option_btns_main_checked, 0);
    lv_style_set_radius(&style_sitting_scr_menu_1_extra_option_btns_main_checked, 5);
    lv_style_set_bg_opa(&style_sitting_scr_menu_1_extra_option_btns_main_checked, 63);
    lv_style_set_bg_color(&style_sitting_scr_menu_1_extra_option_btns_main_checked, lv_color_hex(0x1277b8));
    lv_style_set_bg_grad_dir(&style_sitting_scr_menu_1_extra_option_btns_main_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(ui->sitting_scr_menu_1_cont_3, &style_sitting_scr_menu_1_extra_option_btns_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_add_style(ui->sitting_scr_menu_1_cont_2, &style_sitting_scr_menu_1_extra_option_btns_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_add_style(ui->sitting_scr_menu_1_cont_1, &style_sitting_scr_menu_1_extra_option_btns_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);

    //Write style state: LV_STATE_DEFAULT for &style_sitting_scr_menu_1_extra_main_title_main_default
    static lv_style_t style_sitting_scr_menu_1_extra_main_title_main_default;
    ui_init_style(&style_sitting_scr_menu_1_extra_main_title_main_default);

    lv_style_set_text_color(&style_sitting_scr_menu_1_extra_main_title_main_default, lv_color_hex(0x41485a));
    lv_style_set_text_font(&style_sitting_scr_menu_1_extra_main_title_main_default, &lv_font_YouSheYuFeiTeJianKangTi_2_20);
    lv_style_set_text_opa(&style_sitting_scr_menu_1_extra_main_title_main_default, 255);
    lv_style_set_text_align(&style_sitting_scr_menu_1_extra_main_title_main_default, LV_TEXT_ALIGN_CENTER);
    lv_style_set_bg_opa(&style_sitting_scr_menu_1_extra_main_title_main_default, 0);
    lv_style_set_pad_hor(&style_sitting_scr_menu_1_extra_main_title_main_default, 5);
    lv_style_set_pad_ver(&style_sitting_scr_menu_1_extra_main_title_main_default, 5);
    lv_obj_t * sitting_scr_menu_1_title = lv_menu_get_sidebar_header(ui->sitting_scr_menu_1);
    lv_obj_set_size(sitting_scr_menu_1_title, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_add_style(lv_menu_get_sidebar_header(ui->sitting_scr_menu_1), &style_sitting_scr_menu_1_extra_main_title_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);



    //Write codes sitting_scr_cont_1
    ui->sitting_scr_cont_1 = lv_obj_create(ui->sitting_scr_menu_1_subpage_1_cont);
    lv_obj_set_pos(ui->sitting_scr_cont_1, 3, 13);
    lv_obj_set_size(ui->sitting_scr_cont_1, 211, 218);
    lv_obj_set_scrollbar_mode(ui->sitting_scr_cont_1, LV_SCROLLBAR_MODE_OFF);

    //Write style for sitting_scr_cont_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->sitting_scr_cont_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->sitting_scr_cont_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->sitting_scr_cont_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->sitting_scr_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->sitting_scr_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->sitting_scr_cont_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->sitting_scr_cont_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->sitting_scr_cont_1, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->sitting_scr_cont_1, LV_BORDER_SIDE_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->sitting_scr_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->sitting_scr_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->sitting_scr_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->sitting_scr_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes sitting_scr_spangroup_2
    ui->sitting_scr_spangroup_2 = lv_spangroup_create(ui->sitting_scr_cont_1);
    lv_obj_set_pos(ui->sitting_scr_spangroup_2, 20, 17);
    lv_obj_set_size(ui->sitting_scr_spangroup_2, 74, 18);
    lv_obj_add_flag(ui->sitting_scr_spangroup_2, LV_OBJ_FLAG_HIDDEN);
    lv_spangroup_set_align(ui->sitting_scr_spangroup_2, LV_TEXT_ALIGN_LEFT);
    lv_spangroup_set_overflow(ui->sitting_scr_spangroup_2, LV_SPAN_OVERFLOW_CLIP);
    lv_spangroup_set_mode(ui->sitting_scr_spangroup_2, LV_SPAN_MODE_BREAK);
    //create span
    ui->sitting_scr_spangroup_2_span = lv_spangroup_new_span(ui->sitting_scr_spangroup_2);
    lv_span_set_text(ui->sitting_scr_spangroup_2_span, "网络连接中");
    lv_style_set_text_color(lv_span_get_style(ui->sitting_scr_spangroup_2_span), lv_color_hex(0x000000));
    lv_style_set_text_decor(lv_span_get_style(ui->sitting_scr_spangroup_2_span), LV_TEXT_DECOR_NONE);
    lv_style_set_text_font(lv_span_get_style(ui->sitting_scr_spangroup_2_span), &lv_font_YouSheYuFeiTeJianKangTi_2_16);

    //Write style state: LV_STATE_DEFAULT for &style_sitting_scr_spangroup_2_main_main_default
    static lv_style_t style_sitting_scr_spangroup_2_main_main_default;
    ui_init_style(&style_sitting_scr_spangroup_2_main_main_default);

    lv_style_set_border_width(&style_sitting_scr_spangroup_2_main_main_default, 0);
    lv_style_set_radius(&style_sitting_scr_spangroup_2_main_main_default, 0);
    lv_style_set_bg_opa(&style_sitting_scr_spangroup_2_main_main_default, 0);
    lv_style_set_pad_top(&style_sitting_scr_spangroup_2_main_main_default, 0);
    lv_style_set_pad_right(&style_sitting_scr_spangroup_2_main_main_default, 0);
    lv_style_set_pad_bottom(&style_sitting_scr_spangroup_2_main_main_default, 0);
    lv_style_set_pad_left(&style_sitting_scr_spangroup_2_main_main_default, 0);
    lv_style_set_shadow_width(&style_sitting_scr_spangroup_2_main_main_default, 0);
    lv_obj_add_style(ui->sitting_scr_spangroup_2, &style_sitting_scr_spangroup_2_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_spangroup_refr_mode(ui->sitting_scr_spangroup_2);

    //Write codes sitting_scr_spinner_1
    ui->sitting_scr_spinner_1 = lv_spinner_create(ui->sitting_scr_cont_1);
    lv_obj_set_pos(ui->sitting_scr_spinner_1, 101, 12);
    lv_obj_set_size(ui->sitting_scr_spinner_1, 30, 25);
    lv_obj_add_flag(ui->sitting_scr_spinner_1, LV_OBJ_FLAG_HIDDEN);
    lv_spinner_set_anim_params(ui->sitting_scr_spinner_1, 1500, 150);

    //Write style for sitting_scr_spinner_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_pad_top(ui->sitting_scr_spinner_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->sitting_scr_spinner_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->sitting_scr_spinner_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->sitting_scr_spinner_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->sitting_scr_spinner_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui->sitting_scr_spinner_1, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui->sitting_scr_spinner_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui->sitting_scr_spinner_1, lv_color_hex(0xd5d6de), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(ui->sitting_scr_spinner_1, true, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->sitting_scr_spinner_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for sitting_scr_spinner_1, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_arc_width(ui->sitting_scr_spinner_1, 5, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui->sitting_scr_spinner_1, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui->sitting_scr_spinner_1, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(ui->sitting_scr_spinner_1, true, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write codes sitting_scr_spangroup_1
    ui->sitting_scr_spangroup_1 = lv_spangroup_create(ui->sitting_scr_cont_1);
    lv_obj_set_pos(ui->sitting_scr_spangroup_1, 49, 60);
    lv_obj_set_size(ui->sitting_scr_spangroup_1, 86, 18);
    lv_obj_add_flag(ui->sitting_scr_spangroup_1, LV_OBJ_FLAG_HIDDEN);
    lv_spangroup_set_align(ui->sitting_scr_spangroup_1, LV_TEXT_ALIGN_LEFT);
    lv_spangroup_set_overflow(ui->sitting_scr_spangroup_1, LV_SPAN_OVERFLOW_CLIP);
    lv_spangroup_set_mode(ui->sitting_scr_spangroup_1, LV_SPAN_MODE_BREAK);
    //create span
    ui->sitting_scr_spangroup_1_span = lv_spangroup_new_span(ui->sitting_scr_spangroup_1);
    lv_span_set_text(ui->sitting_scr_spangroup_1_span, "wifi连接成功");
    lv_style_set_text_color(lv_span_get_style(ui->sitting_scr_spangroup_1_span), lv_color_hex(0x000000));
    lv_style_set_text_decor(lv_span_get_style(ui->sitting_scr_spangroup_1_span), LV_TEXT_DECOR_NONE);
    lv_style_set_text_font(lv_span_get_style(ui->sitting_scr_spangroup_1_span), &lv_font_YouSheYuFeiTeJianKangTi_2_16);

    //Write style state: LV_STATE_DEFAULT for &style_sitting_scr_spangroup_1_main_main_default
    static lv_style_t style_sitting_scr_spangroup_1_main_main_default;
    ui_init_style(&style_sitting_scr_spangroup_1_main_main_default);

    lv_style_set_border_width(&style_sitting_scr_spangroup_1_main_main_default, 0);
    lv_style_set_radius(&style_sitting_scr_spangroup_1_main_main_default, 0);
    lv_style_set_bg_opa(&style_sitting_scr_spangroup_1_main_main_default, 0);
    lv_style_set_pad_top(&style_sitting_scr_spangroup_1_main_main_default, 0);
    lv_style_set_pad_right(&style_sitting_scr_spangroup_1_main_main_default, 0);
    lv_style_set_pad_bottom(&style_sitting_scr_spangroup_1_main_main_default, 0);
    lv_style_set_pad_left(&style_sitting_scr_spangroup_1_main_main_default, 0);
    lv_style_set_shadow_width(&style_sitting_scr_spangroup_1_main_main_default, 0);
    lv_obj_add_style(ui->sitting_scr_spangroup_1, &style_sitting_scr_spangroup_1_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_spangroup_refr_mode(ui->sitting_scr_spangroup_1);

    //Write codes sitting_scr_qrcode_1
    ui->sitting_scr_qrcode_1 = lv_qrcode_create(ui->sitting_scr_cont_1);
    lv_obj_set_pos(ui->sitting_scr_qrcode_1, 24, 87);
    lv_obj_set_size(ui->sitting_scr_qrcode_1, 133, 133);
    lv_qrcode_set_size(ui->sitting_scr_qrcode_1, 133);
    lv_qrcode_set_dark_color(ui->sitting_scr_qrcode_1, lv_color_hex(0x2C3224));
    lv_qrcode_set_light_color(ui->sitting_scr_qrcode_1, lv_color_hex(0xffffff));
    const char * sitting_scr_qrcode_1_data = "https://www.nxp.com/";
    lv_qrcode_update(ui->sitting_scr_qrcode_1, sitting_scr_qrcode_1_data, 20);



    //Write codes sitting_scr_cb_1
    ui->sitting_scr_cb_1 = lv_checkbox_create(ui->sitting_scr_menu_1_subpage_2_cont);
    lv_obj_set_pos(ui->sitting_scr_cb_1, 16, 23);
    lv_checkbox_set_text(ui->sitting_scr_cb_1, "checkbox");

    //Write style for sitting_scr_cb_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_pad_top(ui->sitting_scr_cb_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->sitting_scr_cb_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->sitting_scr_cb_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->sitting_scr_cb_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->sitting_scr_cb_1, lv_color_hex(0x0D3055), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->sitting_scr_cb_1, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->sitting_scr_cb_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->sitting_scr_cb_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->sitting_scr_cb_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->sitting_scr_cb_1, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->sitting_scr_cb_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->sitting_scr_cb_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->sitting_scr_cb_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->sitting_scr_cb_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for sitting_scr_cb_1, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_pad_all(ui->sitting_scr_cb_1, 3, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->sitting_scr_cb_1, 2, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->sitting_scr_cb_1, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->sitting_scr_cb_1, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->sitting_scr_cb_1, LV_BORDER_SIDE_FULL, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->sitting_scr_cb_1, 6, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->sitting_scr_cb_1, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->sitting_scr_cb_1, lv_color_hex(0xffffff), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->sitting_scr_cb_1, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write codes sitting_scr_cb_2
    ui->sitting_scr_cb_2 = lv_checkbox_create(ui->sitting_scr_menu_1_subpage_2_cont);
    lv_obj_set_pos(ui->sitting_scr_cb_2, 16, 69);
    lv_checkbox_set_text(ui->sitting_scr_cb_2, "checkbox");

    //Write style for sitting_scr_cb_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_pad_top(ui->sitting_scr_cb_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->sitting_scr_cb_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->sitting_scr_cb_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->sitting_scr_cb_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->sitting_scr_cb_2, lv_color_hex(0x0D3055), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->sitting_scr_cb_2, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->sitting_scr_cb_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->sitting_scr_cb_2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->sitting_scr_cb_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->sitting_scr_cb_2, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->sitting_scr_cb_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->sitting_scr_cb_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->sitting_scr_cb_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->sitting_scr_cb_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for sitting_scr_cb_2, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_pad_all(ui->sitting_scr_cb_2, 3, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->sitting_scr_cb_2, 2, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->sitting_scr_cb_2, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->sitting_scr_cb_2, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->sitting_scr_cb_2, LV_BORDER_SIDE_FULL, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->sitting_scr_cb_2, 6, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->sitting_scr_cb_2, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->sitting_scr_cb_2, lv_color_hex(0xffffff), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->sitting_scr_cb_2, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write codes sitting_scr_cb_3
    ui->sitting_scr_cb_3 = lv_checkbox_create(ui->sitting_scr_menu_1_subpage_2_cont);
    lv_obj_set_pos(ui->sitting_scr_cb_3, 16, 161);
    lv_checkbox_set_text(ui->sitting_scr_cb_3, "checkbox");

    //Write style for sitting_scr_cb_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_pad_top(ui->sitting_scr_cb_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->sitting_scr_cb_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->sitting_scr_cb_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->sitting_scr_cb_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->sitting_scr_cb_3, lv_color_hex(0x0D3055), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->sitting_scr_cb_3, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->sitting_scr_cb_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->sitting_scr_cb_3, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->sitting_scr_cb_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->sitting_scr_cb_3, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->sitting_scr_cb_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->sitting_scr_cb_3, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->sitting_scr_cb_3, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->sitting_scr_cb_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for sitting_scr_cb_3, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_pad_all(ui->sitting_scr_cb_3, 3, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->sitting_scr_cb_3, 2, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->sitting_scr_cb_3, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->sitting_scr_cb_3, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->sitting_scr_cb_3, LV_BORDER_SIDE_FULL, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->sitting_scr_cb_3, 6, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->sitting_scr_cb_3, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->sitting_scr_cb_3, lv_color_hex(0xffffff), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->sitting_scr_cb_3, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write codes sitting_scr_cb_4
    ui->sitting_scr_cb_4 = lv_checkbox_create(ui->sitting_scr_menu_1_subpage_2_cont);
    lv_obj_set_pos(ui->sitting_scr_cb_4, 16, 115);
    lv_checkbox_set_text(ui->sitting_scr_cb_4, "checkbox");

    //Write style for sitting_scr_cb_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_pad_top(ui->sitting_scr_cb_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->sitting_scr_cb_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->sitting_scr_cb_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->sitting_scr_cb_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->sitting_scr_cb_4, lv_color_hex(0x0D3055), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->sitting_scr_cb_4, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->sitting_scr_cb_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->sitting_scr_cb_4, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->sitting_scr_cb_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->sitting_scr_cb_4, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->sitting_scr_cb_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->sitting_scr_cb_4, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->sitting_scr_cb_4, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->sitting_scr_cb_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for sitting_scr_cb_4, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_pad_all(ui->sitting_scr_cb_4, 3, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->sitting_scr_cb_4, 2, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->sitting_scr_cb_4, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->sitting_scr_cb_4, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->sitting_scr_cb_4, LV_BORDER_SIDE_FULL, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->sitting_scr_cb_4, 6, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->sitting_scr_cb_4, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->sitting_scr_cb_4, lv_color_hex(0xffffff), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->sitting_scr_cb_4, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write codes sitting_scr_cb_5
    ui->sitting_scr_cb_5 = lv_checkbox_create(ui->sitting_scr_menu_1_subpage_2_cont);
    lv_obj_set_pos(ui->sitting_scr_cb_5, 16, 207);
    lv_checkbox_set_text(ui->sitting_scr_cb_5, "checkbox");

    //Write style for sitting_scr_cb_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_pad_top(ui->sitting_scr_cb_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->sitting_scr_cb_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->sitting_scr_cb_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->sitting_scr_cb_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->sitting_scr_cb_5, lv_color_hex(0x0D3055), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->sitting_scr_cb_5, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->sitting_scr_cb_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->sitting_scr_cb_5, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->sitting_scr_cb_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->sitting_scr_cb_5, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->sitting_scr_cb_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->sitting_scr_cb_5, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->sitting_scr_cb_5, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->sitting_scr_cb_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for sitting_scr_cb_5, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_pad_all(ui->sitting_scr_cb_5, 3, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->sitting_scr_cb_5, 2, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->sitting_scr_cb_5, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->sitting_scr_cb_5, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->sitting_scr_cb_5, LV_BORDER_SIDE_FULL, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->sitting_scr_cb_5, 6, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->sitting_scr_cb_5, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->sitting_scr_cb_5, lv_color_hex(0xffffff), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->sitting_scr_cb_5, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);



    //Write codes sitting_scr_back_1
    ui->sitting_scr_back_1 = lv_button_create(ui->sitting_scr);
    lv_obj_set_pos(ui->sitting_scr_back_1, 252, 9);
    lv_obj_set_size(ui->sitting_scr_back_1, 53, 40);
    ui->sitting_scr_back_1_label = lv_label_create(ui->sitting_scr_back_1);
    lv_label_set_text(ui->sitting_scr_back_1_label, "返回");
    lv_label_set_long_mode(ui->sitting_scr_back_1_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->sitting_scr_back_1_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->sitting_scr_back_1, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->sitting_scr_back_1_label, LV_PCT(100));

    //Write style for sitting_scr_back_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->sitting_scr_back_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->sitting_scr_back_1, lv_color_hex(0x419ff6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->sitting_scr_back_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->sitting_scr_back_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->sitting_scr_back_1, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->sitting_scr_back_1, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->sitting_scr_back_1, lv_color_hex(0x0d4b3b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->sitting_scr_back_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->sitting_scr_back_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_offset_x(ui->sitting_scr_back_1, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_offset_y(ui->sitting_scr_back_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->sitting_scr_back_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->sitting_scr_back_1, &lv_font_HYPixel11pxU_2_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->sitting_scr_back_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->sitting_scr_back_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of sitting_scr.


    //Update current screen layout.
    lv_obj_update_layout(ui->sitting_scr);

    //Init events for screen.
    events_init_sitting_scr(ui);
}
