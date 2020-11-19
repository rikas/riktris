#ifndef __FIELD_H
#define __FIELD_H

#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include "tetriminos.h"
#include "utils.h"
#include "gfx.h"

#define FIELD_SQUARES_W 10u
#define FIELD_SQUARES_H 20u

typedef struct Playfield {
  unsigned int x0;
  unsigned int y0;

  unsigned int x;
  unsigned int y;

  unsigned int margin_x;
  unsigned int margin_y;

	unsigned int matrix[FIELD_SQUARES_W][FIELD_SQUARES_H];
  //Tetrimino *current_mino;
} Playfield;

void playfield_reset_matrix(Playfield *field);
Playfield *playfield_init();

void playfield_draw(Playfield *field);
void playfield_tetrimino_draw(Playfield *field, Tetrimino *mino);
void playfield_ghost_draw(Playfield *field, Tetrimino *mino);

bool is_touching_down(Playfield *field, Tetrimino *mino);
bool is_touching_left(Playfield *field, Tetrimino *mino);
bool is_touching_right(Playfield *field, Tetrimino *mino);

void playfield_add_to_matrix(Playfield *field, Tetrimino *mino);
void playfield_remove_completed_lines(Playfield *field);

void playfield_move_mino_down(Playfield *field, Tetrimino *mino);
void playfield_hard_drop(Playfield *field, Tetrimino *mino);

#endif
