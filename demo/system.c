
#include <katsu/kt.h>
#include "system.h"

extern const u8 system_4bpp_pal[];
extern const u8 system_4bpp_data[];
extern const u32 system_4bpp_tilenum;

u32 sys_tmap;
u32 win_x;
u32 win_y;
u32 win_width;
u32 sys_pal_ofs;
u32 sys_chr_ofs;


struct Menu {
	u32 size;
	s32 cursor_pos;
	u32 x, y;
} menu;

void system_WindowMenuInit(const char *title, u32 x, u32 y, u32 width, const char **elems, u32 num_elems, KTSpr *cursor)
{
	cursor->pos = KT_SPR_POS(x*8, (x*8) + 2);
	cursor->chr = KT_SPR_CHR(128 + sys_chr_ofs, KT_FLIP_NONE, 1, 1, sys_pal_ofs);
	menu.cursor_pos = 0;
	menu.size = num_elems;
	menu.x = x;
	menu.y = y;
	//Draw
	system_WindowBegin(x+1, y, width / 2);
	system_WindowLabel(title);
	system_WindowEnd();
	system_WindowBegin(x, y+2, width);
	for (u32 i = 0; i < num_elems; ++i) {
		system_WindowLabel(elems[i]);
		system_WindowLabel("");
	}
	system_WindowEnd();
}

u32 system_WindowMenuUpdate(u32 was_selected, KTSpr *cursor)
{
	u32 tile_id = 128 + ((was_selected ? 1 : 0) * 4);
	if (!was_selected) {
		u32 btn = kt_JoyButtonDown(0);
		if (btn & JOY_DOWN) {
			menu.cursor_pos = (menu.cursor_pos >= menu.size - 1 ? 0 : menu.cursor_pos + 1);
		} else if (btn & JOY_UP) {
			menu.cursor_pos = (menu.cursor_pos <= 0 ? menu.size - 1 : menu.cursor_pos - 1);
		}
	}
	cursor->chr = KT_SPR_CHR(tile_id + sys_chr_ofs, KT_FLIP_NONE, 1, 1, sys_pal_ofs);
	cursor->pos = KT_SPR_POS(menu.x*8 + 7, (menu.y*8) + 20 + (menu.cursor_pos * 16));
	return menu.cursor_pos;
}



void system_Init(u32 tmap)
{
	sys_chr_ofs = KT_MAX_TILES - system_4bpp_tilenum;
	sys_pal_ofs = 127;
	kt_TilesetLoad(sys_chr_ofs, system_4bpp_tilenum , system_4bpp_data);
	kt_PaletteLoad(sys_pal_ofs, KT_PAL_SIZE_16, system_4bpp_pal);
	sys_tmap = tmap;
}

void system_GetSystemChrOffs(u32 *chr_ofs, u32 *pal_ofs) {
	*chr_ofs = sys_chr_ofs;
	*pal_ofs = sys_pal_ofs;
}

void system_Clear()
{
	for (u32 y = 0; y < 30; y++) {
		for (u32 x = 0; x < 54; x++) {
			kt_TilemapSetChr(sys_tmap, x, y, 0, KT_FLIP_NONE, 0);
		}
	}

}

u32 system_WindowBegin(u32 x, u32 y, u32 width)
{
	win_x = x;
	win_y = y;
	win_width = width;
	if (!win_width)
		return 0;

	kt_TilemapSetChr(sys_tmap, win_x, win_y, 3, KT_FLIP_Y, 0);
	for (u32 i = 1; i < win_width; ++i) {
		kt_TilemapSetChr(sys_tmap, win_x + i, win_y, 4, KT_FLIP_Y, 0);
	}
	kt_TilemapSetChr(sys_tmap, win_x + win_width, win_y, 3, KT_FLIP_XY, 0);
	win_y++;
	return 1;
}



u32 system_WindowLabel(const char *str)
{
	if (!win_width)
		return 0;
	kt_TilemapSetChr(sys_tmap, win_x, win_y, 5, KT_FLIP_NONE, 0);
	u32 str_end = 0;
	for (u32 i = 1; i < win_width; ++i) {
		if (!str_end) {
			if (*str == '\0') {
				kt_TilemapSetChr(sys_tmap, win_x + i, win_y, ' ', KT_FLIP_NONE, 0);
				str_end = 1;
			} else {
				kt_TilemapSetChr(sys_tmap, win_x + i, win_y, *str, KT_FLIP_NONE, 0);
			}
			str++;
		} else {
			kt_TilemapSetChr(sys_tmap, win_x + i, win_y, ' ', KT_FLIP_NONE, 0);
		}
	}
	kt_TilemapSetChr(sys_tmap, win_x + win_width, win_y, 5, KT_FLIP_X, 0);
	win_y++;
	return 1;
}

u32 system_WindowEnd()
{
	if (!win_width)
		return 0;
	kt_TilemapSetChr(sys_tmap, win_x, win_y, 3, KT_FLIP_NONE, 0);
	for (u32 i = 1; i< win_width; ++i) {
		kt_TilemapSetChr(sys_tmap, win_x + i, win_y, 4, KT_FLIP_NONE, 0);
	}
	kt_TilemapSetChr(sys_tmap, win_x + win_width, win_y, 3, KT_FLIP_X, 0);
	win_width = 0;
	return 1;
}
