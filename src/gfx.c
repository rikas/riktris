#define _CRT_SECURE_NO_WARNINGS // for VS compiler hapiness
#include "gfx.h"

GFX *gfx;

static char *mino_names[] = {"t", "s", "z", "i", "j", "l", "o", 0};

static void load_mino_bitmaps()
{
	for (int i = 0; mino_names[i]; i++)
	{
		char filename[32];

		sprintf(filename, "data/gfx/mino_%s.png", mino_names[i]);
		gfx->minos[i] = al_load_bitmap(filename);
		must_init(gfx->minos[i], filename);

		sprintf(filename, "data/gfx/mino_ghost_%s.png", mino_names[i]);
		gfx->ghosts[i] = al_load_bitmap(filename);
		must_init(gfx->ghosts[i], filename);
	}
}

void init_gfx()
{
	gfx = (GFX *)malloc((sizeof(GFX)));
	must_init(gfx, "gfx");

	gfx->playfield = al_load_bitmap("data/gfx/field.png");
	must_init(gfx->playfield, "field.png");

	load_mino_bitmaps();
}

void destroy_gfx()
{
	if (!gfx)
	{
		return;
	}

	al_destroy_bitmap(gfx->playfield);

	for (int i = 0; mino_names[i]; i++)
	{
		al_destroy_bitmap(gfx->minos[i]);
		al_destroy_bitmap(gfx->ghosts[i]);
	}

	free(gfx);
}
