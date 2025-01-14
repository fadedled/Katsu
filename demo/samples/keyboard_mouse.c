
#include <katsu/kt.h>
#include "../system.h"
#include <stdlib.h>
#include <stdio.h>


#define TEXTBOX_SIZE		2048

u8 *text_box;
KTMouse mouse;
KTSpr *mouse_spr;
u32 line_count;
u32 line_count;
u32 tofs;

struct Caret {
	u32 col;
	u32 row;
	KTSpr *spr;
	u32 timer;
} car;


void samp_keymouse_init(void)
{
	kt_VideoSetResolution(KT_VIDEO_RES_HI);
	KTColor col = {0x80, 0x80, 0x80, 0xFF};
	kt_BackColor(col);
	u8 *text_box = (u8*) malloc(2048);
	line_count = 0;
	car.spr = (KTSpr*) (kt_vram + KT_TMAP8);
	mouse_spr = (KTSpr*) (car.spr + 1);
	u32 pofs;
	system_GetSystemChrOffs(&tofs, &pofs);
	kt_KeyboardSetPolling(KT_ENABLE);
	kt_MouseSetPolling(KT_ENABLE);
	kt_LayerInitMap(KT_LAYER1, KT_LAYER_MAP_NORMAL, KT_TMAP1, KT_MAP_SIZE_128x64);
	kt_LayerSetMapRect(KT_LAYER1, 20, 10, 640, 440);
	kt_LayerSetMapChrOffset(KT_LAYER1, tofs, pofs);
	kt_LayerInitSprite(KT_LAYER2, 2, KT_TMAP8);
	car.spr->pos = KT_SPR_POS((car.col << 3) + 24, (car.row << 3) + 10);
	car.spr->chr = KT_SPR_CHR(tofs+145, KT_FLIP_NONE, KT_SIZE_8, KT_SIZE_8, pofs);

	mouse_spr->pos = KT_SPR_POS(640, 480);
	mouse_spr->chr = KT_SPR_CHR(tofs+139, KT_FLIP_NONE, KT_SIZE_8, KT_SIZE_8, pofs);
	for (int i = 0; i < 64; ++i) {
	for (int j = 0; j < 64; ++j) {
		kt_TilemapSetChr(KT_TMAP1, j, i, ' ', KT_FLIP_NONE, 0);
	}
	}

}

static void __update_textbox_tmap(void) {
}


u32 samp_keymouse_update(void)
{
	KTKeyEvent kev;
	while(kt_KeyboardGetEvent(&kev)) {
		if (kev.type == KEYBOARD_EVTYPE_PRESSED) {
			if(kev.sym < 0x80) {
				if (kev.sym == '\n') {
					car.row++;
				} else {
					kt_TilemapSetChr(KT_TMAP1, car.col, car.row, kev.sym, KT_FLIP_NONE, 0);
					car.timer = 0;
					car.col++;
				}
			} else {
				if (kev.sym == 65293) {
					car.row++;
					car.col = 0;
					car.timer = 0;
				} else if (kev.sym == 65288) {
					car.col--;
					car.timer = 0;
					kt_TilemapSetChr(KT_TMAP1, car.col, car.row, ' ', KT_FLIP_NONE, 0);
				} else if (kev.sym == 65362) {
					car.row--;
					car.timer = 0;
				} else if (kev.sym == 65364) {
					car.row++;
					car.timer = 0;
				} else if (kev.sym == 65361) {
					car.col--;
					car.timer = 0;
				} else if (kev.sym == 65363) {
					car.col++;
					car.timer = 0;
				} else if (kev.sym == 65307) {
					return 0;
				}
				//printf("%d\n", kev.sym);
			}
		}
	}
	car.timer = (car.timer + 1) & 0x3F;
	u32 carspr = (car.timer & 0x20 ? 0 : 145);
	car.spr->pos = KT_SPR_POS((car.col << 3) + 20, (car.row << 3) + 10);
	car.spr->chr = KT_SPR_CHR(tofs + carspr, KT_FLIP_NONE, KT_SIZE_8, KT_SIZE_8, 127);
	kt_MouseGetState(&mouse);
	mouse_spr->pos = KT_SPR_POS(mouse.x, mouse.y);
	return 1;
}


void samp_keymouse_deinit(void)
{
	if (text_box) {
		free(text_box);
	}
	text_box = NULL;
	line_count = 0;
	kt_KeyboardSetPolling(KT_DISABLE);
	kt_MouseSetPolling(KT_DISABLE);
	kt_VideoSetResolution(KT_VIDEO_RES_STD);
	kt_LayerClearAll();
}
