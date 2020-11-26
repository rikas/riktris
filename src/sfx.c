#define _CRT_SECURE_NO_WARNINGS // for VS compiler hapiness
#include "sfx.h"

SFX *sfx;

static void load_sample(Sound **sound, int id, char *name)
{
	char filename[32];
	struct Sound *new_sound = (Sound *)malloc(sizeof(struct Sound));

	new_sound->id = id;

	sprintf(filename, "data/sfx/%s.ogg", name);

	new_sound->sample = al_load_sample(filename);
	must_init(new_sound->sample, filename);

	*sound = new_sound;
}

void init_sfx()
{
	int index = 0;

	sfx = (SFX *)malloc(sizeof(SFX));

	load_sample(&sfx->samples[index++], SOUND_ROTATE, "rotate2");
	load_sample(&sfx->samples[index++], SOUND_HARD_DROP, "hard_drop2");
	load_sample(&sfx->samples[index++], SOUND_LINE_1, "complete_line_1");
	load_sample(&sfx->samples[index++], SOUND_LINE_2, "complete_line_2");
	load_sample(&sfx->samples[index++], SOUND_LINE_3, "complete_line_3");
	load_sample(&sfx->samples[index++], SOUND_ERROR, "error");
	load_sample(&sfx->samples[index++], SOUND_HOLD, "error");
	load_sample(&sfx->samples[index++], SOUND_MOVE, "move");
}

void destroy_sfx()
{
}

void sfx_play(int id, float speed)
{
	for (int i = SOUND_ROTATE; i < SOUND_MAX; i++)
	{
		if (sfx->samples[i]->id == id)
		{
			al_play_sample(sfx->samples[i]->sample, 1.0, 0.0, speed, ALLEGRO_PLAYMODE_ONCE, NULL);
			break;
		}
	}
}
