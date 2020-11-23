#ifndef __TETRIMINOS_H
#define __TETRIMINOS_H

#include "globals.h"
#include <allegro5/allegro5.h>

#define INVALID_TETRIMINO 666

// Because a tetromino is basically a set of 'big pixels' that can be either on or off, it is quite
// suitable and efficient to represent it as a bitmask rather than a matrix of integers.

// Example for the S shape:

// X . . .     1 0 0 0
// X X . .  =  1 1 0 0  =  1000110001000000 (in binary)  =  0x8C40 (in hexadecimal)
// . X . .     0 1 0 0
// . . . .     0 0 0 0

// . X X .     0 1 1 0
// X X . .  =  1 1 0 0  =  0110110000000000 (in binary)  =  0x6C00 (in hexadecimal)
// . . . .     0 0 0 0
// . . . .     0 0 0 0

// I'm using the Super Rotation System (https://tetris.fandom.com/wiki/SRS) which is what is being
// used in modern Tetris games.
static int TETRIMINOS[TETRIMINO_COUNT][ROTATIONS] = {
    // {0x4E00, 0x4640, 0x0E40, 0x4C40}, // T
    // {0x8C40, 0x6C00, 0x8C40, 0x6C00}, // S
    // {0x4C80, 0xC600, 0x4C80, 0xC600}, // Z
    // {0x4444, 0x0F00, 0x4444, 0x0F00}, // I
    // {0x44C0, 0x8E00, 0xC880, 0xE200}, // J
    // {0x2E00, 0x88C0, 0xE800, 0xC440}, // L
    // {0xCC00, 0xCC00, 0xCC00, 0xCC00}  // O
    {0x4E00, 0x4640, 0x0E40, 0x4C40}, // T (SRS)
    {0x6C00, 0x4620, 0x06C0, 0x8C40}, // S (SRS)
    {0xC600, 0x2640, 0x0C60, 0x4C80}, // Z (SRS)
    {0x0F00, 0x2222, 0x00F0, 0x4444}, // I (SRS)
    {0x8E00, 0x6440, 0x0E20, 0x44C0}, // J
    {0x2E00, 0x4460, 0xE800, 0xC440}, // L (SRS)
    {0x6600, 0x6600, 0x6600, 0x6600}  // O (SRS)
};

// The wall kick data is always an array of 4 pairs of coordinates to replace row & col for the
// rotating tetrimino.
typedef int KickData[4][2];

// When the player attempts to rotate a tetromino, but the position it would normally occupy after
// basic rotation is obstructed, (either by the wall or floor of the playfield, or by the stack),
// the game will attempt to "kick" the tetromino into an alternative position nearby
// Wall kick tables taken from https://tetris.fandom.com/wiki/SRS
static KickData WALL_KICKS[8] = {
  { { -1, 0 }, { -1, 1 }, { 0, -2 }, { -1, -2 } },
  { { 1, 0 }, { 1, -1 }, { 0, 2 }, { 1, 2 } },
  { { 1, 0 }, { 1, -1 }, { 0, 2 }, { 1, 2 } },
  { { -1, 0 }, { -1, 1 }, { 0, -2 }, { -1, -2 } },
  { { 1, 0 }, { 1, 1 }, { 0, -2 }, { 1, -2 } },
  { { -1, 0 }, { -1, -1 }, { 0, 2 }, { -1, 2 } },
  { { -1, 0 }, { -1, -1 }, { 0, 2 }, { -1, 2 } },
  { { 1, 0 }, { 1, -1 }, { 0, -2 }, { 1, -2 } }
};

// For the I tetrimino the kicks are different
static KickData WALL_KICKS_I[8] = {
  { { -2, 0 }, { 1, 0 }, { -2, -1 }, { 1, 2 } },
  { { 2, 0 }, { -1, 0 }, { 2, 1 }, { -1, -2 } },
  { { -1, 0 }, { 2, 0 }, { -1, 2 }, { 2, -1 } },
  { { 1, 0 }, { -2, 0 }, { 1, -2 }, { -2, 1 } },
  { { 2, 0 }, { -1, 0 }, { 2, 1 }, { -1, -2 } },
  { { -2, 0 }, { 1, 0 }, { -2, -1 }, { 1, 2 } },
  { { 1, 0 }, { -2, 0 }, { 1, -2 }, { -2, 1 } },
  { { -1, 0 }, { 2, 0 }, { -1, 2 }, { 2, -1 } }
};

typedef enum TETRIMINO_TYPE
{
  MINO_T = 0,
  MINO_S,
  MINO_Z,
  MINO_I,
  MINO_J,
  MINO_L,
  MINO_O
} TETRIMINO_TYPE;

typedef enum MINO_DRAW_TYPE
{
  MINO_BLOCK,
  MINO_GHOST
} MINO_DRAW_TYPE;

typedef struct Tetrimino
{
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

Tetrimino *t_generate(int type, int speed);
bool is_square(Tetrimino *tetrimino, int x, int y);
void t_rotate(Tetrimino *tetrimino, int direction);
void t_move_down(Tetrimino *tetrimino);
void t_move(Tetrimino *tetrimino, int direction);
void t_draw(Tetrimino *tetrimino, int x, int y, int draw_type);
void t_reset(Tetrimino *tetrimino);
#endif
