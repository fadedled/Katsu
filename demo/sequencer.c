
#include <katsu/kt.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "sequencer.h"


#define M_PI_M2 (M_PI + M_PI)


struct StepSeq_t {
	u32 is_open;
	u32 num_entries;
	u32 max_voices;
	u32 row;
	u32 row_acc;
	f32 speed;
	f32 volume;
	u32 active_voice;
	u32 entries[32][256];
	f64 acc[256];
} seq;

f32 note_freq[16] = {
	261.63f, 277.18f, 293.66f, 311.13f, 329.63f, 349.23f, 369.99f, 392.0f,
	415.3f, 440.0f, 466.16f, 493.88f, 0.0f, 0.0f, 0.0f, 0.0f
};


static void __sseq_AudioCallback(s16 *stream, u32 n_frames, void *user_data)
{
	if (seq.is_open) {
		for (u32 i = 0; i < n_frames; ++i) {
			s16 val = 0;
			//for (u32 voice = 0; voice < seq.max_voices; ++voice) {
			for (u32 voice = 0; voice < 3; ++voice) {
				u32 is_active = ((seq.active_voice >> voice) & 0x1);
				if (1) {
					u32 octave = 1 << ((seq.entries[voice][seq.row] & 0x70) >> 4);
					f32 note = note_freq[seq.entries[voice][seq.row] & 0xF] * (0.125f * octave);
					seq.acc[voice] += note * M_PI_M2 / KT_AUDIO_SAMPLE_RATE;
					if (seq.acc[voice] >= M_PI_M2)
						seq.acc[voice] -= M_PI_M2;
					//Convert to 16bit stream
					f64 func = MIN(MAX(sin(seq.acc[voice]) * 16.0, -1.0), 1.0);
					val += func * seq.volume * 32767.0f * 0.04f;
				}
			}

			for (u32 ch = 0; ch < KT_AUDIO_CHANNELS; ch++) {
				*stream++ = val;
			}
		}
		seq.row_acc += n_frames * seq.speed;
		if (seq.row_acc > 2048 * 8) {
			seq.row = (seq.row + 1) & 0x1f;
			seq.row_acc = 0;
		}
	}
}





void sseq_Open(u32 max_voices, f32 volume)
{
	if (!seq.is_open) {
		seq.is_open = KT_TRUE;
		seq.num_entries = 32;
		seq.max_voices = max_voices;

		sseq_SetVolume(volume);

		seq.entries[0][0] = 0x20;
		seq.entries[0][1] = 0x24;
		seq.entries[0][2] = 0x2B;
		seq.entries[0][3] = 0x27;
		seq.entries[0][4] = 0x20;
		seq.entries[0][5] = 0x24;
		seq.entries[0][6] = 0x2B;
		seq.entries[0][7] = 0x27;
		seq.entries[0][8] = 0x20;
		seq.entries[0][9] = 0x24;
		seq.entries[0][10] = 0x2B;
		seq.entries[0][11] = 0x27;
		seq.entries[0][12] = 0x20;
		seq.entries[0][13] = 0x24;
		seq.entries[0][14] = 0x2B;
		seq.entries[0][15] = 0x27;
		seq.entries[0][16] = 0x22;
		seq.entries[0][17] = 0x25;
		seq.entries[0][18] = 0x30;
		seq.entries[0][19] = 0x29;
		seq.entries[0][20] = 0x22;
		seq.entries[0][21] = 0x25;
		seq.entries[0][22] = 0x30;
		seq.entries[0][23] = 0x29;
		seq.entries[0][24] = 0x22;
		seq.entries[0][25] = 0x25;
		seq.entries[0][26] = 0x30;
		seq.entries[0][27] = 0x28;
		seq.entries[0][28] = 0x22;
		seq.entries[0][29] = 0x25;
		seq.entries[0][30] = 0x30;
		seq.entries[0][31] = 0x28;


		seq.entries[1][0] = 0x42;
		seq.entries[1][1] = 0x40;
		seq.entries[1][2] = 0x42;
		seq.entries[1][3] = 0x47;
		seq.entries[1][4] = 0x44;
		seq.entries[1][5] = 0x44;
		seq.entries[1][6] = 0x44;
		seq.entries[1][7] = 0x40;
		seq.entries[1][8] = 0x42;
		seq.entries[1][9] = 0x40;
		seq.entries[1][10] = 0x42;
		seq.entries[1][11] = 0x47;
		seq.entries[1][12] = 0x44;
		seq.entries[1][13] = 0x44;
		seq.entries[1][14] = 0x42;
		seq.entries[1][15] = 0x44;
		seq.entries[1][16] = 0x45;
		seq.entries[1][17] = 0x44;
		seq.entries[1][18] = 0x45;
		seq.entries[1][19] = 0x49;
		seq.entries[1][20] = 0x45;
		seq.entries[1][21] = 0x45;
		seq.entries[1][22] = 0x45;
		seq.entries[1][23] = 0x45;
		seq.entries[1][24] = 0x45;
		seq.entries[1][25] = 0x45;
		seq.entries[1][26] = 0x44;
		seq.entries[1][27] = 0x44;
		seq.entries[1][28] = 0x42;
		seq.entries[1][29] = 0x42;
		seq.entries[1][30] = 0x42;
		seq.entries[1][31] = 0x42;

		seq.entries[2][0] = 0x4f;
		seq.entries[2][1] = 0x4f;
		seq.entries[2][2] = 0x4f;
		seq.entries[2][3] = 0x4f;
		seq.entries[2][4] = 0x4f;
		seq.entries[2][5] = 0x4f;
		seq.entries[2][6] = 0x4f;
		seq.entries[2][7] = 0x4f;
		seq.entries[2][8] = 0x4f;
		seq.entries[2][9] = 0x4f;
		seq.entries[2][10] = 0x4f;
		seq.entries[2][11] = 0x4f;
		seq.entries[2][12] = 0x4f;
		seq.entries[2][13] = 0x30;
		seq.entries[2][14] = 0x32;
		seq.entries[2][15] = 0x34;
		seq.entries[2][16] = 0x35;
		seq.entries[2][17] = 0x35;
		seq.entries[2][18] = 0x35;
		seq.entries[2][19] = 0x35;
		seq.entries[2][20] = 0x35;
		seq.entries[2][21] = 0x35;
		seq.entries[2][22] = 0x35;
		seq.entries[2][23] = 0x35;
		seq.entries[2][24] = 0x34;
		seq.entries[2][25] = 0x34;
		seq.entries[2][26] = 0x34;
		seq.entries[2][27] = 0x4f;
		seq.entries[2][28] = 0x4f;
		seq.entries[2][29] = 0x4f;
		seq.entries[2][30] = 0x4f;
		seq.entries[2][31] = 0x4f;

		//TODO: use locking
		kt_AudioSetCallback(__sseq_AudioCallback, NULL);
	}
}


void sseq_Close(void)
{
	if (seq.is_open) {
		//TODO: use locking
		kt_AudioSetCallback(NULL, NULL);

		seq.is_open = KT_FALSE;
		seq.max_voices = 0;
		seq.speed = 0.0f;
		sseq_SetVolume(0.0f);
	}
}

void sseq_SetSpeed(f32 speed)
{
	seq.speed = speed;
}

void sseq_SetVolume(f32 volume)
{
	seq.volume = volume;
}

f32  sseq_GetSpeed(void)
{
	return seq.speed;
}

f32  sseq_GetVolume(void)
{
	return seq.volume;
}

u32 sseq_SetButton(u32 voice, u32 entrie, u32 state)
{

	return -1;
}
