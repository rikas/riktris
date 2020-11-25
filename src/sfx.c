#include "sfx.h"

SFX *sfx;

static void load_sample(Sound **sound, int id, char *name)
{
  char filename[32];
  struct Sound *new_sound = (Sound *)malloc(sizeof(struct Sound));

  new_sound->id = id;

  sprintf(filename, "sfx/%s.wav", name);

  new_sound->sample = al_load_sample(filename);
  must_init(new_sound->sample, filename);

  *sound = new_sound;
}

void init_sfx()
{
  int index = 0;

  sfx = (SFX *)malloc(sizeof(SFX));

  load_sample(&sfx->samples[index++], SOUND_ROTATE, "rotate");
  load_sample(&sfx->samples[index++], SOUND_HARD_DROP, "hard_drop");
}

void destroy_sfx()
{
}

void sfx_play(int id)
{
  for (int i = SOUND_ROTATE; i < SOUND_MAX; i++)
  {
    if (sfx->samples[i]->id == id)
    {
      al_play_sample(sfx->samples[i]->sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
      break;
    }
  }
}
