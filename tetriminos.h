#ifndef __TETRIMINOS_H
#define __TETRIMINOS_H

#include "globals.h"

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
  unsigned int row;
  unsigned int col;
} Tetrimino;


Tetrimino *tetrimino_generate(int speed);
void tetrimino_rotate(Tetrimino *mino, int direction);
void tetrimino_move_down(Tetrimino *mino);
void tetrimino_move(Tetrimino *mino, int direction);
#endif
