



#include <katsu/kt.h>
#include <stdlib.h>

#include "video_common.h"
#include "gfx_common.h"

u32 tile_mem[0x20000];
u32 pal_mem[0x800];
u32 bg_mem[0x10000];
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
void kt_TileData(u32 tile_ofs, u32 tile_count, const u32* data)
{
	tile_ofs = ((tile_ofs & (MAX_TILES - 1)) << 3);
	tile_count <<= 3;

	if (data) {
		for (u32 i = 0; i < tile_count; ++i) {
			tile_mem[tile_ofs++] = data[i];
			tile_ofs &= ((MAX_TILES << 3) - 1);
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
			tile_mem[tile_ofs++] = 0;
			tile_ofs &= ((MAX_TILES << 3) - 1);
		}
	}
}

void kt_PaletteData(u32 color_ofs, u32 color_count, const u32* data)
{
	color_ofs &= (MAX_COLORS - 1);

	if (data) {
		for (u32 i = 0; i < color_count; ++i) {
			pal_mem[color_ofs++] = data[i];
			color_ofs &= (MAX_COLORS - 1);
		}
	} else {
		for (u32 i = 0; i < color_count; ++i) {
			pal_mem[color_ofs++] = 0;
			color_ofs &= (MAX_COLORS - 1);
		}
	}
}

void kt_BgData(u32 bg_num, u32 bg_size, u32 x, u32 y, u32 w, u32 h, u32 stride, const u32* data)
{

}

void kt_BackColorSet(u32 color)
{
	backcolor = color;
}


void kt_ColorOffsetSet(s32 r, s32 g, s32 b)
{
	coloroffs = COLOROFFS(r, g, b);
}


void kt_ColorLinesSet(u32 active, u32 fill_mode)
{
	active = active ? 0x10 : 0;
	colorline_cnt = active | (fill_mode & (MAX_COLORLINE_FILL - 1));
}


void kt_ColorLinesData(const u32* data, u32 count)
{
	count &= 0xFF;
	if (data) {
		for (u32 i = 0; i < count; ++i) {
			colorline[i] = data[i];
		}
	} else {
		for (u32 i = 0; i < count; ++i) {
			colorline[i] = 0;
		}
	}
}



/*Layers*/
void kt_LayerBg(u32 layer, u32 type, u32 bg_num, u32 bg_size)
{

}

void kt_LayerBgBoxSet(u32 layer, u32 x, u32 y, u32 w, u32 h)
{

}

void kt_LayerBgOffsetSet(u32 layer, u32 x_ofs, u32 y_ofs)
{

}

void kt_LayerBgBlendSet(u32 layer, u32 active, u8 alpha)
{

}

void kt_LayerBgMosaicSet(u32 layer, u32 active, u32 mos_x, u32 mos_y)
{

}

void kt_LayerSprite(u32 layer, Sprite *spr, u32 count)
{
	layer &= 0xF;
	if (count && spr) {
		layer_mem[layer].type = LAYER_TYPE_SPRITE;
		layer_mem[layer].udata_count = count & (MAX_SPRITES - 1);
		layer_mem[layer].udata_arr = (void *) spr;
	} else {
		layer_mem[layer].type = LAYER_TYPE_NONE;
	}
}


void kt_LayerWindowSet(u32 layer, u32 act_windows)
{

}

void kt_LayerClear(u32 layer)
{
	layer_mem[layer & 0xF].type = LAYER_TYPE_NONE;
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
void kt_MatrixSet(u32 mat, f32 a, f32 b, f32 c, f32 d)
{

}

void kt_MatrixRotoscale(u32 mat, f32 x_scale, f32 y_scale, f32 angle)
{

}



/*Windows*/
void kt_WindowBox(u32 win, u32 x, u32 y, u32 w, u32 h)
{

}

void kt_WindowLine(u32 win, const u32* data, u32 count)
{

}


/*Utils*/
void kt_Reset(void)
{

}

void kt_BlendModeSet(u32 src_alpha, u32 dst_alpha, u32 func)
{

}

u32  kt_ColorLerp(u32 color0, u32 color1, u8 blend)
{

}
