



#include <katsu/kt.h>
#include <stdlib.h>
#include <math.h>

#include "video_common.h"
#include "gfx_common.h"

#define COLOROFFS(r, g, b)	(u32) ((((r) & 0x1FFu)) | (((g) & 0x1FFu) << 9) | (((b) & 0x1FFu) << 18))

u8 tile_mem[KT_MAX_TILES * 32];
u8 pal_mem[KT_MAX_COLORS * 4];
u8 tmap_mem[KT_MAX_TILEMAPS * 64 * 64 * sizeof(KTChr)];
KTMtx mtx_mem[KT_MAX_MTX];
KTColor backcolor;
u32 coloroffs;
u32 colorline_cnt;
u32 colorline[0x100];
u32 window_cnt;
u32 window[2][0x100];
u32 blend_mode;
Layer layer_mem[16];



/* Graphics Loading Functions */
void kt_TilesetLoad(u32 tile_num, u32 tile_count, const void* data)
{
	tile_num = ((tile_num & (KT_MAX_TILES - 1)) << 3);
	tile_count <<= 3;
	u32 *dst = (u32*) tile_mem;
	if (data) {
		u32 *src = (u32*) data;
		for (u32 i = 0; i < tile_count; ++i) {
			dst[tile_num++] = src[i];
			tile_num &= ((KT_MAX_TILES << 3) - 1);
		}
		//Set the first tile as zero
		dst[0] = 0;
		dst[1] = 0;
		dst[2] = 0;
		dst[3] = 0;
		dst[4] = 0;
		dst[5] = 0;
		dst[6] = 0;
		dst[7] = 0;

	} else {
		for (u32 i = 0; i < tile_count; ++i) {
			dst[tile_num++] = 0;
			tile_num &= ((KT_MAX_TILES << 3) - 1);
		}
	}
}


void kt_TilemapSetChr(u32 tmap, u32 x, u32 y, u32 tile_num, u32 flip, u32 pal)
{
	x &= 0x3Fu;
	y &= 0x3Fu;
	u32 ofs = (((tmap & 0xf) * (64 * 64)) + (y << 6) + x) << 2;
	tmap_mem[ofs] = pal & 0x7F;
	tmap_mem[ofs+1] = 0;
	tmap_mem[ofs+2] = ((flip & 0x3) << 6) | ((tile_num & 0x3F00) >> 8);
	tmap_mem[ofs+3] = tile_num & 0xFF;
}


void kt_TilemapLoad(u32 tmap, u32 size, u32 x, u32 y, u32 w, u32 h, u32 stride, const void* data)
{
	u32 *src = (u32*) data;
	u32 *dst = (u32*) tmap_mem;
	for (u32 j = 0; j < h; ++j) {
		//TODO: use the size parameter
		for (u32 i = 0; i < w; ++i) {
			u32 ofs = (((tmap & 0xf) * (64 * 64)) + ((j + y) << 6) + (i + x));
			dst[ofs & 0xFFFFu] = src[i];
		}
		src += stride;
	}
}


void kt_PaletteLoad(u32 color_num, u32 color_count, const void* data)
{
	color_num &= (KT_MAX_COLORS - 1);
	u32 *dst = (u32*) pal_mem;
	if (data) {
		u32 *src = (u32*) data;
		for (u32 i = 0; i < color_count; ++i) {
			dst[color_num++] = src[i];
			color_num &= (KT_MAX_COLORS - 1);
		}
	} else {
		for (u32 i = 0; i < color_count; ++i) {
			dst[color_num++] = 0;
			color_num &= (KT_MAX_COLORS - 1);
		}
	}
}

void kt_PaletteSetColor(u32 color_num, KTColor color)
{
	u32 ofs = (color_num & (KT_MAX_COLORS - 1)) << 2;
	pal_mem[ofs] = color.r;
	pal_mem[ofs+1] = color.g;
	pal_mem[ofs+2] = color.b;
}



