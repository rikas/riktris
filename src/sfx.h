#ifndef __SFX_H
#define __SFX_H

#include <allegro5/allegro5.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "utils.h"

enum
{
	SOUND_ROTATE,
	SOUND_HARD_DROP,
	SOUND_MAX // just for control
};

// Stores the unique ID (int) and the ALLEGRO_SAMPLE for a game sfx.
typedef struct Sound
{
	int id;
	ALLEGRO_SAMPLE *sample;
} Sound;

// Stores sound information like volume, but also stores in memory all the samples.
typedef struct SFX
{
	Sound *samples[100];
	int volume;
} SFX;

extern SFX *sfx;

void init_sfx(void);
void destroy_sfx(void);
void sfx_play(int id);
#endif
