#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include "globals.h"

typedef struct GFX {
  ALLEGRO_BITMAP *playfield;
  ALLEGRO_BITMAP *minos[TETRIMINO_COUNT + 1];
} GFX;


extern GFX *gfx;
void init_gfx();
