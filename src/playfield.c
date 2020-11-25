#include "playfield.h"

#define PLAYFIELD_WIDTH 272u
#define PLAYFIELD_HEIGHT 534u

#define PLAYFIELD_PADDING_X 6u
#define PLAYFIELD_PADDING_Y 7u

Playfield *playfield_init()
{
	Playfield *field = (Playfield *)malloc((sizeof(Playfield)));

	playfield_reset_matrix(field);

	field->x0 = WINDOW_W / 2 - PLAYFIELD_WIDTH / 2;
	field->y0 = WINDOW_H / 2 - PLAYFIELD_HEIGHT / 2;
	field->x = field->x0 + PLAYFIELD_PADDING_X;
	field->y = field->y0 + PLAYFIELD_PADDING_Y;

	field->matrix[9][19] = 2;
	field->matrix[8][19] = 2;
	field->matrix[7][19] = 2;

	return field;
}

static bool overlaps(Playfield *field, Tetrimino *tetrimino)
{
	for (int x = 0; x < ROTATIONS; x++)
	{
		for (int y = 0; y < ROTATIONS; y++)
		{
			if (is_square(tetrimino, x, y))
			{
				if (field->matrix[tetrimino->col + x][tetrimino->row + y])
				{
					return true;
				}

				if ((tetrimino->col + x) > ((int)FIELD_SQUARES_W - 1) ||
						(tetrimino->row + y) > ((int)FIELD_SQUARES_H - 1))
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool is_touching_left(Playfield *field, Tetrimino *mino)
{
	for (int x = 0; x < ROTATIONS; x++)
	{
		for (int y = 0; y < ROTATIONS; y++)
		{
			if (is_square(mino, x, y))
			{
				// Touching the left wall
				if ((mino->col + x) == 0)
				{
					return true;
				}

				// Touching existing squares in the field matrix
				if (field->matrix[mino->col + x - 1][mino->row + y])
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool is_touching_right(Playfield *field, Tetrimino *mino)
{
	for (int x = 0; x < ROTATIONS; x++)
	{
		for (int y = 0; y < ROTATIONS; y++)
		{
			if (is_square(mino, x, y))
			{
				// Touching the right wall
				if ((mino->col + x) == (FIELD_SQUARES_W - 1))
				{
					return true;
				}

				// Touching existing squares in the field matrix
				if (field->matrix[mino->col + x + 1][mino->row + y])
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool is_touching_down(Playfield *field, Tetrimino *mino)
{
	for (int x = 0; x < ROTATIONS; x++)
	{
		for (int y = 0; y < ROTATIONS; y++)
		{
			if (is_square(mino, x, y))
			{
				// If next line is the last line of the field then it's a drop!
				if ((mino->row + y) == (FIELD_SQUARES_H - 1))
				{
					return true;
				}

				// With the current rotation, if the next line already has something then it's a drop!
				if (field->matrix[mino->col + x][mino->row + y + 1])
				{
					return true;
				}
			}
		}
	}

	return false;
}

static int max_drop_row(Playfield *field, Tetrimino *tetrimino)
{
	int max_row = FIELD_SQUARES_H - 1;

	for (int x = 0; x < ROTATIONS; x++)
	{
		for (int y = 0; y < ROTATIONS; y++)
		{
			if (is_square(tetrimino, x, y))
			{
				for (int row = tetrimino->row; row < (int)FIELD_SQUARES_H; row++)
				{
					if ((row + y) == (FIELD_SQUARES_H - 1))
					{
						max_row = min(max_row, row);
					}

					if (field->matrix[tetrimino->col + x][row + y + 1])
					{
						max_row = min(max_row, row);
					}
				}
			}
		}
	}

	return max_row;
}

void playfield_hard_drop(Playfield *field, Tetrimino *tetrimino)
{
	tetrimino->row = max_drop_row(field, tetrimino);
	tetrimino->dropped = true;
	playfield_add_to_matrix(field, tetrimino);
}

void playfield_reset_matrix(Playfield *field)
{
	unsigned int x, y;

	for (x = 0; x < FIELD_SQUARES_W; x++)
	{
		for (y = 0; y < FIELD_SQUARES_H; y++)
		{
			field->matrix[x][y] = 0;
		}
	}
}

void playfield_tetrimino_draw(Playfield *field, Tetrimino *tetrimino)
{
	t_draw(tetrimino, field->x, field->y, MINO_BLOCK);
}

void playfield_ghost_draw(Playfield *field, Tetrimino *tetrimino)
{
	int max_row = max_drop_row(field, tetrimino);

	t_draw(tetrimino, field->x, field->y + (max_row - tetrimino->row) * (TETRIMINO_W + 1), MINO_GHOST);
}

void playfield_draw(Playfield *field)
{
	unsigned int tx, ty;
	al_draw_bitmap(gfx->playfield, field->x0, field->y0, 0);

	// Draw the minos already in place
	for (int x = 0; x < (int)FIELD_SQUARES_W; x++)
	{
		for (int y = 0; y < (int)FIELD_SQUARES_H; y++)
		{
			if (field->matrix[x][y])
			{
				tx = x * (TETRIMINO_W + 1);
				ty = y * (TETRIMINO_W + 1);

				// Draw the mino by getting the right mino gfx. If we have 1 in the matrix then the mino is
				// zero, since TETRIMINO_TYPE starts at 0.
				al_draw_bitmap(gfx->minos[field->matrix[x][y] - 1], field->x + tx, field->y + ty, 0);
			}
		}
	}
}

void playfield_add_to_matrix(Playfield *field, Tetrimino *mino)
{
	for (int x = 0; x < ROTATIONS; x++)
	{
		for (int y = 0; y < ROTATIONS; y++)
		{
			if (is_square(mino, x, y))
			{
				// Add the mino to the matrix. The number will be the mino type + 1 since we can't have it
				// as zero (if the type == MINO_T).
				field->matrix[mino->col + x][mino->row + y] = mino->type + 1;
			}
		}
	}
}

void playfield_remove_completed_lines(Playfield *field)
{
	unsigned int col, row, auxrow;
	bool complete_line;

	for (row = 19; row != 0; row--)
	{
		complete_line = true;

		for (col = 0; col < 10; col++)
		{
			if (!field->matrix[col][row])
			{
				complete_line = false;
			}
		}

		// Move all previous rows down
		if (complete_line)
		{
			for (auxrow = row; auxrow > 0; auxrow--)
			{
				for (col = 0; col < 10; col++)
				{
					field->matrix[col][auxrow] = field->matrix[col][auxrow - 1];
				}
			}
		}
	}
}

int playfield_completed_lines(Playfield *field)
{
	bool complete_line;
	int complete_lines = 0;

	for (int row = 19; row != 0; row--)
	{
		complete_line = true;

		for (int col = 0; col < 10; col++)
		{
			if (!field->matrix[col][row])
			{
				complete_line = false;
			}
		}

		// Move all previous rows down
		if (complete_line)
		{
			complete_lines++;
		}
	}

	return complete_lines;
}

// Try to wall kick a tetrimino. This should be called only if the tetrimino is overlapping existing
// minos in the playfield or out of bounds. Depending on the rotation being executed different
// coordinates will be tested for the wall kick.
//
// This function will return true if it successfully wall kicked the tetrimino or false otherwise.
static bool wall_kick(Playfield *field, Tetrimino *tetrimino, int from, int to)
{
	// Will store the kick coordinates for the given rotation
	KickData kicks;
	KickData *kick_data;

	if (tetrimino->type == MINO_I)
	{
		kick_data = WALL_KICKS_I;
	}
	else
	{
		kick_data = WALL_KICKS;
	}

	if (from == 0 && to == 1)
	{
		memcpy(kicks, kick_data[0], sizeof(kicks));
	}
	else if (from == 1 && to == 0)
	{
		memcpy(kicks, kick_data[1], sizeof(kicks));
	}
	else if (from == 1 && to == 2)
	{
		memcpy(kicks, kick_data[2], sizeof(kicks));
	}
	else if (from == 2 && to == 1)
	{
		memcpy(kicks, kick_data[3], sizeof(kicks));
	}
	else if (from == 2 && to == 3)
	{
		memcpy(kicks, kick_data[4], sizeof(kicks));
	}
	else if (from == 3 && to == 2)
	{
		memcpy(kicks, kick_data[5], sizeof(kicks));
	}
	else if (from == 3 && to == 0)
	{
		memcpy(kicks, kick_data[6], sizeof(kicks));
	}
	else if (from == 0 && to == 3)
	{
		memcpy(kicks, kick_data[7], sizeof(kicks));
	}

	int orig_col = tetrimino->col;
	int orig_row = tetrimino->row;

	// Tries wall kicks until we're done
	for (int i = 0; i < 4; i++)
	{
		tetrimino->col = orig_col + kicks[i][0];
		tetrimino->row = orig_row + kicks[i][1];

		if (!overlaps(field, tetrimino))
		{
			return true;
		}
	}

	// We couldn't wall kick!
	return false;
}

// Tries to rotate the tetrimino in the playfield. It may have to wall kick it. If it's not possible
// to rotate the tetrimino then nothing happens.
void playfield_rotate_tetrimino(Playfield *field, Tetrimino *tetrimino, int direction)
{
	int orig_rotation = tetrimino->rotation_index;

	t_rotate(tetrimino, direction);

	// Col and row before the wall kick calculations
	int orig_col = tetrimino->col;
	int orig_row = tetrimino->row;
	bool kick_success = false;

	// If the resulting rotation will overlap existing minos or will end up outside of the playfield
	// then we need to perform a wall kick. This may or may not succeed, depending on where the
	// tetrimino is. If it doesn't succeed then we assume we can't rotati the tetrimino at all and
	// put it back to the original rotation.
	if (overlaps(field, tetrimino))
	{
		kick_success = wall_kick(field, tetrimino, orig_rotation, tetrimino->rotation_index);

		if (!kick_success)
		{
			tetrimino->rotation_index = orig_rotation;
			tetrimino->col = orig_col;
			tetrimino->row = orig_row;
		}
	}
}

void playfield_move_mino_down(Playfield *field, Tetrimino *tetrimino)
{
	bool touching_down = is_touching_down(field, tetrimino);

	if (touching_down)
	{
		if (tetrimino->imminent_drop)
		{
			tetrimino->dropped = true;
			playfield_add_to_matrix(field, tetrimino);
		}
		else
		{
			tetrimino->imminent_drop = true;
			t_move_down(tetrimino);
		}
	}
	else
	{
		t_move_down(tetrimino);
	}

	tetrimino->imminent_drop = is_touching_down(field, tetrimino);
}
