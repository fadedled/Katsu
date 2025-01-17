

#include <katsu/kt.h>
#include <stdio.h>
#include <math.h>
#include "system.h"
#include "sequencer.h"
#include "samples/keyboard_mouse.h"
#include "samples/sprites.h"
#include "samples/normal_map.h"
#include "samples/line_map.h"
#include "samples/joypad.h"
#include "samples/audio.h"

typedef void (*SampleFunc)(void);
typedef int (*SampleFuncUpd)(void);

void samp_none_init(void);
u32 samp_none_update(void);
void samp_none_deinit(void);


enum SampleId {
	SAMPLE_NONE,
	SAMPLE_SPRITES,
	SAMPLE_NORMALMAP,
	SAMPLE_LINEMAP,
	SAMPLE_KEYBOARDMOUSE,
	SAMPLE_JOYPAD,
	SAMPLE_AUDIO
};

struct SampleCtx {
	SampleFunc init;
	SampleFuncUpd update;
	SampleFunc deinit;
} samples[8] = {
	{samp_none_init, samp_none_update, samp_none_deinit},
	{samp_sprites_init, samp_sprites_update, samp_sprites_deinit},
	{samp_normmap_init, samp_normmap_update, samp_normmap_deinit},
	{samp_linemap_init, samp_linemap_update, samp_linemap_deinit},
	{samp_joypad_init, samp_joypad_update, samp_joypad_deinit},
	{samp_keymouse_init, samp_keymouse_update, samp_keymouse_deinit},
	//{samp_audio_init, samp_audio_update, samp_audio_deinit},
};

/*Demo Palette*/
extern const u8 affine_0_demo_4bpp_pal[];
extern const u8 affine_1_demo_4bpp_pal[];


/*Demo Tiles*/

extern const u8 affine_0_demo_4bpp_data[];
extern const u8 affine_1_demo_4bpp_data[];


extern const u32 affine_0_demo_4bpp_tilenum;
extern const u32 affine_1_demo_4bpp_tilenum;



extern u8 affine_tm_0_data[];
extern u8 affine_tm_1_data[];

extern u8 affine_bg_0_data[];
extern u32 affine_bg_w;
extern u32 affine_bg_h;

static u32 sample_curr = SAMPLE_NONE;
static u32 sample_next = SAMPLE_NONE;
static u32 sample_fade = 0;
static u32 sample_fade_half = 0;


int sample_fade_black(u32 sample, u32 num_frames)
{
	sample_next = sample;
	sample_fade = num_frames & ~1;
	sample_fade_half = num_frames >> 1;
}


int sample_fade_step(void)
{
	//Halfway the sample changes..
	s32 value = --sample_fade - sample_fade_half;
	value = (value < 0 ? -value : value) - sample_fade_half;
	value = (-value * 255) / sample_fade_half;
	kt_OffsetColor(-value, -value, -value);
	if (sample_fade_half == sample_fade) {
		samples[sample_curr].deinit();
		samples[sample_next].init();
		sample_curr = sample_next;
	} else {
	}
}

struct MenuCursor {
	u32 sample;
	KTSpr spr;
} mcur;

void samp_none_init(void)
{
	u32 chr_ofs, pal_ofs;
	system_GetSystemChrOffs(&chr_ofs, &pal_ofs);
	kt_LayerInitMap(KT_LAYER0, KT_LAYER_MAP_NORMAL, KT_TMAP15, KT_MAP_SIZE_64x64);
	kt_LayerInitSprite(KT_LAYER1, 1, KT_TMAP8);
	mcur.sample = SAMPLE_SPRITES;
	mcur.spr.pos = KT_SPR_POS(12, 12);
	mcur.spr.chr = KT_SPR_CHR(128 + chr_ofs, KT_FLIP_NONE, 1, 1, pal_ofs);
	kt_SpriteLoad(KT_TMAP8, 1, &mcur.spr);
	kt_LayerSetMapChrOffset(KT_LAYER0, chr_ofs, pal_ofs);
	system_WindowBegin(4, 4, 30);
	system_WindowLabel("SAMPLES:");
	system_WindowLabel("  Sprite sample");
	system_WindowLabel("  Normal Map sample");
	system_WindowLabel("  Line Map sample");
	system_WindowLabel("  Joypad sample");
	system_WindowLabel("  Keyboard-Mouse sample");
	system_WindowEnd();
}

u32 samp_none_update(void)
{
	u32 chr_ofs, pal_ofs;
	system_GetSystemChrOffs(&chr_ofs, &pal_ofs);
	u32 btn = kt_JoyButtonDown(0);
	if (btn & JOY_STR) {
		sample_fade_black(mcur.sample, 32);
		mcur.spr.chr = KT_SPR_CHR(132 + chr_ofs, KT_FLIP_NONE, 1, 1, pal_ofs);
	} else if (btn & JOY_DOWN) {
		mcur.sample++;

		if (mcur.sample > SAMPLE_AUDIO) {
			mcur.sample = SAMPLE_SPRITES;
		}
	} else if (btn & JOY_UP) {
		mcur.sample--;
		if (mcur.sample < SAMPLE_SPRITES) {
			mcur.sample = SAMPLE_AUDIO;
		}
	}
	mcur.spr.pos = KT_SPR_POS(20, 24 + (mcur.sample << 3));
	kt_SpriteLoad(KT_TMAP8, 1, &mcur.spr);
	return 1;
}


void samp_none_deinit(void)
{
	kt_LayerClearAll();
}


int main()
{
	if (kt_Init()) {
		return 0;
	}
	kt_VideoTitleSet("Katsu Demo");
	kt_VideoFillModeSet(KT_VIDEO_FILL_SCALE);
	kt_VideoFrameSet(KT_VIDEO_FRAME_2X);
	system_Init(KT_TMAP15);

	samp_none_init();
	sample_curr = SAMPLE_NONE;
	while (1) {
		kt_Poll();

		if (sample_fade) {
			sample_fade_step();
			samples[sample_curr].update();
		} else {
			u32 cont = samples[sample_curr].update();
			if (!cont) {
				sample_fade_black(SAMPLE_NONE, 32);
			}
		}

		kt_Draw();
	}

	return 0;
}
