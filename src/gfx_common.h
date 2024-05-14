/*
 * gfx_common.h
 */


#ifndef __KT_GFX_COMMON_H__
#define __KT_GFX_COMMON_H__

#include <katsu/gfx.h>

typedef struct Matrix_t {
	f32 a;
	f32 b;
	f32 c;
	f32 d;
} mat;

/* LAYER STRUCTURE
 * type: controls what is shown
 * rect_pos = [pos_y : 16][pos_x : 16]  - top-left corner shown
 * rect_size = [height : 16][width : 16]  - size of layer startng from top-left corner
 * map_attr: [blend : 1][norep : 1][tmap_size : 2][tmap : 4][mos_y : 4][mos_x : 4][alpha : 8] - attributes for backgorund layer
 * map_ofs: [ofs_y : 16][ofs_x : 16] - offset of backgorund layer (unused for scroll bg)
 * blnd: [func : 2][dst_alpha : 3][src_alpha : 3]  - Blending function and alpha source and destination
 * win_act: [spr_win : 1][win1 : 1][win0 : 1][out_win : 1]  - Window Activation
 * udata_count: number of user data passed entries in udata_arr
 * udata_arr: array of user data
 * 	- if type is LAYER_TYPE_NONE then the array is not used.
 * 	- if type is LAYER_TYPE_MAP_NORMAL then the array is not used.
 * 	- if type is LAYER_TYPE_MAP_SCROLL then the entries are scaled offsets.
 * 	- if type is LAYER_TYPE_MAP_AFFINE then these are matrices.
 * 	- if type is LAYER_TYPE_SPRITES then these are sprites.
 */

typedef struct Layer_t {
	u32 type;
	u32 rect_pos;
	u32 rect_size;
	u32 map_attr;
	u32 map_ofs;
	u8 blnd;
	u8 win_act;
	u32 udata_count;
	void *udata_arr;
} Layer;


#define S9TOS32(x)		((-((s32)(x) & 0x100) | ((s32)(x) & 0xFF)))

extern u32 tile_mem[0x20000];
extern u32 pal_mem[0x800];
extern u32 tmap_mem[0x10000];
extern mat mat_mem[0x100];
extern u32 backcolor;
extern u32 coloroffs;
extern u32 colorline_cnt;
extern u32 colorline[0x100];
extern u32 window_cnt;
extern u32 window[2][0x100];
extern u32 blend_mode;
extern Layer layer_mem[16];


#endif /*__KT_GFX_COMMON_H__*/
