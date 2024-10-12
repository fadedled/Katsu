

#include <katsu/kt.h>
#include <stdio.h>
#include <math.h>
#include "system.h"
#include "sequencer.h"



/*Demo Palette*/
extern const u8 norm_demo_4bpp_pal[];
extern const u8 affine_0_demo_4bpp_pal[];
extern const u8 affine_1_demo_4bpp_pal[];
extern const u8 system_4bpp_pal[];


/*Demo Tiles*/
extern const u8 norm_demo_4bpp_data[];
extern const u8 affine_0_demo_4bpp_data[];
extern const u8 affine_1_demo_4bpp_data[];
extern const u8 system_4bpp_data[];

extern const u32 norm_demo_4bpp_tilenum;
extern const u32 affine_0_demo_4bpp_tilenum;
extern const u32 affine_1_demo_4bpp_tilenum;
extern const u32 system_4bpp_tilenum;

extern u8 norm_tm_0_data[];
extern u8 norm_tm_1_data[];
extern u8 affine_tm_0_data[];
extern u8 affine_tm_1_data[];

extern u8 affine_bg_0_data[];
extern u32 affine_bg_w;
extern u32 affine_bg_h;


#define TMAP_PALETTE_OFS 0
#define SPR_PALETTE_OFS (4 * 16)


KTLineMapEntry lmap_data[256] = {0};

void demo_AffineSetup(void)
{
	kt_LayerClear(KT_LAYER0);
	kt_LayerClear(KT_LAYER1);
	kt_LayerClear(KT_LAYER3);
	//Load tilesets
	kt_TilesetLoad(system_4bpp_tilenum, affine_0_demo_4bpp_tilenum, affine_0_demo_4bpp_data);
	kt_TilesetLoad(affine_0_demo_4bpp_tilenum + system_4bpp_tilenum, affine_1_demo_4bpp_tilenum , affine_1_demo_4bpp_data);
	kt_PaletteLoad(2, KT_PAL_SIZE_16, affine_0_demo_4bpp_pal);
	kt_PaletteLoad(3, KT_PAL_SIZE_16, affine_1_demo_4bpp_pal);

	//Setup tilemap
	kt_TilemapLoad(0, KT_MAP_SIZE_64x64, 0, 0, affine_bg_w, affine_bg_h, affine_bg_w, affine_bg_0_data);
	kt_TilemapLoad(0, KT_MAP_SIZE_64x64, 32, 0, affine_bg_w, affine_bg_h, affine_bg_w, affine_bg_0_data);
	kt_TilemapLoad(1, KT_MAP_SIZE_64x64, 0, 0, 64, 64, 64, affine_tm_0_data);
	kt_TilemapLoad(2, KT_MAP_SIZE_64x64, 0, 0, 64, 64, 64, affine_tm_1_data);

	//Generate linemap data
	f32 f = 1024;
	f32 d = 32;
	for (s32 i = 0; i < 256; i++) {
		s32 y_d = (s32) (sinf(i * (3.14159f / 32.0f)) * 1024.0f*8);
		lmap_data[i].ofs_delta = (y_d & 0xFFFFFu);
		lmap_data[i].scale_x_delta = 0;
		f -= d;
		d -= 0.5;
	}

	for (s32 i = 100; i < 256; i++) {
		f32 y_d = (i*128*3);
		lmap_data[i].ofs_delta = ((i*4) & 0xFFFFFu) | (((s32)(y_d) & 0xFFC00) << 10); //((s32) (sinf(i * (3.14159f / 32.0f)) * 1024.0f*4)) & 0xFFFFFu;
		lmap_data[i].scale_x_delta = ((s32)(-i*3) & 0xFFFFu) | (((s32)(y_d) & 0x3FF) << 16);
		f -= d;
		d -= 0.5;
	}

	//Setup layers
	kt_LayerInitMap(KT_LAYER0, KT_LAYER_MAP_NORMAL, 0, KT_MAP_SIZE_64x64);
	kt_LayerInitMap(KT_LAYER1, KT_LAYER_MAP_NORMAL, 1, KT_MAP_SIZE_64x64);
	kt_LayerInitMap(KT_LAYER3, KT_LAYER_MAP_NORMAL, 2, KT_MAP_SIZE_64x64);
	kt_LayerSetBlendMode(KT_LAYER3, KT_BL_SRC_ALPHA, KT_BL_INV_SRC_ALPHA, KT_BL_FUNC_ADD);
	kt_LayerSetMapBlend(KT_LAYER3, KT_TRUE, 0x80);
	kt_LayerSetMapRect(KT_LAYER0, 0, 0, KT_VIDEO_STD_WIDTH, affine_bg_h * 8);
	kt_LayerSetMapRect(KT_LAYER1, 0, affine_bg_h * 8, KT_VIDEO_STD_WIDTH, KT_VIDEO_STD_HEIGHT - (affine_bg_h * 8));
	kt_LayerSetMapRect(KT_LAYER3, 0, (affine_bg_h * 8) - 2, KT_VIDEO_STD_WIDTH, KT_VIDEO_STD_HEIGHT - (affine_bg_h * 8));
	kt_LayerSetMapChrOffset(KT_LAYER0, system_4bpp_tilenum, 2);
	kt_LayerSetMapChrOffset(KT_LAYER1, system_4bpp_tilenum, 2);
	kt_LayerSetMapChrOffset(KT_LAYER3, system_4bpp_tilenum, 2);
	kt_LayerSetUserData(KT_LAYER1, 256, lmap_data);
	kt_LayerSetUserData(KT_LAYER3, 256, lmap_data);
}


