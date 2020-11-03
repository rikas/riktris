#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include "globals.h"
#include "tetriminos.h"

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
int TETRIMINOS[TETRIMINO_COUNT][ROTATIONS] = {
	{0x4E00, 0x4640, 0x0E40, 0x4C40}, // T
	{0x8C40, 0x6C00, 0x8C40, 0x6C00}, // S
	{0x4C80, 0xC600, 0x4C80, 0xC600}, // Z
	{0x4444, 0x0F00, 0x4444, 0x0F00}, // I
	{0x44C0, 0x8E00, 0xC880, 0xE200}, // J
	{0x88C0, 0xE800, 0xC440, 0x2E00}, // L
	{0xCC00, 0xCC00, 0xCC00, 0xCC00}  // O
};

Tetrimino *tetrimino_generate(int type, int speed)
{
  Tetrimino *mino = (Tetrimino *)malloc((sizeof(Tetrimino)));

	mino->type = type;
	mino->speed = speed;
	mino->row = 0;
	mino->col = 0;

  return mino;
}

void tetrimino_rotate(Tetrimino *mino, int direction) {
	int new_rotation = 0;

	switch(direction) {
		case ROTATE_LEFT:
			new_rotation = (mino->rotation - 1) % ROTATIONS;
			break;
		case ROTATE_RIGHT:
			new_rotation = (mino->rotation + 1) % ROTATIONS;
			break;
	}

	if (new_rotation < 0) {
		new_rotation = ROTATIONS -1;
	}

	mino->rotation = new_rotation;
}

void tetrimino_move(Tetrimino *mino, int row, int col) {
	mino->row = row;
	mino->col = col;
}

// void tetrimino_draw(Playfield *field, int num, int rotation)
// {
// 	int x, y, tx, ty;

// 	for (x = 0; x < ROTATIONS; x++)
// 	{
// 		for (y = 0; y < ROTATIONS; y++)
// 		{
// 			tx = x * TETRIMINO_W;
// 			ty = y * TETRIMINO_W;

// 			if (TETRIMINOS[num][rotation] & (0x8000 >> (y * ROTATIONS + x)))
// 			{
// 				al_draw_bitmap(field->mino, tx, ty, 0);
// 			}
// 		}
// 	}
// }
