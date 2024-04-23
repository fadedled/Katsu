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
 * pos: top-left corner shown
 * size: size of layer startng from top-left corner
 * attr: attributes for layer
 * udata_count: number of user data passed entries in udata_arr
 * udata_arr: array of user data
 * 	- if type is LAYER_TYPE_NONE then the array is not used.
 * 	- if type is LAYER_TYPE_BG_NORMAL then the array is not used.
 * 	- if type is LAYER_TYPE_BG_SCROLL then the entries are scaled offsets.
 * 	- if type is LAYER_TYPE_BG_AFFINE then these are matrices.
 * 	- if type is LAYER_TYPE_SPRITES then these are sprites.
 */
typedef struct Layer_t {
	u32 type;
	u32 pos;
	u32 size;
	u32 attr;
	u32 blnd;
	u32 udata_count;
	void *udata_arr;
} Layer;


#define S9TOS32(x)		((-((s32)(x) & 0x100) | ((s32)(x) & 0xFF)))

extern u32 tile_mem[0x20000];
extern u32 pal_mem[0x800];
extern u32 bg_mem[0x10000];
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