/*Layers*/
void kt_LayerInitMap(u32 layer, u32 type, u32 tmap, u32 map_size)
{
	kt_LayerClear(layer);
	kt_LayerSetType(layer, type);
	kt_LayerSetMapSize(layer, tmap, map_size);
}


void kt_LayerInitSprite(u32 layer, u32 spr_count, KTSpr *data)
{
	u32 type = (spr_count && data ? KT_LAYER_SPRITE : KT_LAYER_NONE);
	u32 count = (type == KT_LAYER_SPRITE ? spr_count & (KT_MAX_SPRITES - 1) : 0);
	kt_LayerClear(layer);
	kt_LayerSetType(layer, type);
	kt_LayerSetAppData(layer, count, data);
}


void kt_LayerSetType(u32 layer, u32 type)
{
	layer &= 0xF;
	layer_mem[layer].type = type & 0x7;
}


void kt_LayerSetMapSize(u32 layer, u32 tmap, u32 map_size)
{
	layer &= 0xF;
	layer_mem[layer].map_attr &= ~0x003F0000u;
	layer_mem[layer].map_attr = ((map_size & 0x3) << 20) | ((tmap & 0xF) << 16);
}


void kt_LayerSetMapOffset(u32 layer, u32 x_ofs, u32 y_ofs)
{
	layer &= 0xF;
	layer_mem[layer].map_ofs = (x_ofs & 0xFFFFu) | (y_ofs << 16);
}


void kt_LayerSetMapBlend(u32 layer, u32 active, u8 alpha)
{
	layer &= 0xF;
	active = active ? 0x80000000u : 0x0;
	layer_mem[layer].map_attr &= ~0x800000FFu;
	layer_mem[layer].map_attr |= (active | (alpha & 0xFFu));
}


void kt_LayerSetMapRect(u32 layer, u32 x, u32 y, u32 w, u32 h)
{
	layer &= 0xF;
	layer_mem[layer].rect_pos = (x & 0xFFFFu) | (y << 16);
	layer_mem[layer].rect_size = (w & 0xFFFFu) | (h << 16);
}


void kt_LayerSetMapMosaic(u32 layer, u32 active, u32 mos_x, u32 mos_y)
{
	//XXX: this function is not finished
	layer &= 0xF;
	layer_mem[layer].map_attr &= ~0x0000FF00u;
	layer_mem[layer].map_attr = ((mos_x & 0xF) << 8) | ((mos_y & 0xF) << 12);
}


void kt_LayerSetMapChrOffset(u32 layer, u32 tile_ofs, u32 pal_ofs)
{
	layer &= 0xF;
	layer_mem[layer].chr_ofs = ((pal_ofs & 0x7Fu) << 16) | (tile_ofs & 0xFFFFu);
}


void kt_LayerSetBlendMode(u32 layer, u32 src_factor, u32 dst_factor, u32 func)
{
	layer &= 0xF;
	layer_mem[layer].blnd = (src_factor & 0x7) | ((dst_factor & 0x7) << 3) | ((func & 0x3) << 6);
}


void kt_LayerSetAppData(u32 layer, u32 num_elems, void* data)
{
	layer &= 0xF;
	if (num_elems && data) {
		layer_mem[layer].data_count = num_elems;
		layer_mem[layer].data_ptr = data;
	} else {
		layer_mem[layer].data_count = 0;
		layer_mem[layer].data_ptr = NULL;
	}
}


void kt_LayerSetWindow(u32 layer, u32 act_windows)
{
	layer &= 0xF;
	layer_mem[layer].win_act = act_windows;
}


void kt_LayerClear(u32 layer)
{
	layer &= 0xF;
	layer_mem[layer].type = KT_LAYER_NONE;
	kt_LayerSetMapRect(layer, 0, 0, KT_VIDEO_MAX_WIDTH, KT_VIDEO_MAX_HEIGHT);
	layer_mem[layer].map_attr = 0;
	layer_mem[layer].map_ofs = 0;
	layer_mem[layer].blnd = 0x0;
	layer_mem[layer].win_act = 0xF;
	layer_mem[layer].data_count = 0;
	layer_mem[layer].data_ptr = NULL;
}


