#include "gfx.h"

GFX *gfx;

void init_gfx() {
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
}
