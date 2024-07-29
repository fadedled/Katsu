/*
 * gfx_common.h
 */


#ifndef __KT_GFX_COMMON_H__
#define __KT_GFX_COMMON_H__

#include <katsu/gfx.h>

typedef struct KTMtx_t {
	f32 a;
	f32 b;
	f32 c;
	f32 d;
} KTMtx;

/* LAYER STRUCTURE
 * type: controls what is shown
 * rect_pos = [pos_y : 16][pos_x : 16]  - top-left corner shown
 * rect_size = [height : 16][width : 16]  - size of layer startng from top-left corner
 * map_attr: [none : 8][blend : 1][norep : 1][tmap_size : 2][tmap : 4][mos_y : 4][mos_x : 4][alpha : 8] - attributes for backgorund layer
 * map_ofs: [ofs_y : 16][ofs_x : 16] - offset of background layer
 * map_scale: [scale_y : 16][scale_x : 16] - scale of background layer
 * chr_ofs: [pal_ofs : 16][tile_ofs : 16] - offset of tile and palette for characters
 * blnd: [func : 2][dst_factor : 3][src_factor : 3]  - Blending function and alpha source and destination
 * win_act: [spr_win : 1][win1 : 1][win0 : 1][out_win : 1]  - Window Activation
 * udata_count: number of app data passed entries in udata_arr
 * udata_arr: array of app data
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
	u32 map_scale;
	u32 chr_ofs;
	u8 blnd;
	u8 win_act;
	u32 data_count;
	void *data_ptr;
} Layer;


#define S9TOS32(x)		((-((s32)(x) & 0x100) | ((s32)(x) & 0xFF)))

extern u8 tile_mem[KT_MAX_TILES * 32];
extern u8 pal_mem[KT_MAX_COLORS * 4];
extern u8 tmap_mem[KT_MAX_TILEMAPS * 64 * 64 * sizeof(KTChr)];
extern KTMtx mtx_mem[KT_MAX_MTX];
extern KTColor backcolor;
extern u32 coloroffs;
extern u32 colorline_cnt;
extern u32 colorline[0x100];
extern u32 window_cnt;
extern u32 window[2][0x100];
extern u32 blend_mode;
extern Layer layer_mem[16];


#endif /*__KT_GFX_COMMON_H__*/
