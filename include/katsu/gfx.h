/*
 * gfx.h
 */


#ifndef __KT_GFX_H__
#define __KT_GFX_H__

#include <katsu/types.h>

/*Graphics Constants*/
#define MAX_TILES		0x4000
#define MAX_SPRITES		2048
#define MAX_LAYERS		16
#define MAX_BGS			16
#define MAX_COLORS		2048
#define MAX_PALETTES	(MAX_COLORS >> 4)


/*Layer types*/
#define LAYER_BG_TYPE_NORMAL		0
#define LAYER_BG_TYPE_SCROLL		1
#define LAYER_BG_TYPE_AFFINE		2


/*Sprites*/
#define SPR_POS(x, y)									((y << 16) | (x & 0xFFFFu))
#define SPR_TILE(tile_num, flip, hsize, vsize, pal)		((pal << 24) | ((vsize & 0xF) << 20) | ((hsize & 0xF) << 16) | ((flip & 0x3) << 14) | (tile_num & 0x3FFF))
#define SPR_HUE(hue, hue_alpha)							(((hue & 0x7FFFu) << 16) | ((hue_alpha & 0xFFu) << 8))
#define SPR_BLEND(alpha)								((0x80000000u) | (alpha & 0xFFu))
#define SPR_MAT(mat_num)								((0x00000100u) | (mat_num & 0xFFu))


/*Sprite Flip*/
#define FLIP_NONE		0x0
#define FLIP_X			0x1
#define FLIP_Y			0x2
#define FLIP_XY			0x3


/*Window IDs*/
enum WindowIds {
	WINDOW_0,
	WINDOW_1,
	WINDOW_SPR,
	WINDOW_OUT,
	MAX_WINDOW
};

/*Window Types*/
#define WINDOW_TYPE_BOX		0
#define WINDOW_TYPE_LINE	1

#define COLORLINE_FILL_STRETCH		0
#define COLORLINE_FILL_REPEAT		1
#define COLORLINE_FILL_NONE			2
#define COLORLINE_FILL_DUMMY		3
#define MAX_COLORLINE_FILL			4

#define COLOROFFS(r, g, b)	((((u32) (r) & 0x1FEu) >> 1) | (((u32) (g) & 0x1FEu) << 7) | (((u32) (b) & 0x1FEu) << 15))

/*Blending*/
#define BLEND_FUNC_ADD						0x0
#define BLEND_FUNC_SUB						0x1

#define KT_BLEND_ZERO						0x0
#define KT_BLEND_ONE						0x1
#define KT_BLEND_SRC_ALPHA					0x2
#define KT_BLEND_ONE_MINUS_SRC_ALPHA		0x3
#define KT_BLEND_DST_ALPHA					0x4
#define KT_BLEND_ONE_MINUS_DST_ALPHA		0x5


#define MAX_MATRIX							256





/*== Structs ==*/

/* BACKGROUND TILES
 * tile = [pal : 8][none : 8][vf : 1][hf : 1][tile_num : 14]
 */


/* SPRITE STRUCTURE
 * pos  = [pos_y : 16][pos_x : 16]
 * tile = [pal : 8][vsize : 4][hsize : 4][vf : 1][hf : 1][tile_num : 14]
 * sfx  = [blend : 1][hue : 15][hue_alpha : 8][alpha : 8]
 * mat  = [none : 23][mat_act : 1][mat_num : 8]
 */
typedef struct Sprite_t {
	u32 pos;
	u32 tile;
	u32 sfx;
	u32 mat;
} Sprite;

/* Graphics Loading Functions */
void kt_TileData(u32 tile_ofs, u32 tile_count, const u32* data);
void kt_PaletteData(u32 color_ofs, u32 color_count, const u32* data);
void kt_BgData(u32 bg_num, u32 bg_size, u32 x, u32 y, u32 w, u32 h, u32 stride, const u32* data);

void kt_BackColorSet(u32 color);
void kt_ColorOffset0Set(s32 r, s32 g, s32 b);
void kt_ColorOffset1Set(s32 r, s32 g, s32 b);
void kt_ColorLinesData(const u32* data, u32 count);
void kt_ColorLinesSet(u32 active, u32 fill_mode);


/*Layers*/
void kt_LayerBg(u32 layer, u32 type, u32 bg_num, u32 bg_size);
void kt_LayerBgBoxSet(u32 layer, u32 x, u32 y, u32 w, u32 h);
void kt_LayerBgOffsetSet(u32 layer, u32 x_ofs, u32 y_ofs);
void kt_LayerBgBlendSet(u32 layer, u32 active, u8 alpha);
void kt_LayerBgMosaicSet(u32 layer, u32 active, u32 mos_x, u32 mos_y);
void kt_LayerSprite(u32 layer, Sprite *spr, u32 count);

void kt_LayerWindowSet(u32 layer, u32 act_windows);
void kt_LayerClear(u32 layer);
void kt_LayerClearAll(void);


/*Matrices*/
void kt_MatrixSet(u32 mat, f32 a, f32 b, f32 c, f32 d);
void kt_MatrixRotoscale(u32 mat, f32 x_scale, f32 y_scale, f32 angle);


/*Windows*/
void kt_WindowBox(u32 win, u32 x, u32 y, u32 w, u32 h);
void kt_WindowLine(u32 win, const u32* data, u32 count);


/*Utils*/
void kt_Reset(void);
void kt_BlendModeSet(u32 src_alpha, u32 dst_alpha, u32 func);
u32  kt_ColorLerp(u32 color0, u32 color1, u8 blend);



#endif /*__KT_GFX_H__*/
