/*
* Copyright 2026 NXP
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



void setup_scr_more_scr(lv_ui *ui)
{
    //Write codes more_scr
    ui->more_scr = lv_obj_create(NULL);
    lv_obj_set_size(ui->more_scr, 320, 240);
    lv_obj_set_scrollbar_mode(ui->more_scr, LV_SCROLLBAR_MODE_OFF);
    lv_obj_remove_flag(ui->more_scr, LV_OBJ_FLAG_SCROLLABLE);

    //Write style for more_scr, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->more_scr, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes more_scr_more_ss
    ui->more_scr_more_ss = lv_tabview_create(ui->more_scr);
    lv_obj_set_pos(ui->more_scr_more_ss, 0, 0);
    lv_obj_set_size(ui->more_scr_more_ss, 320, 240);
    lv_obj_set_scrollbar_mode(ui->more_scr_more_ss, LV_SCROLLBAR_MODE_AUTO);
    lv_obj_remove_flag(ui->more_scr_more_ss, LV_OBJ_FLAG_SCROLLABLE);
    lv_tabview_set_tab_bar_position(ui->more_scr_more_ss, LV_DIR_TOP);
    lv_tabview_set_tab_bar_size(ui->more_scr_more_ss, 40);
    ui->more_scr_more_ss_tab_1 = lv_tabview_add_tab(ui->more_scr_more_ss, "AI建议");
    ui->more_scr_more_ss_tab_2 = lv_tabview_add_tab(ui->more_scr_more_ss, "图表");
    ui->more_scr_more_ss_tab_3 = lv_tabview_add_tab(ui->more_scr_more_ss, "历史记录");

    //Write style for more_scr_more_ss, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->more_scr_more_ss, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->more_scr_more_ss, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->more_scr_more_ss, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->more_scr_more_ss, lv_color_hex(0x4d4d4d), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->more_scr_more_ss, &lv_font_YouSheYuFeiTeJianKangTi_2_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->more_scr_more_ss, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->more_scr_more_ss, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->more_scr_more_ss, 16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->more_scr_more_ss, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->more_scr_more_ss, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->more_scr_more_ss, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_more_scr_more_ss_extra_btnm_main_default
    static lv_style_t style_more_scr_more_ss_extra_btnm_main_default;
    ui_init_style(&style_more_scr_more_ss_extra_btnm_main_default);

    lv_style_set_bg_opa(&style_more_scr_more_ss_extra_btnm_main_default, 255);
    lv_style_set_bg_color(&style_more_scr_more_ss_extra_btnm_main_default, lv_color_hex(0x9bd4e8));
    lv_style_set_bg_grad_dir(&style_more_scr_more_ss_extra_btnm_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_border_width(&style_more_scr_more_ss_extra_btnm_main_default, 2);
    lv_style_set_border_opa(&style_more_scr_more_ss_extra_btnm_main_default, 255);
    lv_style_set_border_color(&style_more_scr_more_ss_extra_btnm_main_default, lv_color_hex(0x000000));
    lv_style_set_border_side(&style_more_scr_more_ss_extra_btnm_main_default, LV_BORDER_SIDE_BOTTOM);
    lv_style_set_radius(&style_more_scr_more_ss_extra_btnm_main_default, 0);
    for(uint32_t i = 0; i < lv_tabview_get_tab_count(ui->more_scr_more_ss); i++)
    {
        lv_obj_add_style(lv_obj_get_child(lv_tabview_get_tab_bar(ui->more_scr_more_ss), i), &style_more_scr_more_ss_extra_btnm_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    }

    //Write style state: LV_STATE_DEFAULT for &style_more_scr_more_ss_extra_btnm_items_default
    static lv_style_t style_more_scr_more_ss_extra_btnm_items_default;
    ui_init_style(&style_more_scr_more_ss_extra_btnm_items_default);

    lv_style_set_text_color(&style_more_scr_more_ss_extra_btnm_items_default, lv_color_hex(0x291f1f));
    lv_style_set_text_font(&style_more_scr_more_ss_extra_btnm_items_default, &lv_font_YouSheYuFeiTeJianKangTi_2_16);
    lv_style_set_text_opa(&style_more_scr_more_ss_extra_btnm_items_default, 255);
    for(uint32_t i = 0; i < lv_tabview_get_tab_count(ui->more_scr_more_ss); i++)
    {
        lv_obj_add_style(lv_obj_get_child(lv_tabview_get_tab_bar(ui->more_scr_more_ss), i), &style_more_scr_more_ss_extra_btnm_items_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    }

    //Write style state: LV_STATE_CHECKED for &style_more_scr_more_ss_extra_btnm_items_checked
    static lv_style_t style_more_scr_more_ss_extra_btnm_items_checked;
    ui_init_style(&style_more_scr_more_ss_extra_btnm_items_checked);

    lv_style_set_text_color(&style_more_scr_more_ss_extra_btnm_items_checked, lv_color_hex(0x007185));
    lv_style_set_text_font(&style_more_scr_more_ss_extra_btnm_items_checked, &lv_font_YouSheYuFeiTeJianKangTi_2_18);
    lv_style_set_text_opa(&style_more_scr_more_ss_extra_btnm_items_checked, 255);
    lv_style_set_border_width(&style_more_scr_more_ss_extra_btnm_items_checked, 2);
    lv_style_set_border_opa(&style_more_scr_more_ss_extra_btnm_items_checked, 255);
    lv_style_set_border_color(&style_more_scr_more_ss_extra_btnm_items_checked, lv_color_hex(0x2a646e));
    lv_style_set_border_side(&style_more_scr_more_ss_extra_btnm_items_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_more_scr_more_ss_extra_btnm_items_checked, 0);
    lv_style_set_bg_opa(&style_more_scr_more_ss_extra_btnm_items_checked, 241);
    lv_style_set_bg_color(&style_more_scr_more_ss_extra_btnm_items_checked, lv_color_hex(0xea90f1));
    lv_style_set_bg_grad_dir(&style_more_scr_more_ss_extra_btnm_items_checked, LV_GRAD_DIR_NONE);
    for(uint32_t i = 0; i < lv_tabview_get_tab_count(ui->more_scr_more_ss); i++)
    {
        lv_obj_add_style(lv_obj_get_child(lv_tabview_get_tab_bar(ui->more_scr_more_ss), i), &style_more_scr_more_ss_extra_btnm_items_checked, LV_PART_MAIN|LV_STATE_CHECKED);
    }

    //Write codes AI建议
    lv_obj_t * more_scr_more_ss_tab_1_label = lv_label_create(ui->more_scr_more_ss_tab_1);
    lv_label_set_text(more_scr_more_ss_tab_1_label, "");

    //Write codes 图表
    lv_obj_t * more_scr_more_ss_tab_2_label = lv_label_create(ui->more_scr_more_ss_tab_2);
    lv_label_set_text(more_scr_more_ss_tab_2_label, "con2");

    //Write codes more_scr_tab_2
    ui->more_scr_tab_2 = lv_obj_create(ui->more_scr_more_ss_tab_2);
    lv_obj_set_pos(ui->more_scr_tab_2, -15, -21);
    lv_obj_set_size(ui->more_scr_tab_2, 274, 200);
    lv_obj_set_scrollbar_mode(ui->more_scr_tab_2, LV_SCROLLBAR_MODE_OFF);

    //Write style for more_scr_tab_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->more_scr_tab_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->more_scr_tab_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->more_scr_tab_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->more_scr_tab_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->more_scr_tab_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->more_scr_tab_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->more_scr_tab_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->more_scr_tab_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->more_scr_tab_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->more_scr_tab_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes more_scr_chart_1
    ui->more_scr_chart_1 = lv_chart_create(ui->more_scr_tab_2);
    lv_obj_set_pos(ui->more_scr_chart_1, 16, 4);
    lv_obj_set_size(ui->more_scr_chart_1, 244, 168);
    lv_obj_set_scrollbar_mode(ui->more_scr_chart_1, LV_SCROLLBAR_MODE_OFF);
    lv_chart_set_type(ui->more_scr_chart_1, LV_CHART_TYPE_BAR);
    lv_chart_set_div_line_count(ui->more_scr_chart_1, 5, 5);
    lv_chart_set_point_count(ui->more_scr_chart_1, 3);
    lv_chart_set_range(ui->more_scr_chart_1, LV_CHART_AXIS_PRIMARY_Y, 0, 100);
    lv_chart_set_range(ui->more_scr_chart_1, LV_CHART_AXIS_SECONDARY_Y, 0, 100);
    ui->more_scr_chart_1_0 = lv_chart_add_series(ui->more_scr_chart_1, lv_color_hex(0xff8960), LV_CHART_AXIS_PRIMARY_Y);
#if LV_USE_FREEMASTER == 0
    lv_chart_set_next_value(ui->more_scr_chart_1, ui->more_scr_chart_1_0, 50);
    lv_chart_set_next_value(ui->more_scr_chart_1, ui->more_scr_chart_1_0, 25);
    lv_chart_set_next_value(ui->more_scr_chart_1, ui->more_scr_chart_1_0, 30);
#endif

    //Write style for more_scr_chart_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->more_scr_chart_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->more_scr_chart_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->more_scr_chart_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->more_scr_chart_1, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->more_scr_chart_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->more_scr_chart_1, lv_color_hex(0xe8e8e8), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->more_scr_chart_1, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->more_scr_chart_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_width(ui->more_scr_chart_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui->more_scr_chart_1, lv_color_hex(0xe8e8e8), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->more_scr_chart_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->more_scr_chart_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes more_scr_label_1
    ui->more_scr_label_1 = lv_label_create(ui->more_scr_tab_2);
    lv_obj_set_pos(ui->more_scr_label_1, 24, 174);
    lv_obj_set_size(ui->more_scr_label_1, 41, 22);
    lv_label_set_text(ui->more_scr_label_1, "氮");
    lv_label_set_long_mode(ui->more_scr_label_1, LV_LABEL_LONG_WRAP);

    //Write style for more_scr_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->more_scr_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->more_scr_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->more_scr_label_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->more_scr_label_1, &lv_font_YouSheYuFeiTeJianKangTi_2_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->more_scr_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->more_scr_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->more_scr_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->more_scr_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->more_scr_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->more_scr_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->more_scr_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->more_scr_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->more_scr_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->more_scr_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes more_scr_label_2
    ui->more_scr_label_2 = lv_label_create(ui->more_scr_tab_2);
    lv_obj_set_pos(ui->more_scr_label_2, 174, 174);
    lv_obj_set_size(ui->more_scr_label_2, 37, 24);
    lv_label_set_text(ui->more_scr_label_2, "钾");
    lv_label_set_long_mode(ui->more_scr_label_2, LV_LABEL_LONG_WRAP);

    //Write style for more_scr_label_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->more_scr_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->more_scr_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->more_scr_label_2, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->more_scr_label_2, &lv_font_YouSheYuFeiTeJianKangTi_2_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->more_scr_label_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->more_scr_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->more_scr_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->more_scr_label_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->more_scr_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->more_scr_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->more_scr_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->more_scr_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->more_scr_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->more_scr_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes more_scr_label_3
    ui->more_scr_label_3 = lv_label_create(ui->more_scr_tab_2);
    lv_obj_set_pos(ui->more_scr_label_3, 99, 174);
    lv_obj_set_size(ui->more_scr_label_3, 38, 24);
    lv_label_set_text(ui->more_scr_label_3, "磷");
    lv_label_set_long_mode(ui->more_scr_label_3, LV_LABEL_LONG_WRAP);

    //Write style for more_scr_label_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->more_scr_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->more_scr_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->more_scr_label_3, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->more_scr_label_3, &lv_font_YouSheYuFeiTeJianKangTi_2_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->more_scr_label_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->more_scr_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->more_scr_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->more_scr_label_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->more_scr_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->more_scr_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->more_scr_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->more_scr_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->more_scr_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->more_scr_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes 历史记录
    lv_obj_t * more_scr_more_ss_tab_3_label = lv_label_create(ui->more_scr_more_ss_tab_3);
    lv_label_set_text(more_scr_more_ss_tab_3_label, "2026/3/23:\n光照低");

    //Write codes more_scr_btn_2
    ui->more_scr_btn_2 = lv_button_create(ui->more_scr);
    lv_obj_set_pos(ui->more_scr_btn_2, 246, 186);
    lv_obj_set_size(ui->more_scr_btn_2, 53, 40);
    ui->more_scr_btn_2_label = lv_label_create(ui->more_scr_btn_2);
    lv_label_set_text(ui->more_scr_btn_2_label, "返回");
    lv_label_set_long_mode(ui->more_scr_btn_2_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->more_scr_btn_2_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->more_scr_btn_2, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->more_scr_btn_2_label, LV_PCT(100));

    //Write style for more_scr_btn_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->more_scr_btn_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->more_scr_btn_2, lv_color_hex(0x419ff6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->more_scr_btn_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->more_scr_btn_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->more_scr_btn_2, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->more_scr_btn_2, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->more_scr_btn_2, lv_color_hex(0x0d4b3b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->more_scr_btn_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->more_scr_btn_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_offset_x(ui->more_scr_btn_2, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_offset_y(ui->more_scr_btn_2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->more_scr_btn_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->more_scr_btn_2, &lv_font_HYPixel11pxU_2_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->more_scr_btn_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->more_scr_btn_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of more_scr.


    //Update current screen layout.
    lv_obj_update_layout(ui->more_scr);

    //Init events for screen.
    events_init_more_scr(ui);
}
