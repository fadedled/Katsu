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

typedef struct Layer_t {
	u32 type;
	u32 stuff1;
	u32 stuff2;
	u32 stuff3;
} Layer;

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
