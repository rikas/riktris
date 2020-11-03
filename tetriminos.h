#ifndef __TETRIMINOS_H
#define __TETRIMINOS_H

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
  int speed;
  int row;
  int col;
} Tetrimino;


Tetrimino *tetrimino_generate(int type, int speed);
// void tetrimino_draw(Playfield *field, int num, int rotation);

#endif
