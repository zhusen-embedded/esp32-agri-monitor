/*******************************************************************************
 * Size: 11 px
 * Bpp: 4
 * Opts: undefined
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl.h"
#endif

#ifndef LV_CUSTOMER_FONT_YOUSHEYUFEITEJIANKANGTI_2_11
#define LV_CUSTOMER_FONT_YOUSHEYUFEITEJIANKANGTI_2_11 1
#endif

#if LV_CUSTOMER_FONT_YOUSHEYUFEITEJIANKANGTI_2_11

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+56FE "图" */
    0xf, 0xff, 0xff, 0xff, 0xf1, 0x1e, 0x2, 0x40,
    0x0, 0xe1, 0x1e, 0x2e, 0xfe, 0xe5, 0xe1, 0x1e,
    0xbb, 0xb9, 0x90, 0xe1, 0x1e, 0x39, 0xec, 0xb3,
    0xe1, 0x1e, 0xac, 0xb5, 0x49, 0xe1, 0x1e, 0x4,
    0x7c, 0xe0, 0xe1, 0x1e, 0x3c, 0xea, 0x50, 0xe1,
    0xf, 0x0, 0x5, 0xa2, 0xf1, 0x8, 0xff, 0xff,
    0xff, 0x80,

    /* U+5EFA "建" */
    0x0, 0x0, 0x4, 0x80, 0x0, 0x6f, 0xf5, 0xef,
    0xff, 0x80, 0x3, 0xe3, 0x79, 0xc9, 0xc1, 0xb,
    0x63, 0x7a, 0xca, 0xc1, 0x1f, 0xf6, 0xde, 0xfe,
    0x40, 0x22, 0xa7, 0xac, 0xd9, 0x40, 0x5a, 0xe2,
    0x58, 0xb5, 0x20, 0xe, 0xbb, 0xff, 0xff, 0xe0,
    0x1e, 0xd3, 0x5, 0x90, 0x0, 0x97, 0x4c, 0xff,
    0xff, 0xe1,

    /* U+8868 "表" */
    0x0, 0x0, 0x85, 0x0, 0x0, 0xe, 0xff, 0xff,
    0xff, 0x90, 0x1, 0x22, 0xa8, 0x22, 0x0, 0xd,
    0xff, 0xff, 0xff, 0x80, 0x13, 0x33, 0xa8, 0x33,
    0x20, 0x7c, 0xce, 0xff, 0xcc, 0xc1, 0x0, 0x4e,
    0x3c, 0x51, 0x40, 0x7, 0xfb, 0x2, 0xde, 0x50,
    0x9c, 0x7b, 0x23, 0x4c, 0x30, 0x0, 0x4e, 0x93,
    0x2, 0xb0,

    /* U+8BAE "议" */
    0x8, 0x40, 0x5, 0x50, 0x0, 0x4, 0xb0, 0x83,
    0xc0, 0xb0, 0x0, 0xa0, 0xd0, 0xc2, 0xd0, 0x3,
    0x20, 0xa3, 0x5, 0x90, 0x2c, 0xe0, 0x69, 0xb,
    0x40, 0x1, 0xe0, 0xd, 0x4d, 0x0, 0x1, 0xe0,
    0x6, 0xf5, 0x0, 0x1, 0xe2, 0x6, 0xf6, 0x0,
    0x1, 0xfe, 0x5d, 0x3d, 0x50, 0x0, 0xd7, 0xd2,
    0x2, 0xd5, 0x0, 0x2, 0x10, 0x0, 0x11
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 158, .box_w = 10, .box_h = 10, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 50, .adv_w = 158, .box_w = 10, .box_h = 10, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 100, .adv_w = 158, .box_w = 10, .box_h = 10, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 150, .adv_w = 158, .box_w = 10, .box_h = 11, .ofs_x = 0, .ofs_y = -2}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_0[] = {
    0x0, 0x7fc, 0x316a, 0x34b0
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 22270, .range_length = 13489, .glyph_id_start = 1,
        .unicode_list = unicode_list_0, .glyph_id_ofs_list = NULL, .list_length = 4, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    }
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
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 1,
    .bpp = 4,
    .kern_classes = 0,
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
const lv_font_t lv_customer_font_YouSheYuFeiTeJianKangTi_2_11 = {
#else
lv_font_t lv_customer_font_YouSheYuFeiTeJianKangTi_2_11 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 11,          /*The maximum line height required by the font*/
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



#endif /*#if LV_CUSTOMER_FONT_YOUSHEYUFEITEJIANKANGTI_2_11*/

