#ifndef __GFX_H
#define __GFX_H

#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include "globals.h"
#include "utils.h"

typedef struct GFX {
  ALLEGRO_BITMAP *playfield;
  ALLEGRO_BITMAP *minos[TETRIMINO_COUNT + 1];
  ALLEGRO_BITMAP *ghosts[TETRIMINO_COUNT]; // the gray mino has no ghost
} GFX;


extern GFX *gfx;
void init_gfx(void);
void destroy_gfx(void);
#endif
