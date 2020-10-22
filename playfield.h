#ifndef __FIELD_H
#define __FIELD_H

#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>

#define FIELD_SQUARES_W 10u
#define FIELD_SQUARES_H 20u

typedef struct Playfield {
  ALLEGRO_BITMAP *background;
	unsigned int squares[FIELD_SQUARES_W][FIELD_SQUARES_H];
} Playfield;

void playfield_reset(Playfield *field);
Playfield *playfield_init();
void playfield_draw(Playfield *playfield);

#endif