KTSpr sys_spr[8] = {0};

int main() {
	if (kt_Init()) {
		return 0;
	}
	u32 x = 40;
	u32 y = 20;

	KTSpr spr[4] = {0};
	//kt_TilesetLoad(0, 12, bg_pal_size);
	kt_TilesetLoad(0, system_4bpp_tilenum , system_4bpp_data);
	kt_TilesetLoad(system_4bpp_tilenum, norm_demo_4bpp_tilenum , norm_demo_4bpp_data);
	kt_VideoTitleSet("Test program");
	kt_PaletteLoad(0, KT_PAL_SIZE_16, system_4bpp_pal);
	kt_PaletteLoad(1, KT_PAL_SIZE_16, norm_demo_4bpp_pal);
	kt_PaletteLoad(2, KT_PAL_SIZE_16, affine_0_demo_4bpp_pal);
	kt_PaletteLoad(3, KT_PAL_SIZE_16, affine_1_demo_4bpp_pal);
	system_Init(15);
	kt_VideoFillModeSet(KT_VIDEO_FILL_SCALE);
	kt_VideoFrameSet(KT_VIDEO_FRAME_2X);

	spr[0].pos = KT_SPR_POS(204, 112);
	spr[0].chr = KT_SPR_CHR(32*2 + norm_demo_4bpp_tilenum, 0, KT_SIZE_16, KT_SIZE_16, 1);
	spr[0].sfx = 0;
	spr[0].mtx = KT_MTX_IDENTITY;

	spr[1].pos = KT_SPR_POS(40, 20);
	spr[1].chr = KT_SPR_CHR(12, 0, KT_SIZE_16, KT_SIZE_16, 1);
	spr[1].sfx = KT_SPR_HUE(0x00FF, 0x60);
	spr[1].mtx = KT_MTX_IDENTITY;

	spr[3].pos = KT_SPR_POS(40, 20);
	spr[3].chr = KT_SPR_CHR(12, 0, KT_SIZE_24, KT_SIZE_32, 1);
	spr[3].sfx = KT_SPR_HUE(0x00FF, 0x80);
	spr[3].mtx = KT_MTX_IDENTITY;

	u32 pointer_indx = 0;

	sys_spr[0].pos = KT_SPR_POS(10*8-8, 4*8 +4 + (pointer_indx*8));
	sys_spr[0].chr = KT_SPR_CHR(128, KT_FLIP_NONE, KT_SIZE_16, KT_SIZE_16, 0);
	sys_spr[0].sfx = 0;
	sys_spr[0].mtx = KT_MTX_IDENTITY;

	kt_MtxSet(1, 2.3, 0.0, 0.6, -2.3);


	kt_LayerInitSprite(KT_LAYER2, 4, spr);

	kt_TilemapLoad(0, KT_MAP_SIZE_64x64, 0, 0, 64, 64, 64, norm_tm_0_data);
	kt_TilemapLoad(1, KT_MAP_SIZE_64x64, 0, 0, 64, 64, 64, norm_tm_1_data);

	kt_LayerInitMap(KT_LAYER6, KT_LAYER_MAP_NORMAL, 15, KT_MAP_SIZE_64x64);
	kt_LayerInitSprite(KT_LAYER7, 2, sys_spr);
	system_WindowBegin(10, 4, 32);
	system_WindowLabel("- Normal Map Demo");
	system_WindowLabel("- Sprite Demo");
	system_WindowLabel("- ");
	system_WindowLabel("");
	system_WindowLabel("OPTION");
	system_WindowEnd();

	//KTColor col_a = {0x50, 0x50, 0x50, 0x0};
	//KTColor col_b = {0x50, 0x20, 0x20, 0x0};
	u32 x2 = 60, y2 = 120;
	f32 rot = 0.0;

	u32 voice = 0, entrie = 0;
	//sseq_Open(10, 1.0);
	//sseq_SetSpeed(1.0);

	demo_AffineSetup();
	KTMouse mouse;
	KTKeyEvent kev;
	//kt_KeyboardEnablePolling(1);
	//kt_MouseEnablePolling(1);
	while (1) {
		kt_Poll();

		while (kt_KeyboardGetEvent(&kev)) {
			if (kev.type == KEYBOARD_EVTYPE_PRESSED) {
				//Print the UTF
				if(kev.sym > 0x7FF) {
					printf("%c%c%c", ((kev.sym >> 12) & 0xF) | 0xE0, (kev.sym >> 6 & 0x3F) | 0x80, (kev.sym & 0x3F) | 0x80);
				} else if(kev.sym > 0x7F) {
					printf("%c%c", ((kev.sym >> 6) & 0x1F) | 0xC0, (kev.sym & 0x3F) | 0x80);
				} else {
					printf("%c", kev.sym);
				}
				fflush(stdout);
			}
		}
		kt_MouseGetState(&mouse);

		u32 btns = kt_JoyButtonHeld(0);
		x += ((btns >> JOY_BIT_RIGHT) & 0x1) - ((btns >> JOY_BIT_LEFT) & 0x1) + mouse.scroll;
		y += ((btns >> JOY_BIT_DOWN) & 0x1) - ((btns >> JOY_BIT_UP) & 0x1);

		u32 btns2 = kt_JoyButtonHeld(1);
		x2 += ((btns2 >> JOY_BIT_RIGHT) & 0x1) - ((btns2 >> JOY_BIT_LEFT) & 0x1);
		y2 += ((btns2 >> JOY_BIT_DOWN) & 0x1) - ((btns2 >> JOY_BIT_UP) & 0x1);
		//kt_OffsetColor(x - 50, x - 50, y - 50);
		kt_LayerSetMapOffset(KT_LAYER0, x >> 3 , 0);
		kt_LayerSetMapOffset(KT_LAYER1, x >> 1 , y >> 1);
		kt_LayerSetMapOffset(KT_LAYER3, x, y);
		//kt_LayerSetMapOffsetf(KT_LAYER2, x, y, 1);

		spr[1].pos = KT_SPR_POS(mouse.x, mouse.y);
		spr[3].pos = KT_SPR_POS(x, y);
		spr[2].pos = KT_SPR_POS(x2, y2);

		f32 rot_inc = ((btns2 >> JOY_BIT_R) & 0x1) - ((btns2 >> JOY_BIT_L) & 0x1);
		rot += rot_inc / 256.0;
		kt_MtxSetRotoscale(1, 8, 8, 0.31);

		if (kt_JoyButtonDown(0) & JOY_DOWN) {
			pointer_indx = (pointer_indx + 1) & 3;
		} else if (kt_JoyButtonDown(0) & JOY_UP) {
			pointer_indx = (pointer_indx - 1) & 3;
		}

		if (kt_JoyButtonDown(0) & JOY_DOWN) {
			voice = (voice + 1) & 0xf;
		} else if (kt_JoyButtonDown(0) & JOY_UP) {
			voice = (voice - 1) & 0xf;
		}
		if (kt_JoyButtonDown(0) & JOY_RIGHT) {
			entrie = (entrie + 1) & 0x1f;
		} else if (kt_JoyButtonDown(0) & JOY_LEFT) {
			entrie = (entrie - 1) & 0x1f;
		}

		if (kt_JoyButtonDown(0) & JOY_STR) {
			sseq_SetButton(voice, entrie, 1);
		}

		sys_spr[0].pos = KT_SPR_POS(10*8-8, 4*8 +4 + (pointer_indx*8));

		if (kt_JoyButtonHeld(0) & JOY_A) {
			sys_spr[0].chr = KT_SPR_CHR(132, KT_FLIP_NONE, KT_SIZE_16, KT_SIZE_16, 0);
		} else {
			sys_spr[0].chr = KT_SPR_CHR(128, KT_FLIP_NONE, KT_SIZE_16, KT_SIZE_16, 0);
		}
		kt_Draw();
	}

	return 0;
}
