
#include <katsu/kt.h>
#include <stdio.h>

extern const u8 sprite_kap_demo_4bpp_data[];
extern const u32 sprite_kap_demo_4bpp_tilenum;
extern const u8 sprite_kap_demo_4bpp_pal[];

extern const u8 sprite1_demo_4bpp_data[];
extern const u32 sprite1_demo_4bpp_tilenum;
extern const u8 sprite_demo_4bpp_pal[];

struct SpriteValues {
	u16 hue;
	u8 blend;
	u8 flip;
	f32 mtx[4];
};

typedef struct AnimFrame_tag {
	u32 tile_ofs;
	u32 chr_const;
} AnimFrame;

typedef struct Anim_tag {
	u32 num_frames;
	const AnimFrame *frames;
} Anim;

const AnimFrame kappa_down_walk_frames[] = {
	{ 0, KT_SPR_CHR(0, KT_FLIP_NONE, KT_SIZE_16, KT_SIZE_24, 0)},
	{ 6, KT_SPR_CHR(0, KT_FLIP_NONE, KT_SIZE_16, KT_SIZE_24, 0)},
	{12, KT_SPR_CHR(0, KT_FLIP_NONE, KT_SIZE_16, KT_SIZE_24, 0)},
	{18, KT_SPR_CHR(0, KT_FLIP_NONE, KT_SIZE_16, KT_SIZE_24, 0)},
	{ 0, KT_SPR_CHR(0, KT_FLIP_X, KT_SIZE_16, KT_SIZE_24, 0)},
	{ 6, KT_SPR_CHR(0, KT_FLIP_X, KT_SIZE_16, KT_SIZE_24, 0)},
	{12, KT_SPR_CHR(0, KT_FLIP_X, KT_SIZE_16, KT_SIZE_24, 0)},
	{18, KT_SPR_CHR(0, KT_FLIP_X, KT_SIZE_16, KT_SIZE_24, 0)}
};


const AnimFrame bird_walk_frames[] = {
	{  0, KT_SPR_CHR(0, KT_FLIP_NONE, KT_SIZE_40, KT_SIZE_40, 0)},
	{ 25, KT_SPR_CHR(0, KT_FLIP_NONE, KT_SIZE_40, KT_SIZE_40, 0)},
	{ 50, KT_SPR_CHR(0, KT_FLIP_NONE, KT_SIZE_40, KT_SIZE_40, 0)},
	{ 75, KT_SPR_CHR(0, KT_FLIP_NONE, KT_SIZE_40, KT_SIZE_40, 0)},
	{100, KT_SPR_CHR(0, KT_FLIP_NONE, KT_SIZE_40, KT_SIZE_40, 0)},
	{125, KT_SPR_CHR(0, KT_FLIP_NONE, KT_SIZE_40, KT_SIZE_40, 0)},
	{150, KT_SPR_CHR(0, KT_FLIP_NONE, KT_SIZE_40, KT_SIZE_40, 0)},
};

const Anim kappa_down_walk_anim = {
	.num_frames = 8,
	.frames = kappa_down_walk_frames
};

const Anim bird_walk_anim = {
	.num_frames = 7,
	.frames = bird_walk_frames
};

const struct SpriteValues init_sprite = {
	.hue = 0,
	.blend = 0,
	.flip = KT_FLIP_NONE,
	.mtx = {1.0, 0.0, 0.0, 1.0},
};

u32 anim_sprite_frame = 0;
const u32 anim_timer = 4;
u32 anim_sprite_time = 4;

KTSpr sprites[8];

struct SpriteValues current_spr;

#define KAPPA_TILES_START			32

void samp_sprites_init(void)
{
	for (u32 i = 0; i < 8; ++i) {
		sprites[i].pos = 0;
		sprites[i].chr = 0;
		sprites[i].sfx = 0;
		sprites[i].mtx = 0;
	}

	current_spr = init_sprite;
	kt_LayerInitSprite(KT_LAYER1, 8, KT_TMAP8);
	kt_SpriteLoad(KT_TMAP8, 8, sprites);
	anim_sprite_frame = 0;

#if 0
	kt_TilesetLoad(KAPPA_TILES_START, sprite_kap_demo_4bpp_tilenum, sprite_kap_demo_4bpp_data);
	kt_PaletteLoad(0, KT_PAL_SIZE_16, sprite_kap_demo_4bpp_pal);
#else
	kt_TilesetLoad(KAPPA_TILES_START, sprite_kap_demo_4bpp_tilenum, sprite_kap_demo_4bpp_data);
	kt_PaletteLoad(0, KT_PAL_SIZE_16, sprite_kap_demo_4bpp_pal);
#endif

}

void anim_Update(KTSpr *spr, const Anim *anim) {
	if (!anim_sprite_time--) {
		anim_sprite_frame = (anim_sprite_frame + 1 >= anim->num_frames ? 0 : anim_sprite_frame + 1);
		anim_sprite_time = anim_timer;
	}
	spr->chr = anim->frames[anim_sprite_frame].chr_const | ((anim->frames[anim_sprite_frame].tile_ofs + KAPPA_TILES_START) & 0x3FFF);
}

u32 samp_sprites_update(void)
{
	anim_Update(&sprites[0], &kappa_down_walk_anim);
	sprites[0].pos = KT_SPR_POS(32, 32);
	kt_SpriteLoad(KT_TMAP8, 8, sprites);
	return (~kt_JoyButtonDown(0) >> JOY_BIT_STR) & 0x1;
}


void samp_sprites_deinit(void)
{
	kt_LayerClearAll();
}
