

#include <katsu/kt.h>
#include <stdio.h>
#include "system.h"

/*Demo Palette*/
extern u8 _binary_res_palettes_bg_data_pal_start[];
extern u8 _binary_res_palettes_bg_data_pal_end[];
extern u8 _binary_res_palettes_spr_data_pal_start[];
extern u8 _binary_res_palettes_spr_data_pal_end[];

/*Demo Tiles*/
extern u8 _binary_res_tiles_norm_map_tiles_4bpp_start[];
extern u8 _binary_res_tiles_norm_map_tiles_4bpp_end[];
extern u8 _binary_res_tiles_affine_map_tiles_4bpp_start[];
extern u8 _binary_res_tiles_affine_map_tiles_4bpp_end[];


#define TMAP_PALETTE_OFS 0
#define SPR_PALETTE_OFS (4 * 16)



int main() {
	if (kt_Init()) {
		return 0;
	}
	u32 x = 40;
	u32 y = 20;
	u32 bg_pal_size =  (u32)((u32)&_binary_res_palettes_bg_data_pal_end - (u32)&_binary_res_palettes_bg_data_pal_start);
	u32 nmap_tile_size =  (u32)((u32)&_binary_res_tiles_norm_map_tiles_4bpp_end - (u32)&_binary_res_tiles_norm_map_tiles_4bpp_start) / 32;
	KTSpr spr[4] = {0};
	//kt_TilesetLoad(0, 12, bg_pal_size);
	kt_TilesetLoad(32*2, nmap_tile_size , &_binary_res_tiles_norm_map_tiles_4bpp_start);
	kt_VideoTitleSet("Test program");
	kt_PaletteLoad(0, bg_pal_size, &_binary_res_palettes_bg_data_pal_start);
	//kt_PaletteLoad(SPR_PALETTE_OFS, 16, test_pal_PAL);
	system_Init(15);

	spr[0].pos = SPR_POS(20, 40);
	spr[0].chr = SPR_CHR(0, 0, SIZE_16, SIZE_24, 0);
	spr[0].sfx = SPR_HUE(0xFFFF, 0x00) | SPR_BLEND(0x80);
	spr[0].mat = 0;

	spr[1].pos = SPR_POS(60, 120);
	spr[1].chr = SPR_CHR(0, 0, SIZE_32, SIZE_16, 0);
	spr[1].sfx = SPR_HUE(0x00FF, 0x00);
	spr[1].mat = 0;

	spr[2].pos = SPR_POS(40, 20);
	spr[2].chr = SPR_CHR(12, 0, SIZE_64, SIZE_48, 1);
	spr[2].sfx = SPR_HUE(0x00FF, 0x00);
	spr[2].mat = 0;


	kt_LayerSprite(1, 3, spr);
	kt_LayerMap(0, LAYER_TYPE_MAP_NORMAL, 0, TILEMAP_SIZE_64x64);
	kt_VideoFrameSet(VIDEO_FRAME_2X);

	kt_TilemapSetChr(0, 4, 4, 1, FLIP_NONE, 0);
	kt_TilemapSetChr(0, 5, 4, 1, FLIP_X, 0);
	kt_TilemapSetChr(0, 4, 2, 1, FLIP_XY, 0);
	kt_TilemapSetChr(0, 7, 6, 1, FLIP_NONE, 0);
	for (int i = 0; i < 64;++i) {
		for (int j = 0; j < 64;++j) {
		kt_TilemapSetChr(1, i, j, 1, FLIP_NONE, 0);
		kt_TilemapSetChr(2, i, j, 5, FLIP_NONE, 0);
		kt_TilemapSetChr(3, i, j, 3, FLIP_NONE, 0);
		}
	}


	system_WindowBegin(10, 4, 15);
	system_WindowLabel("HELLO THERE");
	system_WindowLabel("");
	system_WindowLabel("DEMO");
	system_WindowLabel("");
	system_WindowLabel("OPTION");
	system_WindowEnd();
	kt_LayerMap(15, LAYER_TYPE_MAP_NORMAL, 15, TILEMAP_SIZE_64x64);


	while (1) {
		kt_Poll();

		u32 btns = kt_JoyButtonHeld(0);
		x += ((btns >> JOY_BIT_RIGHT) & 0x1) - ((btns >> JOY_BIT_LEFT) & 0x1);
		y += ((btns >> JOY_BIT_DOWN) & 0x1) - ((btns >> JOY_BIT_UP) & 0x1);
		kt_OffsetColor(x - 50, x - 50, y - 50);

		//kt_LayerRect(0, 10, 15, x+30, y+40);
		kt_LayerMapOffset(0, x, y);

		spr[2].pos = SPR_POS(x, y);

		if (kt_JoyButtonHeld(0) & JOY_A) {
			kt_BackColor(0x50, 0x50, 0x50);
		} else {
			kt_BackColor(0x50, 0x20, 0x20);
		}
		kt_Draw();
	}

	return 0;
}
