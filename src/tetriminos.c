#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include "globals.h"
#include "tetriminos.h"
#include "gfx.h"

void tetrimino_draw(Tetrimino *mino, int pos_x, int pos_y) {
  int x, y, tx, ty;

  for (x = 0; x < ROTATIONS; x++)
  {
    for (y = 0; y < ROTATIONS; y++)
    {
      tx = x * (TETRIMINO_W + 1) + pos_x + mino->col * (TETRIMINO_W + 1);
      ty = y * (TETRIMINO_W + 1) + pos_y + mino->row * (TETRIMINO_W + 1);

      if (mino->rotation & (0x8000 >> (y * ROTATIONS + x)))
      {
        al_draw_bitmap(gfx->square, tx, ty, 0);
      }
      else
      {
        // al_draw_tinted_bitmap(gfx->square, al_map_rgba_f(5, 0.2, 0.1, 0.2), tx, ty, 0);
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

	if (new_col < 0)
	{
		new_col = 0;
	}

	mino->col = new_col;
}