void kt_LayerClearAll(void)
{
	u32 i = 0;
	while (i < KT_MAX_LAYERS) {
		kt_LayerClear(i);
		++i;
	}
}



/*Matrices*/
void kt_MtxSet(u32 mtx_idx, f32 a, f32 b, f32 c, f32 d)
{
	u32 i = mtx_idx & 0xFF;
	if (i == KT_MTX_IDENTITY) {
		return;
	}
	mtx_mem[i].a = MIN(MAX(a, -8.0), 8.0);
	mtx_mem[i].b = MIN(MAX(b, -8.0), 8.0);
	mtx_mem[i].c = MIN(MAX(c, -8.0), 8.0);
	mtx_mem[i].d = MIN(MAX(d, -8.0), 8.0);
}

void kt_MtxSetRotoscale(u32 mtx_idx, f32 x_scale, f32 y_scale, f32 angle)
{
	//Rotation matrix
	f32 sn = sin(angle);
	f32 cs = cos(angle);

	f32 a = cs * x_scale;
	f32 b = -sn * y_scale;
	f32 c = sn * x_scale;
	f32 d = cs * x_scale;

	kt_MtxSet(mtx_idx, a, b, c, d);
}



/*Windows*/
void kt_WindowBox(u32 win, u32 x, u32 y, u32 w, u32 h)
{

}

void kt_WindowLine(u32 win, u32 fill_mode, u32 line_count, const void* data)
{

}



/* Color Related Functions */
void kt_BackColor(KTColor color)
{
	backcolor = color;
}


void kt_OffsetColor(s32 r, s32 g, s32 b)
{
	r = MIN(MAX(r, -255), 255);
	g = MIN(MAX(g, -255), 255);
	b = MIN(MAX(b, -255), 255);
	coloroffs = COLOROFFS(r, g, b);
}


void kt_ColorLineSetParams(u32 fill_mode, u32 line_offset)
{
	//active = active ? 0x10 : 0;
	//line_offset
	colorline_cnt = (fill_mode & (KT_MAX_COLORLINE_FILL - 1));
}


void kt_ColorLineLoad(u32 line_count, const void* data)
{
	line_count &= 0xFF;
	if (data) {
		u32 *ptr = (u32*) data;
		for (u32 i = 0; i < line_count; ++i) {
			colorline[i] = ptr[i];
		}
	} else {
		for (u32 i = 0; i < line_count; ++i) {
			colorline[i] = 0;
		}
	}
}



/*Utils*/
void kt_Reset(void)
{
	const KTColor black_color = {0};
	kt_BackColor(black_color);
	kt_OffsetColor(0, 0, 0);
	kt_LayerClearAll();
	kt_TilesetLoad(0, KT_MAX_TILES, NULL);
	kt_PaletteLoad(0, KT_MAX_COLORS, NULL);

	/*Init matrix memory*/
	mtx_mem[0].a = 1.0f;
	mtx_mem[0].b = 0.0f;
	mtx_mem[0].c = 0.0f;
	mtx_mem[0].d = 1.0f;
	for (u32 i = 1; i < KT_MAX_MTX; ++i) {
		kt_MtxSet(i, 0.0, 0.0, 0.0, 0.0);
	}
}


u32  kt_LerpColor(u32 c0, u32 c1, u8 t)
{
	return  (((c0 & 0xFF00FF) + ((((c1 & 0xFF00FF) - (c0 & 0xFF00FF))*t) >> 8)) & 0xFF00FF) +
			(((c0 & 0x00FF00) + ((((c1 & 0x00FF00) - (c0 & 0x00FF00))*t) >> 8)) & 0x00FF00);
}
