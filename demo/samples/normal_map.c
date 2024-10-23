
#include <katsu/kt.h>

extern const u8 norm_demo_4bpp_data[];
extern const u32 norm_demo_4bpp_tilenum;
extern const u8 norm_demo_4bpp_pal[];

#define BG_TILES_START			1024
#define BG_PALETTE_START		32

extern u8 norm_tm_0_data[];
extern u8 norm_tm_1_data[];

KTSpr *spr;

struct Player {
	u32 x;
	u32 y;
} ply;

void samp_normmap_init(void)
{
	kt_TilesetLoad(BG_TILES_START, norm_demo_4bpp_tilenum, norm_demo_4bpp_data);
	kt_PaletteLoad(BG_PALETTE_START, KT_PAL_SIZE_16, norm_demo_4bpp_pal);


	kt_TilemapLoad(KT_TMAP0, KT_MAP_SIZE_64x64, 0, 0, 64, 64, 64, norm_tm_0_data);
	kt_TilemapLoad(KT_TMAP1, KT_MAP_SIZE_64x64, 0, 0, 64, 64, 64, norm_tm_1_data);

	spr = (KTSpr*) (kt_vram + KT_TMAP8);
	kt_LayerInitMap(KT_LAYER0, KT_LAYER_MAP_NORMAL, KT_TMAP0, KT_MAP_SIZE_64x64);
	kt_LayerInitMap(KT_LAYER2, KT_LAYER_MAP_NORMAL, KT_TMAP1, KT_MAP_SIZE_64x64);
	kt_LayerSetMapChrOffset(KT_LAYER0, BG_TILES_START, BG_PALETTE_START);
	kt_LayerSetMapChrOffset(KT_LAYER2, BG_TILES_START, BG_PALETTE_START);
}


void samp_normmap_update(void)
{
	u32 btn = kt_JoyButtonHeld(0);
	s32 dx = ((btn >> JOY_BIT_RIGHT)  & 1) - ((btn >> JOY_BIT_LEFT) & 1);
	s32 dy = ((btn >> JOY_BIT_DOWN)  & 1) - ((btn >> JOY_BIT_UP) & 1);
	s32 mult = (dx & dy ? 2 : 3);
	ply.x += mult * dx;
	ply.y += mult * dy;
	kt_LayerSetMapOffsetf(KT_LAYER0, ply.x, ply.y, 1);
	kt_LayerSetMapOffsetf(KT_LAYER2, ply.x, ply.y, 1);

}


void samp_normmap_deinit(void)
{
	kt_LayerClearAll();
}
