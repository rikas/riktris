#include "gfx.h"

GFX *gfx;

void init_gfx()
{
  gfx = (GFX *)malloc((sizeof(GFX)));
  must_init(gfx, "gfx");

  gfx->playfield = al_load_bitmap("field.png");
  must_init(gfx->playfield, "gfx playfield.png");

  char *mino_names[TETRIMINO_COUNT] = {"t", "s", "z", "i", "j", "l", "o"};

  for (int i = 0; i < TETRIMINO_COUNT; i++)
  {
    char filename[32];

    sprintf(filename, "mino_%s.png", mino_names[i]);
    gfx->minos[i] = al_load_bitmap(filename);
    must_init(gfx->minos[i], filename);

    sprintf(filename, "mino_ghost_%s.png", mino_names[i]);
    gfx->ghosts[i] = al_load_bitmap(filename);
    must_init(gfx->ghosts[i], filename);
  }
}

void destroy_gfx()
{
  if (!gfx)
  {
    return;
  }

  al_destroy_bitmap(gfx->playfield);

  for (int i = 0; i < TETRIMINO_COUNT; i++)
  {
    al_destroy_bitmap(gfx->minos[i]);
    al_destroy_bitmap(gfx->ghosts[i]);
  }

  free(gfx);
}
