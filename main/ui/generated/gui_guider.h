/*
* Copyright 2026 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"


typedef struct
{
  
	lv_obj_t *screen;
	bool screen_del;
	lv_obj_t *screen_jia_bar;
	lv_obj_t *screen_ling_bar;
	lv_obj_t *screen_sty_fire;
	lv_obj_t *screen_cont_2;
	lv_obj_t *screen_img_1;
	lv_obj_t *screen_img_2;
	lv_obj_t *screen_img_3;
	lv_obj_t *screen_img_4;
	lv_obj_t *screen_water_temp;
	lv_obj_t *screen_ph_temp;
	lv_obj_t *screen_ling;
	lv_obj_t *screen_dan;
	lv_obj_t *screen_jia;
	lv_obj_t *screen_bar_1;
	lv_obj_t *screen_cont_1;
	lv_obj_t *screen_label_1;
	lv_obj_t *screen_label_2;
	lv_obj_t *screen_sitting;
	lv_obj_t *screen_sitting_label;
	lv_obj_t *screen_more;
	lv_obj_t *screen_more_label;
	lv_obj_t *screen_warring;
	lv_obj_t *screen_waring_2;
	lv_obj_t *screen_waring_3;
	lv_obj_t *screen_waring_4;
	lv_obj_t *screen_danger;
	lv_obj_t *screen_danger_2;
	lv_obj_t *screen_danger_3;
	lv_obj_t *screen_danger_4;
	lv_obj_t *screen_wifi_off;
	lv_obj_t *screen_wifi_on;
	lv_obj_t *screen_pump_k;
	lv_obj_t *screen_pump_k_label;
	lv_obj_t *screen_light_k;
	lv_obj_t *screen_light_k_label;
	lv_obj_t *screen_label_3;
	lv_obj_t *sitting_scr;
	bool sitting_scr_del;
	lv_obj_t *sitting_scr_menu_1;
	lv_obj_t *sitting_scr_menu_1_sidebar_page;
	lv_obj_t *sitting_scr_menu_1_subpage_1_cont;
	lv_obj_t *sitting_scr_menu_1_cont_1;
	lv_obj_t *sitting_scr_menu_1_label_1;
	lv_obj_t *sitting_scr_menu_1_subpage_2_cont;
	lv_obj_t *sitting_scr_menu_1_cont_2;
	lv_obj_t *sitting_scr_menu_1_label_2;
	lv_obj_t *sitting_scr_menu_1_subpage_3_cont;
	lv_obj_t *sitting_scr_menu_1_cont_3;
	lv_obj_t *sitting_scr_menu_1_label_3;
	lv_obj_t *sitting_scr_cont_1;
	lv_obj_t *sitting_scr_wif_conn_load;
	lv_span_t *sitting_scr_wif_conn_load_span;
	lv_obj_t *sitting_scr_spinner_1;
	lv_obj_t *sitting_scr_wifi_conn_unsucc;
	lv_span_t *sitting_scr_wifi_conn_unsucc_span;
	lv_obj_t *sitting_scr_wifi_conn_succ;
	lv_span_t *sitting_scr_wifi_conn_succ_span;
	lv_obj_t *sitting_scr_qrcode_1;
	lv_obj_t *sitting_scr_wifi_Incorrect_password;
	lv_span_t *sitting_scr_wifi_Incorrect_password_span;
	lv_obj_t *sitting_scr_WIFI_TITLE;
	lv_span_t *sitting_scr_WIFI_TITLE_span;
	lv_obj_t *sitting_scr_cb_1;
	lv_obj_t *sitting_scr_cb_2;
	lv_obj_t *sitting_scr_cb_3;
	lv_obj_t *sitting_scr_cb_4;
	lv_obj_t *sitting_scr_cb_5;
	lv_obj_t *sitting_scr_btn_2;
	lv_obj_t *sitting_scr_btn_2_label;
	lv_obj_t *sitting_scr_sw_1;
	lv_obj_t *sitting_scr_label_1;
	lv_obj_t *sitting_scr_btn_1;
	lv_obj_t *sitting_scr_btn_1_label;
	lv_obj_t *sitting_scr_back_1;
	lv_obj_t *sitting_scr_back_1_label;
	lv_obj_t *more_scr;
	bool more_scr_del;
	lv_obj_t *more_scr_more_ss;
	lv_obj_t *more_scr_more_ss_tab_1;
	lv_obj_t *more_scr_more_ss_tab_2;
	lv_obj_t *more_scr_more_ss_tab_3;
	lv_obj_t *more_scr_tab_2;
	lv_obj_t *more_scr_chart_1;
	lv_chart_series_t *more_scr_chart_1_0;
	lv_obj_t *more_scr_label_1;
	lv_obj_t *more_scr_label_2;
	lv_obj_t *more_scr_label_3;
	lv_obj_t *more_scr_btn_2;
	lv_obj_t *more_scr_btn_2_label;
}lv_ui;

typedef void (*ui_setup_scr_t)(lv_ui * ui);

void ui_init_style(lv_style_t * style);

void ui_load_scr_animation(lv_ui *ui, lv_obj_t ** new_scr, bool new_scr_del, bool * old_scr_del, ui_setup_scr_t setup_scr,
                           lv_screen_load_anim_t anim_type, uint32_t time, uint32_t delay, bool is_clean, bool auto_del);

void ui_animation(void * var, uint32_t duration, int32_t delay, int32_t start_value, int32_t end_value, lv_anim_path_cb_t path_cb,
                  uint32_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time, uint32_t playback_delay,
                  lv_anim_exec_xcb_t exec_cb, lv_anim_start_cb_t start_cb, lv_anim_completed_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);


void init_scr_del_flag(lv_ui *ui);

void setup_bottom_layer(void);

void setup_ui(lv_ui *ui);

void video_play(lv_ui *ui);

void init_keyboard(lv_ui *ui);

extern lv_ui guider_ui;


void setup_scr_screen(lv_ui *ui);
void setup_scr_sitting_scr(lv_ui *ui);
void setup_scr_more_scr(lv_ui *ui);
LV_IMAGE_DECLARE(_772804_RGB565A8_49x47);
LV_IMAGE_DECLARE(_10_RGB565A8_100x100);
LV_IMAGE_DECLARE(_34_RGB565A8_78x73);
LV_IMAGE_DECLARE(_324754_RGB565A8_110x105);
LV_IMAGE_DECLARE(_275342_RGB565A8_105x100);
LV_IMAGE_DECLARE(_waring_RGB565A8_20x20);

LV_FONT_DECLARE(lv_font_YouSheYuFeiTeJianKangTi_2_16)
LV_FONT_DECLARE(lv_font_YouSheYuFeiTeJianKangTi_2_18)
LV_FONT_DECLARE(lv_font_icon_zero_two_18)
LV_FONT_DECLARE(lv_font_YouSheYuFeiTeJianKangTi_2_12)
LV_FONT_DECLARE(lv_font_YouSheYuFeiTeJianKangTi_2_20)
LV_FONT_DECLARE(lv_font_montserratMedium_16)
LV_FONT_DECLARE(lv_font_HYPixel11pxU_2_18)


#ifdef __cplusplus
}
#endif
#endif
