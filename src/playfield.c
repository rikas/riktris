#include "globals.h"
#include "playfield.h"
#include "gfx.h"

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

  field->matrix[9][19] = 1;
  field->matrix[8][19] = 2;
  field->matrix[7][19] = 3;
  field->matrix[6][19] = 4;
  field->matrix[5][19] = 5;
  field->matrix[4][19] = 6;
  field->matrix[3][19] = 1;
  field->matrix[2][19] = 1;

  return field;
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

void playfield_tetrimino_draw(Playfield *field, Tetrimino *mino)
{
  tetrimino_draw(mino, field->x, field->y);
}

void playfield_draw(Playfield *field)
{
  unsigned int tx, ty;

  al_draw_bitmap(gfx->playfield, field->x0, field->y0, 0);

  // Draw the minos already in place
  for (int x = 0; x < FIELD_SQUARES_W; x++)
  {
    for (int y = 0; y < FIELD_SQUARES_H; y++)
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

void playfield_move_mino_down(Playfield *field)
{
  bool touching_down = is_touching_down(field, field->current_mino);

  if (touching_down)
  {
    if (field->current_mino->imminent_drop)
    {
      field->current_mino->dropped = true;
      playfield_add_to_matrix(field, field->current_mino);
    }
    else
    {
      field->current_mino->imminent_drop = true;
      tetrimino_move_down(field->current_mino);
    }
  }
  else
  {
    tetrimino_move_down(field->current_mino);
  }

  field->current_mino->imminent_drop = is_touching_down(field, field->current_mino);
}
