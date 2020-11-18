#ifndef __TETRIMINOS_H
#define __TETRIMINOS_H

#include "globals.h"
#include <allegro5/allegro5.h>

#define INVALID_TETRIMINO 666

typedef enum TETRIMINO_TYPE {
  MINO_T = 0,
  MINO_S,
  MINO_Z,
  MINO_I,
  MINO_J,
  MINO_L,
  MINO_O
} TETRIMINO_TYPE;

typedef struct Tetrimino {
  TETRIMINO_TYPE type;
  int rotations[ROTATIONS];
  int rotation;
  int rotation_index;
  int speed;

  bool imminent_drop;
  bool dropped;

  int col;
  int row;
} Tetrimino;


bool is_square(Tetrimino *tetrimino, int x, int y);
void tetrimino_rotate(Tetrimino *tetrimino, int direction);
void tetrimino_move_down(Tetrimino *tetrimino);
void tetrimino_move(Tetrimino *tetrimino, int direction);
void tetrimino_draw(Tetrimino *tetrimino, int x, int y);
#endif
