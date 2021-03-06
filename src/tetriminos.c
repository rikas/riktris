#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include "globals.h"
#include "tetriminos.h"
#include "gfx.h"

Tetrimino *t_generate(int type, int speed)
{
	Tetrimino *mino = (Tetrimino *)malloc(sizeof(Tetrimino));

	mino->type = type;
	mino->speed = speed;

	memcpy(mino->rotations, TETRIMINOS[type], sizeof(TETRIMINOS[type]));

	t_reset(mino);

	return mino;
}

// Resets the tetrimino values. The default col is 3 because it's the center of the playfield.
void t_reset(Tetrimino *tetrimino)
{
	tetrimino->row = 0;
	tetrimino->col = 3;
	tetrimino->imminent_drop = false;
	tetrimino->dropped = false;
	tetrimino->rotation_index = 0;
}

// Checks if the given tetrimino has a 1 for the given x and y coordinates. For that we need to
bool is_square(Tetrimino *tetrimino, int x, int y)
{
	return tetrimino->rotations[tetrimino->rotation_index] & (0x8000 >> (y * ROTATIONS + x));
}

void t_draw(Tetrimino *tetrimino, int pos_x, int pos_y, int draw_type)
{
	int tx, ty;

	for (int x = 0; x < ROTATIONS; x++)
	{
		for (int y = 0; y < ROTATIONS; y++)
		{
			tx = x * (TETRIMINO_W + 1) + pos_x + tetrimino->col * (TETRIMINO_W + 1);
			ty = y * (TETRIMINO_W + 1) + pos_y + tetrimino->row * (TETRIMINO_W + 1);

			if (is_square(tetrimino, x, y))
			{
				if (draw_type == MINO_BLOCK)
				{
					al_draw_bitmap(gfx->minos[tetrimino->type], tx, ty, 0);
				}
				else
				{
					al_draw_bitmap(gfx->ghosts[tetrimino->type], tx, ty, 0);
				}
			}
			else
			{
				// al_draw_tinted_bitmap(gfx->ghosts[tetrimino->type], al_map_rgba_f(0.2, 0.2, 0.2, 0.5), tx, ty, 0);
			}
		}
	}
}

void t_rotate(Tetrimino *mino, int direction)
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
}

void t_move_down(Tetrimino *mino)
{
	mino->row += 1;
}

void t_move(Tetrimino *mino, int direction)
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
