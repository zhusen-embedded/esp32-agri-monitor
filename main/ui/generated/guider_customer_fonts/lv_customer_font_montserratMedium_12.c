/*******************************************************************************
 * Size: 12 px
 * Bpp: 4
 * Opts: undefined
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl.h"
#endif

#ifndef LV_CUSTOMER_FONT_MONTSERRATMEDIUM_12
#define LV_CUSTOMER_FONT_MONTSERRATMEDIUM_12 1
#endif

#if LV_CUSTOMER_FONT_MONTSERRATMEDIUM_12

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */

    /* U+0025 "%" */
    0xa, 0xc8, 0x0, 0xc, 0x10, 0x66, 0xa, 0x20,
    0x76, 0x0, 0x83, 0x7, 0x42, 0xc0, 0x0, 0x57,
    0xa, 0x2b, 0x20, 0x0, 0x9, 0xc6, 0x68, 0x5c,
    0x90, 0x0, 0x1, 0xc1, 0xc0, 0x67, 0x0, 0xa,
    0x43, 0x90, 0x2a, 0x0, 0x49, 0x1, 0xb0, 0x47,
    0x0, 0xc1, 0x0, 0x7b, 0xb1,

    /* U+002E "." */
    0x2a, 0x4, 0xd0,

    /* U+003A ":" */
    0x4e, 0x2, 0xa0, 0x0, 0x0, 0x0, 0x0, 0x2,
    0xa0, 0x4d, 0x0,

    /* U+0048 "H" */
    0xb7, 0x0, 0x0, 0xb7, 0xb7, 0x0, 0x0, 0xb7,
    0xb7, 0x0, 0x0, 0xb7, 0xb7, 0x0, 0x0, 0xb7,
    0xbf, 0xff, 0xff, 0xf7, 0xb8, 0x22, 0x22, 0xc7,
    0xb7, 0x0, 0x0, 0xb7, 0xb7, 0x0, 0x0, 0xb7,
    0xb7, 0x0, 0x0, 0xb7,

    /* U+0050 "P" */
    0xbf, 0xff, 0xd8, 0x0, 0xb8, 0x22, 0x5d, 0x90,
    0xb7, 0x0, 0x4, 0xe0, 0xb7, 0x0, 0x3, 0xf0,
    0xb7, 0x0, 0x2c, 0xa0, 0xbf, 0xff, 0xfa, 0x10,
    0xb8, 0x22, 0x0, 0x0, 0xb7, 0x0, 0x0, 0x0,
    0xb7, 0x0, 0x0, 0x0,

    /* U+0066 "f" */
    0x1, 0xcf, 0x60, 0x9a, 0x11, 0xb, 0x60, 0xd,
    0xff, 0xf3, 0xb, 0x60, 0x0, 0xb6, 0x0, 0xb,
    0x60, 0x0, 0xb6, 0x0, 0xb, 0x60, 0x0, 0xb6,
    0x0,

    /* U+0069 "i" */
    0xd, 0x40, 0x82, 0x0, 0x0, 0xe4, 0xe, 0x40,
    0xe4, 0xe, 0x40, 0xe4, 0xe, 0x40, 0xe4,

    /* U+0077 "w" */
    0xc5, 0x0, 0x3f, 0x10, 0x7, 0x86, 0xa0, 0x9,
    0xf6, 0x0, 0xd3, 0x1f, 0x0, 0xe7, 0xb0, 0x2d,
    0x0, 0xb5, 0x4c, 0xe, 0x18, 0x80, 0x6, 0xa9,
    0x60, 0xa6, 0xd3, 0x0, 0x1e, 0xe1, 0x4, 0xed,
    0x0, 0x0, 0xbb, 0x0, 0xe, 0x80, 0x0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 52, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 0, .adv_w = 162, .box_w = 10, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 45, .adv_w = 44, .box_w = 3, .box_h = 2, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 48, .adv_w = 44, .box_w = 3, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 59, .adv_w = 156, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 95, .adv_w = 139, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 131, .adv_w = 68, .box_w = 5, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 156, .adv_w = 54, .box_w = 3, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 171, .adv_w = 173, .box_w = 11, .box_h = 7, .ofs_x = 0, .ofs_y = 0}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_0[] = {
    0x0, 0x5, 0xe, 0x1a, 0x28, 0x30, 0x46, 0x49,
    0x57
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 88, .glyph_id_start = 1,
        .unicode_list = unicode_list_0, .glyph_id_ofs_list = NULL, .list_length = 9, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    }
};

/*-----------------
 *    KERNING
 *----------------*/


/*Map glyph_ids to kern left classes*/
static const uint8_t kern_left_class_mapping[] =
{
    0, 0, 1, 2, 0, 0, 3, 4,
    0, 5
};

/*Map glyph_ids to kern right classes*/
static const uint8_t kern_right_class_mapping[] =
{
    0, 0, 1, 2, 3, 4, 4, 0,
    5, 6
};

/*Kern values between classes*/
static const int8_t kern_class_values[] =
{
    -23, 6, 0, 0, 0, 0, -12, 0,
    0, 0, 0, 0, 0, -5, 0, -2,
    -4, -1, 3, -2, 4, 0, 15, 2,
    0, -6, 1, 0, 0, -2
};


/*Collect the kern class' data in one place*/
static const lv_font_fmt_txt_kern_classes_t kern_classes =
{
    .class_pair_values   = kern_class_values,
    .left_class_mapping  = kern_left_class_mapping,
    .right_class_mapping = kern_right_class_mapping,
    .left_class_cnt      = 5,
    .right_class_cnt     = 6,
};

/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = &kern_classes,
    .kern_scale = 16,
    .cmap_num = 1,
    .bpp = 4,
    .kern_classes = 1,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif
};



/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t lv_customer_font_montserratMedium_12 = {
#else
lv_font_t lv_customer_font_montserratMedium_12 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 12,          /*The maximum line height required by the font*/
    .base_line = 1,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -1,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if LV_CUSTOMER_FONT_MONTSERRATMEDIUM_12*/

