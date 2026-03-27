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



void setup_scr_screen(lv_ui *ui)
{
    //Write codes screen
    ui->screen = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen, 320, 240);
    lv_obj_set_scrollbar_mode(ui->screen, LV_SCROLLBAR_MODE_OFF);
    lv_obj_remove_flag(ui->screen, LV_OBJ_FLAG_SCROLLABLE);

    //Write style for screen, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_jia_bar
    ui->screen_jia_bar = lv_bar_create(ui->screen);
    lv_obj_set_pos(ui->screen_jia_bar, 16, 116);
    lv_obj_set_size(ui->screen_jia_bar, 90, 20);
    lv_obj_set_style_anim_duration(ui->screen_jia_bar, 400, 0);
    lv_bar_set_mode(ui->screen_jia_bar, LV_BAR_MODE_NORMAL);
    lv_bar_set_range(ui->screen_jia_bar, 0, 600);
    lv_bar_set_value(ui->screen_jia_bar, 100, LV_ANIM_OFF);

    //Write style for screen_jia_bar, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_jia_bar, 60, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_jia_bar, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_jia_bar, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_jia_bar, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_jia_bar, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_jia_bar, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_jia_bar, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_jia_bar, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_jia_bar, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_jia_bar, 10, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write codes screen_ling_bar
    ui->screen_ling_bar = lv_bar_create(ui->screen);
    lv_obj_set_pos(ui->screen_ling_bar, 16, 80);
    lv_obj_set_size(ui->screen_ling_bar, 71, 20);
    lv_obj_set_style_anim_duration(ui->screen_ling_bar, 400, 0);
    lv_bar_set_mode(ui->screen_ling_bar, LV_BAR_MODE_NORMAL);
    lv_bar_set_range(ui->screen_ling_bar, 0, 100);
    lv_bar_set_value(ui->screen_ling_bar, 20, LV_ANIM_OFF);

    //Write style for screen_ling_bar, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_ling_bar, 60, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_ling_bar, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_ling_bar, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_ling_bar, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_ling_bar, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_ling_bar, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_ling_bar, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_ling_bar, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_ling_bar, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_ling_bar, 10, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write codes screen_sty_fire
    ui->screen_sty_fire = lv_image_create(ui->screen);
    lv_obj_set_pos(ui->screen_sty_fire, -52, 7);
    lv_obj_set_size(ui->screen_sty_fire, 49, 47);
    lv_obj_add_flag(ui->screen_sty_fire, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_sty_fire, &_772804_RGB565A8_49x47);
    lv_image_set_pivot(ui->screen_sty_fire, 50,50);
    lv_image_set_rotation(ui->screen_sty_fire, 900);

    //Write style for screen_sty_fire, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_sty_fire, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_sty_fire, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_cont_2
    ui->screen_cont_2 = lv_obj_create(ui->screen);
    lv_obj_set_pos(ui->screen_cont_2, 213, 54);
    lv_obj_set_size(ui->screen_cont_2, 103, 184);
    lv_obj_set_scrollbar_mode(ui->screen_cont_2, LV_SCROLLBAR_MODE_ACTIVE);
    lv_obj_remove_flag(ui->screen_cont_2, LV_OBJ_FLAG_SCROLLABLE);

    //Write style for screen_cont_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_cont_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_cont_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_cont_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_img_1
    ui->screen_img_1 = lv_image_create(ui->screen_cont_2);
    lv_obj_set_pos(ui->screen_img_1, 7, 118);
    lv_obj_set_size(ui->screen_img_1, 100, 100);
    lv_obj_add_flag(ui->screen_img_1, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_img_1, &_10_RGB565A8_100x100);
    lv_image_set_pivot(ui->screen_img_1, 50,50);
    lv_image_set_rotation(ui->screen_img_1, 0);

    //Write style for screen_img_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_img_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_img_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_img_2
    ui->screen_img_2 = lv_image_create(ui->screen_cont_2);
    lv_obj_set_pos(ui->screen_img_2, 15, 86);
    lv_obj_set_size(ui->screen_img_2, 78, 73);
    lv_obj_add_flag(ui->screen_img_2, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_img_2, &_34_RGB565A8_78x73);
    lv_image_set_pivot(ui->screen_img_2, 50,50);
    lv_image_set_rotation(ui->screen_img_2, 0);

    //Write style for screen_img_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_img_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_img_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_img_3
    ui->screen_img_3 = lv_image_create(ui->screen_cont_2);
    lv_obj_set_pos(ui->screen_img_3, -2, 3);
    lv_obj_set_size(ui->screen_img_3, 110, 105);
    lv_obj_add_flag(ui->screen_img_3, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_img_3, &_324754_RGB565A8_110x105);
    lv_image_set_pivot(ui->screen_img_3, 50,50);
    lv_image_set_rotation(ui->screen_img_3, 0);

    //Write style for screen_img_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_img_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_img_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_img_4
    ui->screen_img_4 = lv_image_create(ui->screen_cont_2);
    lv_obj_set_pos(ui->screen_img_4, 0, 8);
    lv_obj_set_size(ui->screen_img_4, 105, 100);
    lv_obj_add_flag(ui->screen_img_4, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui->screen_img_4, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_img_4, &_275342_RGB565A8_105x100);
    lv_image_set_pivot(ui->screen_img_4, 50,50);
    lv_image_set_rotation(ui->screen_img_4, 0);

    //Write style for screen_img_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_img_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_img_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_water_temp
    ui->screen_water_temp = lv_label_create(ui->screen);
    lv_obj_set_pos(ui->screen_water_temp, 245, 30);
    lv_obj_set_size(ui->screen_water_temp, 84, 18);
    lv_label_set_text(ui->screen_water_temp, "湿度:40%");
    lv_label_set_long_mode(ui->screen_water_temp, LV_LABEL_LONG_SCROLL_CIRCULAR);

    //Write style for screen_water_temp, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_water_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_water_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_water_temp, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_water_temp, &lv_font_YouSheYuFeiTeJianKangTi_2_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_water_temp, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_water_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_water_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_water_temp, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_water_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_water_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_water_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_water_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_water_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_water_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_ph_temp
    ui->screen_ph_temp = lv_label_create(ui->screen);
    lv_obj_set_pos(ui->screen_ph_temp, 256, 54);
    lv_obj_set_size(ui->screen_ph_temp, 58, 23);
    lv_label_set_text(ui->screen_ph_temp, "PH值:6");
    lv_label_set_long_mode(ui->screen_ph_temp, LV_LABEL_LONG_WRAP);

    //Write style for screen_ph_temp, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_ph_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_ph_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_ph_temp, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_ph_temp, &lv_font_YouSheYuFeiTeJianKangTi_2_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_ph_temp, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_ph_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_ph_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_ph_temp, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_ph_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_ph_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_ph_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_ph_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_ph_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_ph_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_ling
    ui->screen_ling = lv_label_create(ui->screen);
    lv_obj_set_pos(ui->screen_ling, 73, 80);
    lv_obj_set_size(ui->screen_ling, 50, 18);
    lv_label_set_text(ui->screen_ling, "磷");
    lv_label_set_long_mode(ui->screen_ling, LV_LABEL_LONG_WRAP);

    //Write style for screen_ling, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_ling, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_ling, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_ling, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_ling, &lv_font_YouSheYuFeiTeJianKangTi_2_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_ling, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_ling, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_ling, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_ling, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_ling, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_ling, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_ling, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_ling, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_ling, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_ling, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_dan
    ui->screen_dan = lv_label_create(ui->screen);
    lv_obj_set_pos(ui->screen_dan, 48, 48);
    lv_obj_set_size(ui->screen_dan, 50, 18);
    lv_label_set_text(ui->screen_dan, "氮");
    lv_label_set_long_mode(ui->screen_dan, LV_LABEL_LONG_WRAP);

    //Write style for screen_dan, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_dan, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_dan, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_dan, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_dan, &lv_font_YouSheYuFeiTeJianKangTi_2_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_dan, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_dan, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_dan, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_dan, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_dan, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_dan, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_dan, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_dan, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_dan, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_dan, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_jia
    ui->screen_jia = lv_label_create(ui->screen);
    lv_obj_set_pos(ui->screen_jia, 92, 116);
    lv_obj_set_size(ui->screen_jia, 50, 17);
    lv_label_set_text(ui->screen_jia, "钾");
    lv_label_set_long_mode(ui->screen_jia, LV_LABEL_LONG_WRAP);

    //Write style for screen_jia, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_jia, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_jia, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_jia, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_jia, &lv_font_YouSheYuFeiTeJianKangTi_2_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_jia, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_jia, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_jia, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_jia, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_jia, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_jia, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_jia, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_jia, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_jia, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_jia, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_bar_1
    ui->screen_bar_1 = lv_bar_create(ui->screen);
    lv_obj_set_pos(ui->screen_bar_1, 16, 48);
    lv_obj_set_size(ui->screen_bar_1, 47, 20);
    lv_obj_set_style_anim_duration(ui->screen_bar_1, 400, 0);
    lv_bar_set_mode(ui->screen_bar_1, LV_BAR_MODE_NORMAL);
    lv_bar_set_range(ui->screen_bar_1, 0, 200);
    lv_bar_set_value(ui->screen_bar_1, 50, LV_ANIM_OFF);

    //Write style for screen_bar_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_bar_1, 60, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_bar_1, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_bar_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_bar_1, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_bar_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_bar_1, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_bar_1, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_bar_1, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_bar_1, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_bar_1, 10, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write codes screen_cont_1
    ui->screen_cont_1 = lv_obj_create(ui->screen);
    lv_obj_set_pos(ui->screen_cont_1, 0, 0);
    lv_obj_set_size(ui->screen_cont_1, 320, 25);
    lv_obj_set_scrollbar_mode(ui->screen_cont_1, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_cont_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_cont_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_cont_1, lv_color_hex(0x9e9e9e), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_cont_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_label_1
    ui->screen_label_1 = lv_label_create(ui->screen_cont_1);
    lv_obj_set_pos(ui->screen_label_1, 244, 5);
    lv_obj_set_size(ui->screen_label_1, 65, 17);
    lv_label_set_text(ui->screen_label_1, "光照:适宜");
    lv_label_set_long_mode(ui->screen_label_1, LV_LABEL_LONG_WRAP);

    //Write style for screen_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_label_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_label_1, &lv_font_YouSheYuFeiTeJianKangTi_2_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_label_1, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_label_2
    ui->screen_label_2 = lv_label_create(ui->screen_cont_1);
    lv_obj_set_pos(ui->screen_label_2, 10, 4);
    lv_obj_set_size(ui->screen_label_2, 91, 17);
    lv_label_set_text(ui->screen_label_2, "温度:26.5度");
    lv_label_set_long_mode(ui->screen_label_2, LV_LABEL_LONG_WRAP);

    //Write style for screen_label_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_label_2, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_label_2, &lv_font_YouSheYuFeiTeJianKangTi_2_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_label_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_label_2, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_sitting
    ui->screen_sitting = lv_button_create(ui->screen);
    lv_obj_set_pos(ui->screen_sitting, 16, 177);
    lv_obj_set_size(ui->screen_sitting, 76, 43);
    ui->screen_sitting_label = lv_label_create(ui->screen_sitting);
    lv_label_set_text(ui->screen_sitting_label, "设置");
    lv_label_set_long_mode(ui->screen_sitting_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_sitting_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_sitting, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_sitting_label, LV_PCT(100));

    //Write style for screen_sitting, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_sitting, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_sitting, lv_color_hex(0x419ff6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_sitting, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_sitting, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_sitting, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_sitting, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->screen_sitting, lv_color_hex(0x0d4b3b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->screen_sitting, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->screen_sitting, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_offset_x(ui->screen_sitting, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_offset_y(ui->screen_sitting, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_sitting, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_sitting, &lv_font_YouSheYuFeiTeJianKangTi_2_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_sitting, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_sitting, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_more
    ui->screen_more = lv_button_create(ui->screen);
    lv_obj_set_pos(ui->screen_more, 123, 177);
    lv_obj_set_size(ui->screen_more, 76, 43);
    ui->screen_more_label = lv_label_create(ui->screen_more);
    lv_label_set_text(ui->screen_more_label, "详细");
    lv_label_set_long_mode(ui->screen_more_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_more_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_more, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_more_label, LV_PCT(100));

    //Write style for screen_more, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_more, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_more, lv_color_hex(0x419ff6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_more, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_more, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_more, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_more, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->screen_more, lv_color_hex(0x0d4b3b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->screen_more, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->screen_more, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_offset_x(ui->screen_more, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_offset_y(ui->screen_more, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_more, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_more, &lv_font_YouSheYuFeiTeJianKangTi_2_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_more, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_more, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_warring
    ui->screen_warring = lv_image_create(ui->screen);
    lv_obj_set_pos(ui->screen_warring, 126, 116);
    lv_obj_set_size(ui->screen_warring, 20, 20);
    lv_obj_add_flag(ui->screen_warring, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui->screen_warring, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_warring, &_waring_RGB565A8_20x20);
    lv_image_set_pivot(ui->screen_warring, 50,50);
    lv_image_set_rotation(ui->screen_warring, 0);

    //Write style for screen_warring, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_warring, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_warring, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_waring_2
    ui->screen_waring_2 = lv_image_create(ui->screen);
    lv_obj_set_pos(ui->screen_waring_2, 106, 79);
    lv_obj_set_size(ui->screen_waring_2, 20, 20);
    lv_obj_add_flag(ui->screen_waring_2, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui->screen_waring_2, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_waring_2, &_waring_RGB565A8_20x20);
    lv_image_set_pivot(ui->screen_waring_2, 50,50);
    lv_image_set_rotation(ui->screen_waring_2, 0);

    //Write style for screen_waring_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_waring_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_waring_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_waring_3
    ui->screen_waring_3 = lv_image_create(ui->screen);
    lv_obj_set_pos(ui->screen_waring_3, 82, 47);
    lv_obj_set_size(ui->screen_waring_3, 20, 20);
    lv_obj_add_flag(ui->screen_waring_3, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui->screen_waring_3, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_waring_3, &_waring_RGB565A8_20x20);
    lv_image_set_pivot(ui->screen_waring_3, 50,50);
    lv_image_set_rotation(ui->screen_waring_3, 0);

    //Write style for screen_waring_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_waring_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_waring_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_waring_4
    ui->screen_waring_4 = lv_image_create(ui->screen);
    lv_obj_set_pos(ui->screen_waring_4, 225, 28);
    lv_obj_set_size(ui->screen_waring_4, 20, 20);
    lv_obj_add_flag(ui->screen_waring_4, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui->screen_waring_4, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_waring_4, &_waring_RGB565A8_20x20);
    lv_image_set_pivot(ui->screen_waring_4, 50,50);
    lv_image_set_rotation(ui->screen_waring_4, 0);

    //Write style for screen_waring_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_waring_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_waring_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_danger
    ui->screen_danger = lv_image_create(ui->screen);
    lv_obj_set_pos(ui->screen_danger, 126, 116);
    lv_obj_set_size(ui->screen_danger, 20, 20);
    lv_obj_add_flag(ui->screen_danger, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui->screen_danger, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_danger, &_waring_RGB565A8_20x20);
    lv_image_set_pivot(ui->screen_danger, 50,50);
    lv_image_set_rotation(ui->screen_danger, 0);

    //Write style for screen_danger, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_danger, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_recolor(ui->screen_danger, lv_color_hex(0xf00000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_danger, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_danger_2
    ui->screen_danger_2 = lv_image_create(ui->screen);
    lv_obj_set_pos(ui->screen_danger_2, 106, 79);
    lv_obj_set_size(ui->screen_danger_2, 20, 20);
    lv_obj_add_flag(ui->screen_danger_2, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui->screen_danger_2, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_danger_2, &_waring_RGB565A8_20x20);
    lv_image_set_pivot(ui->screen_danger_2, 50,50);
    lv_image_set_rotation(ui->screen_danger_2, 0);

    //Write style for screen_danger_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_danger_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_recolor(ui->screen_danger_2, lv_color_hex(0xf00000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_danger_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_danger_3
    ui->screen_danger_3 = lv_image_create(ui->screen);
    lv_obj_set_pos(ui->screen_danger_3, 82, 47);
    lv_obj_set_size(ui->screen_danger_3, 20, 20);
    lv_obj_add_flag(ui->screen_danger_3, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui->screen_danger_3, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_danger_3, &_waring_RGB565A8_20x20);
    lv_image_set_pivot(ui->screen_danger_3, 50,50);
    lv_image_set_rotation(ui->screen_danger_3, 0);

    //Write style for screen_danger_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_danger_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_recolor(ui->screen_danger_3, lv_color_hex(0xf00000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_danger_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_danger_4
    ui->screen_danger_4 = lv_image_create(ui->screen);
    lv_obj_set_pos(ui->screen_danger_4, 225, 28);
    lv_obj_set_size(ui->screen_danger_4, 20, 20);
    lv_obj_add_flag(ui->screen_danger_4, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui->screen_danger_4, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_danger_4, &_waring_RGB565A8_20x20);
    lv_image_set_pivot(ui->screen_danger_4, 50,50);
    lv_image_set_rotation(ui->screen_danger_4, 0);

    //Write style for screen_danger_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_danger_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_recolor(ui->screen_danger_4, lv_color_hex(0xf00000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_danger_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_wifi_off
    ui->screen_wifi_off = lv_label_create(ui->screen);
    lv_obj_set_pos(ui->screen_wifi_off, 195, 4);
    lv_obj_set_size(ui->screen_wifi_off, 44, 16);
    lv_label_set_text(ui->screen_wifi_off, "");
    lv_label_set_long_mode(ui->screen_wifi_off, LV_LABEL_LONG_WRAP);

    //Write style for screen_wifi_off, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_wifi_off, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_wifi_off, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_wifi_off, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_wifi_off, &lv_font_icon_zero_two_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_wifi_off, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_wifi_off, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_wifi_off, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_wifi_off, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_wifi_off, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_wifi_off, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_wifi_off, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_wifi_off, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_wifi_off, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_wifi_off, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_wifi_on
    ui->screen_wifi_on = lv_label_create(ui->screen);
    lv_obj_set_pos(ui->screen_wifi_on, 195, 4);
    lv_obj_set_size(ui->screen_wifi_on, 44, 16);
    lv_obj_add_flag(ui->screen_wifi_on, LV_OBJ_FLAG_HIDDEN);
    lv_label_set_text(ui->screen_wifi_on, "");
    lv_label_set_long_mode(ui->screen_wifi_on, LV_LABEL_LONG_WRAP);

    //Write style for screen_wifi_on, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_wifi_on, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_wifi_on, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_wifi_on, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_wifi_on, &lv_font_icon_zero_two_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_wifi_on, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_wifi_on, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_wifi_on, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_wifi_on, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_wifi_on, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_wifi_on, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_wifi_on, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_wifi_on, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_wifi_on, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_wifi_on, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_pump_k
    ui->screen_pump_k = lv_button_create(ui->screen);
    lv_obj_set_pos(ui->screen_pump_k, 155, 54);
    lv_obj_set_size(ui->screen_pump_k, 62, 24);
    ui->screen_pump_k_label = lv_label_create(ui->screen_pump_k);
    lv_label_set_text(ui->screen_pump_k_label, "水泵:关");
    lv_label_set_long_mode(ui->screen_pump_k_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_pump_k_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_pump_k, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_pump_k_label, LV_PCT(100));

    //Write style for screen_pump_k, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_pump_k, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_pump_k, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_pump_k, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_pump_k, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_pump_k, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_pump_k, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_pump_k, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_pump_k, &lv_font_YouSheYuFeiTeJianKangTi_2_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_pump_k, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_pump_k, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_light_k
    ui->screen_light_k = lv_button_create(ui->screen);
    lv_obj_set_pos(ui->screen_light_k, 155, 105);
    lv_obj_set_size(ui->screen_light_k, 62, 24);
    ui->screen_light_k_label = lv_label_create(ui->screen_light_k);
    lv_label_set_text(ui->screen_light_k_label, "补光:关");
    lv_label_set_long_mode(ui->screen_light_k_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_light_k_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_light_k, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_light_k_label, LV_PCT(100));

    //Write style for screen_light_k, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_light_k, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_light_k, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_light_k, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_light_k, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_light_k, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_light_k, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_light_k, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_light_k, &lv_font_YouSheYuFeiTeJianKangTi_2_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_light_k, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_light_k, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_label_3
    ui->screen_label_3 = lv_label_create(ui->screen);
    lv_obj_set_pos(ui->screen_label_3, 115, -63);
    lv_obj_set_size(ui->screen_label_3, 100, 32);
    lv_label_set_text(ui->screen_label_3, "极暗偏正常适宜过强");
    lv_label_set_long_mode(ui->screen_label_3, LV_LABEL_LONG_WRAP);

    //Write style for screen_label_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_label_3, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_label_3, &lv_font_YouSheYuFeiTeJianKangTi_2_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_label_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_label_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen);

    //Init events for screen.
    events_init_screen(ui);
}
