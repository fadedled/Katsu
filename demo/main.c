

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
extern u8 _binary_res_tiles_system_spr_4bpp_start[];
extern u8 _binary_res_tiles_system_spr_4bpp_end[];

extern u8 norm_tm0[];
extern u8 norm_tm1[];

#define TMAP_PALETTE_OFS 0
#define SPR_PALETTE_OFS (4 * 16)



int main() {
	if (kt_Init()) {
		return 0;
	}
	u32 x = 40;
	u32 y = 20;
	u32 bg_pal_size =  (u32)((u32)&_binary_res_palettes_bg_data_pal_end - (u32)&_binary_res_palettes_bg_data_pal_start) / 4;
	u32 nmap_tile_size =  (u32)((u32)&_binary_res_tiles_norm_map_tiles_4bpp_end - (u32)&_binary_res_tiles_norm_map_tiles_4bpp_start) / 32;
	u32 sys_spr_tile_size =  (u32)((u32)&_binary_res_tiles_system_spr_4bpp_end - (u32)&_binary_res_tiles_system_spr_4bpp_start) / 32;
	KTSpr spr[4] = {0};
	//kt_TilesetLoad(0, 12, bg_pal_size);
	kt_TilesetLoad(32*2, nmap_tile_size , &_binary_res_tiles_norm_map_tiles_4bpp_start);
	kt_TilesetLoad(32*2 + nmap_tile_size, sys_spr_tile_size , &_binary_res_tiles_system_spr_4bpp_start);
	kt_VideoTitleSet("Test program");
	kt_PaletteLoad(0, bg_pal_size, &_binary_res_palettes_bg_data_pal_start);
	kt_PaletteLoad(bg_pal_size, 16, &_binary_res_palettes_spr_data_pal_start);
	system_Init(15);

	spr[0].pos = KT_SPR_POS(204, 112);
	spr[0].chr = KT_SPR_CHR(32*2 + nmap_tile_size, 0, KT_SIZE_16, KT_SIZE_16, bg_pal_size / 16);
	spr[0].sfx = 0;
	spr[0].mtx = 0;

	spr[1].pos = KT_SPR_POS(40, 20);
	spr[1].chr = KT_SPR_CHR(12, 0, KT_SIZE_16, KT_SIZE_16, 1);
	spr[1].sfx = KT_SPR_HUE(0x00FF, 0x60);
	spr[1].mtx = 0;

	spr[2].pos = KT_SPR_POS(60, 120);
	spr[2].chr = KT_SPR_CHR(0, 0, KT_SIZE_32, KT_SIZE_16, 0);
	spr[2].sfx = KT_SPR_BLEND(0x80);
	spr[2].mtx = 0;

	spr[3].pos = KT_SPR_POS(40, 20);
	spr[3].chr = KT_SPR_CHR(12, 0, KT_SIZE_24, KT_SIZE_32, 1);
	spr[3].sfx = KT_SPR_HUE(0x00FF, 0x80);
	spr[3].mtx = 0;

	kt_LayerInitSprite(KT_LAYER1, 4, spr);
	kt_VideoFrameSet(KT_VIDEO_FRAME_2X);

	kt_TilemapLoad(0, KT_MAP_SIZE_64x64, 0, 0, 64, 64, 64, norm_tm0);
	kt_TilemapLoad(1, KT_MAP_SIZE_64x64, 0, 0, 64, 64, 64, norm_tm1);

	kt_LayerInitMap(KT_LAYER0, KT_LAYER_MAP_NORMAL, 0, KT_MAP_SIZE_64x64);
	kt_LayerInitMap(KT_LAYER2, KT_LAYER_MAP_NORMAL, 1, KT_MAP_SIZE_64x64);
	kt_LayerSetMapChrOffset(KT_LAYER0, 64, 1);
	kt_LayerSetMapChrOffset(KT_LAYER2, 64, 1);


	kt_LayerInitMap(KT_LAYER15, KT_LAYER_MAP_NORMAL, 15, KT_MAP_SIZE_64x64);
	system_WindowBegin(KT_LAYER10, 4, 15);
	system_WindowLabel("HELLO THERE");
	system_WindowLabel("");
	system_WindowLabel("DEMO");
	system_WindowLabel("");
	system_WindowLabel("OPTION");
	system_WindowEnd();

	KTColor col_a = {0x50, 0x50, 0x50, 0x0};
	KTColor col_b = {0x50, 0x20, 0x20, 0x0};
	u32 x2 = 60, y2 = 120;
	while (1) {
		kt_Poll();

		u32 btns = kt_JoyButtonHeld(0);
		x += ((btns >> JOY_BIT_RIGHT) & 0x1) - ((btns >> JOY_BIT_LEFT) & 0x1);
		y += ((btns >> JOY_BIT_DOWN) & 0x1) - ((btns >> JOY_BIT_UP) & 0x1);

		u32 btns2 = kt_JoyButtonHeld(1);
		x2 += ((btns2 >> JOY_BIT_RIGHT) & 0x1) - ((btns2 >> JOY_BIT_LEFT) & 0x1);
		y2 += ((btns2 >> JOY_BIT_DOWN) & 0x1) - ((btns2 >> JOY_BIT_UP) & 0x1);
		kt_OffsetColor(x - 50, x - 50, y - 50);

		kt_LayerSetMapOffset(KT_LAYER0, x, y);
		kt_LayerSetMapOffset(KT_LAYER2, x, y);

		spr[1].pos = KT_SPR_POS(-x + 307, -y + 234);
		spr[3].pos = KT_SPR_POS(x, y);
		spr[2].pos = KT_SPR_POS(x2, y2);

		if (kt_JoyButtonHeld(0) & JOY_A) {
			kt_BackColor(col_a);
		} else {
			kt_BackColor(col_b);
		}
		kt_Draw();
	}

	return 0;
}
