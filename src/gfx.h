#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>

typedef struct GFX {
  ALLEGRO_BITMAP *square;
  ALLEGRO_BITMAP *playfield;
} GFX;


extern GFX *gfx;
void init_gfx();
