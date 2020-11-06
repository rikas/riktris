#include "gfx.h"

GFX *gfx;

void init_gfx() {
  gfx = (GFX *)malloc((sizeof(GFX)));

  gfx->playfield = al_load_bitmap("gfx/field.png");
  gfx->square = al_load_bitmap("gfx/mino.png");
}
