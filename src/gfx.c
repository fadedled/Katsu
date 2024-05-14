



#include <katsu/kt.h>
#include <stdlib.h>

#include "video_common.h"
#include "gfx_common.h"

#define COLOROFFS(r, g, b)	(u32) ((((r) & 0x1FFu)) | (((g) & 0x1FFu) << 9) | (((b) & 0x1FFu) << 18))

u32 tile_mem[0x20000];
u32 pal_mem[0x800];
u32 tmap_mem[0x10000];
mat mat_mem[0x100];
u32 backcolor;
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
	tile_num = ((tile_num & (MAX_TILES - 1)) << 3);
	tile_count <<= 3;

	if (data) {
		u32 *ptr = (u32*) data;
		for (u32 i = 0; i < tile_count; ++i) {
			tile_mem[tile_num++] = ptr[i];
			tile_num &= ((MAX_TILES << 3) - 1);
		}
		//Set the first tile as zero
		tile_mem[0] = 0;
		tile_mem[1] = 0;
		tile_mem[2] = 0;
		tile_mem[3] = 0;
		tile_mem[4] = 0;
		tile_mem[5] = 0;
		tile_mem[6] = 0;
		tile_mem[7] = 0;

	} else {
		for (u32 i = 0; i < tile_count; ++i) {
			tile_mem[tile_num++] = 0;
			tile_num &= ((MAX_TILES << 3) - 1);
		}
	}
}


void kt_TilemapSetChr(u32 tmap, u32 x, u32 y, u32 value)
{
	u32 ofs = tmap * (64 * 64);
	x &= 0x3Fu;
	y &= 0x3Fu;
	tmap_mem[ofs + (y << 6) + x] = value;
}

void kt_TilemapLoad(u32 tmap, u32 size, u32 x, u32 y, u32 w, u32 h, u32 stride, const void* data)
{
	/*x &= 0x7Fu;
	y = (y & 0x7Fu) << 7u;
	w &= 0x7Fu;
	h &= 0x7Fu;
	tmap &= (MAX_TILEMAPS-1);
	Check for different sizes of bg
	u32 *bg_mem = _plm + BG0_MEM + (BG_SIZE * bg);
	stride = (data ? stride - w : 0);
	while (h--) {
		u32 w_tmp = w;
		u32 x_tmp = x;
		while (w_tmp--) {
			bg_mem[y + x_tmp] = (data ? *data++ : 0);
			x_tmp = (x_tmp + 1) & 0x7Fu;
		}
		y = (y + 1) & 0x3F80u;
		data += stride;
	}
	*/
}


void kt_PaletteLoad(u32 color_num, u32 color_count, const void* data)
{
	color_num &= (MAX_COLORS - 1);

	if (data) {
		u32 *ptr = (u32*) data;
		for (u32 i = 0; i < color_count; ++i) {
			pal_mem[color_num++] = ptr[i];
			color_num &= (MAX_COLORS - 1);
		}
	} else {
		for (u32 i = 0; i < color_count; ++i) {
			pal_mem[color_num++] = 0;
			color_num &= (MAX_COLORS - 1);
		}
	}
}

void kt_PaletteSetColor(u32 color_num, u32 color)
{
	color_num &= (MAX_COLORS - 1);
	pal_mem[color_num] = color;
}



/*Layers*/
void kt_LayerMap(u32 layer, u32 type, u32 tmap, u32 size)
{
	layer &= 0xF;
	layer_mem[layer].type = type & 0x3;
	layer_mem[layer].rect_pos = 0;
	layer_mem[layer].rect_size = (VIDEO_MAX_HEIGHT << 16) | (VIDEO_MAX_WIDTH & 0xFFFFu);
	layer_mem[layer].map_attr = ((size & 0x3) << 20) | ((tmap & 0xF) << 16);
	layer_mem[layer].udata_count = 0;
	layer_mem[layer].udata_arr = NULL;
}


void kt_LayerMapOffset(u32 layer, u32 x_ofs, u32 y_ofs)
{
	layer &= 0xF;
	layer_mem[layer].map_ofs = (x_ofs & 0xFFFFu) | (y_ofs << 16);
}


void kt_LayerMapAlpha(u32 layer, u32 active, u8 alpha)
{
	layer &= 0xF;
	active = active ? 0x80000000u : 0x0;
	layer_mem[layer].map_attr &= ~0x800000FFu;
	layer_mem[layer].map_attr |= (active | (alpha & 0xFFu));
}


void kt_LayerMapMosaic(u32 layer, u32 active, u32 mos_x, u32 mos_y)
{

}


void kt_LayerSprite(u32 layer, u32 spr_count, Sprite *data)
{
	layer &= 0xF;
	if (spr_count && data) {
		layer_mem[layer].type = LAYER_TYPE_SPRITE;
		layer_mem[layer].udata_count = spr_count & (MAX_SPRITES - 1);
		layer_mem[layer].udata_arr = (void *) data;
	} else {
		layer_mem[layer].type = LAYER_TYPE_NONE;
	}
}


void kt_LayerRect(u32 layer, u32 x, u32 y, u32 w, u32 h)
{
	layer &= 0xF;
	layer_mem[layer].rect_pos = (x & 0xFFFFu) | (y << 16);
	layer_mem[layer].rect_size = (w & 0xFFFFu) | (h << 16);
}


void kt_LayerBlendMode(u32 layer, u32 src_alpha, u32 dst_alpha, u32 func)
{
	layer &= 0xF;
	layer_mem[layer].blnd = (src_alpha & 0x7) | ((dst_alpha & 0x7) << 3) | ((func & 0x3) << 6);
}


void kt_LayerWindow(u32 layer, u32 act_windows)
{

}


void kt_LayerClear(u32 layer)
{
	layer &= 0xF;
	layer_mem[layer].type = LAYER_TYPE_NONE;
	layer_mem[layer].rect_pos = 0;
	layer_mem[layer].rect_size = (VIDEO_MAX_WIDTH << 16) | (VIDEO_MAX_HEIGHT & 0xFFFFu);
	layer_mem[layer].map_attr = 0;
	layer_mem[layer].udata_count = 0;
	layer_mem[layer].udata_arr = NULL;
}


void kt_LayerClearAll(void)
{
	u32 i = 0;
	while (i < MAX_LAYERS) {
		kt_LayerClear(i);
		++i;
	}
}



/*Matrices*/
void kt_MatrixLoad(u32 mat, f32 a, f32 b, f32 c, f32 d)
{

}

void kt_MatrixRotoscale(u32 mat, f32 x_scale, f32 y_scale, f32 angle)
{

}



/*Windows*/
void kt_WindowBox(u32 win, u32 x, u32 y, u32 w, u32 h)
{

}

void kt_WindowLine(u32 win, u32 fill_mode, u32 line_count, const void* data)
{

}



/* Color Related Functions */
void kt_BackColor(u32 color)
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
	colorline_cnt = (fill_mode & (MAX_COLORLINE_FILL - 1));
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
	kt_LayerClearAll();
}


u32  kt_LerpColor(u32 c0, u32 c1, u8 t)
{
	return  (((c0 & 0xFF00FF) + ((((c1 & 0xFF00FF) - (c0 & 0xFF00FF))*t) >> 8)) & 0xFF00FF) +
			(((c0 & 0x00FF00) + ((((c1 & 0x00FF00) - (c0 & 0x00FF00))*t) >> 8)) & 0x00FF00);
}
