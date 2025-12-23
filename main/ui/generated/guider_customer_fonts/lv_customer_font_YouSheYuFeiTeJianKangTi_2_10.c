/*******************************************************************************
 * Size: 10 px
 * Bpp: 4
 * Opts: undefined
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl.h"
#endif

#ifndef LV_CUSTOMER_FONT_YOUSHEYUFEITEJIANKANGTI_2_10
#define LV_CUSTOMER_FONT_YOUSHEYUFEITEJIANKANGTI_2_10 1
#endif

#if LV_CUSTOMER_FONT_YOUSHEYUFEITEJIANKANGTI_2_10

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+56FE "图" */
    0xf, 0xee, 0xee, 0xee, 0xa1, 0xc0, 0x41, 0x0,
    0x2c, 0x1c, 0x3f, 0xde, 0xb2, 0xc1, 0xc4, 0x7d,
    0xc0, 0x2c, 0x1c, 0xae, 0x78, 0xd8, 0xc1, 0xc2,
    0x4f, 0xb3, 0x3c, 0x1c, 0xa, 0x77, 0x22, 0xc1,
    0xd0, 0x26, 0xc7, 0x2b, 0xa, 0xee, 0xee, 0xee,
    0x50,

    /* U+5EFA "建" */
    0x13, 0x20, 0x3c, 0x31, 0x5, 0xbe, 0x4a, 0xfa,
    0xc0, 0x9, 0x58, 0xaf, 0xae, 0x33, 0xfc, 0x5b,
    0xfc, 0x90, 0x0, 0xd3, 0x6e, 0x63, 0x7, 0x7c,
    0x59, 0xf9, 0x60, 0x2f, 0x8d, 0xdf, 0xdd, 0x11,
    0xfa, 0x0, 0xd0, 0x0, 0x96, 0x7d, 0xdf, 0xdd,
    0x30,

    /* U+8868 "表" */
    0x0, 0x0, 0xc0, 0x0, 0x0, 0xbe, 0xef, 0xee,
    0xe2, 0x4, 0x77, 0xe7, 0x77, 0x0, 0x36, 0x6e,
    0x66, 0x50, 0x3e, 0xef, 0xff, 0xee, 0x90, 0x4,
    0xd4, 0xc0, 0x41, 0x6, 0xf8, 0x8, 0xeb, 0x3,
    0xb7, 0x96, 0x1a, 0x90, 0x0, 0x5d, 0x60, 0x7,
    0x80,

    /* U+8BAE "议" */
    0x0, 0x0, 0x0, 0x0, 0x1, 0xa0, 0x15, 0x70,
    0x20, 0xb, 0x2c, 0x1c, 0x2b, 0x0, 0x10, 0x94,
    0x56, 0x70, 0x9e, 0x25, 0x80, 0xa3, 0x0, 0xb3,
    0xd, 0x3c, 0x0, 0xb, 0x30, 0x6e, 0x50, 0x0,
    0xb4, 0x5, 0xf4, 0x0, 0xb, 0xe6, 0xd4, 0xd3,
    0x0, 0x99, 0xd2, 0x2, 0xd2, 0x0, 0x1, 0x0,
    0x0, 0x0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 144, .box_w = 9, .box_h = 9, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 41, .adv_w = 144, .box_w = 9, .box_h = 9, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 82, .adv_w = 144, .box_w = 9, .box_h = 9, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 123, .adv_w = 144, .box_w = 9, .box_h = 11, .ofs_x = 0, .ofs_y = -2}
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
const lv_font_t lv_customer_font_YouSheYuFeiTeJianKangTi_2_10 = {
#else
lv_font_t lv_customer_font_YouSheYuFeiTeJianKangTi_2_10 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 10,          /*The maximum line height required by the font*/
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



#endif /*#if LV_CUSTOMER_FONT_YOUSHEYUFEITEJIANKANGTI_2_10*/

