#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include "globals.h"
#include "tetriminos.h"
#include "gfx.h"

// Checks if the given tetrimino has a 1 for the given x and y coordinates. For that we need to
bool is_square(Tetrimino *mino, int x, int y)
{
	return mino->rotation & (0x8000 >> (y * ROTATIONS + x));
}

void tetrimino_draw(Tetrimino *mino, int pos_x, int pos_y)
{
	int tx, ty;

	for (int x = 0; x < ROTATIONS; x++)
	{
		for (int y = 0; y < ROTATIONS; y++)
		{
			tx = x * (TETRIMINO_W + 1) + pos_x + mino->col * (TETRIMINO_W + 1);
			ty = y * (TETRIMINO_W + 1) + pos_y + mino->row * (TETRIMINO_W + 1);

			if (is_square(mino, x, y))
			{
				al_draw_bitmap(gfx->minos[mino->type], tx, ty, 0);
			}
			else
			{
				// al_draw_tinted_bitmap(gfx->square, al_map_rgba_f(0.2, 0.2, 0.2, 0.5), tx, ty, 0);
			}
		}
	}
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

	mino->col = new_col;
}
