#include "gfx.h"

GFX *gfx;

void init_gfx()
{
  gfx = (GFX *)malloc((sizeof(GFX)));

  gfx->playfield = al_load_bitmap("gfx/field.png");

  gfx->minos[0] = al_load_bitmap("gfx/mino_t.png");
  gfx->minos[1] = al_load_bitmap("gfx/mino_s.png");
  gfx->minos[2] = al_load_bitmap("gfx/mino_z.png");
  gfx->minos[3] = al_load_bitmap("gfx/mino_i.png");
  gfx->minos[4] = al_load_bitmap("gfx/mino_j.png");
  gfx->minos[5] = al_load_bitmap("gfx/mino_l.png");
  gfx->minos[6] = al_load_bitmap("gfx/mino_o.png");
  gfx->minos[7] = al_load_bitmap("gfx/mino_x.png");

  gfx->ghosts[0] = al_load_bitmap("gfx/mino_ghost_t.png");
  gfx->ghosts[1] = al_load_bitmap("gfx/mino_ghost_s.png");
  gfx->ghosts[2] = al_load_bitmap("gfx/mino_ghost_z.png");
  gfx->ghosts[3] = al_load_bitmap("gfx/mino_ghost_i.png");
  gfx->ghosts[4] = al_load_bitmap("gfx/mino_ghost_j.png");
  gfx->ghosts[5] = al_load_bitmap("gfx/mino_ghost_l.png");
  gfx->ghosts[6] = al_load_bitmap("gfx/mino_ghost_o.png");
}

void destroy_gfx()
{
  if (!gfx) {
    return;
  }

  al_destroy_bitmap(gfx->playfield);

  for (int i = 0; i < TETRIMINO_COUNT; i++)
  {
    al_destroy_bitmap(gfx->minos[i]);
    al_destroy_bitmap(gfx->ghosts[i]);
  }

  al_destroy_bitmap(gfx->minos[TETRIMINO_COUNT]);

  free(gfx);
}
