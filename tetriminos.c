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
		{0xCC00, 0xCC00, 0xCC00, 0xCC00}	// O
};

Tetrimino *tetrimino_generate(int speed)
{
	int type = rand() % TETRIMINO_COUNT;
	Tetrimino *mino = (Tetrimino *)malloc(sizeof(Tetrimino));

	memcpy(mino->rotations, TETRIMINOS[type], sizeof(TETRIMINOS[type]));
	mino->type = type;
	mino->speed = speed;
	mino->rotation_index = 0;
	mino->rotation = TETRIMINOS[type][0];
	mino->row = 0;
	mino->col = 0;

	return mino;
}

void tetrimino_rotate(Tetrimino *mino, int direction)
{
	int new_index = 0;

	switch (direction)
	{
	case LEFT:
		new_index = (mino->rotation_index - 1) % ROTATIONS;
		break;
	case RIGHT:
		new_index = (mino->rotation_index + 1) % ROTATIONS;
		break;
	}

	if (new_index < 0)
	{
		new_index = ROTATIONS - 1;
	}
	mino->rotation_index = new_index;
	mino->rotation = mino->rotations[new_index];
}

void tetrimino_move_down(Tetrimino *mino)
{
	mino->row += 1;
}

void tetrimino_move(Tetrimino *mino, int direction)
{
	int new_col;

	switch (direction)
	{
	case LEFT:
		new_col = mino->col - 1;
		break;
	case RIGHT:
		new_col = mino->col + 1;
		break;
	}

	if (new_col < 0)
	{
		new_col = 0;
	}

	mino->col = new_col;
}
