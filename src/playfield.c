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
  field->matrix[8][19] = 1;
  field->matrix[7][19] = 1;
  field->matrix[6][19] = 1;
  field->matrix[5][19] = 1;
  field->matrix[4][19] = 1;
  field->matrix[3][19] = 1;
  field->matrix[2][19] = 1;

  return field;
}

bool is_tetrimino_drop(Playfield *field, Tetrimino *mino)
{
  int x, y;

  for (x = 0; x < ROTATIONS; x++)
  {
    for (y = 0; y < ROTATIONS; y++)
    {
      if (mino->rotation & (0x8000 >> (y * ROTATIONS + x)))
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
  unsigned int x, y, tx, ty;

  al_draw_bitmap(gfx->playfield, field->x0, field->y0, 0);

  // Draw the minos already in place
  for (x = 0; x < 10; x++)
  {
    for (y = 0; y < 20; y++)
    {
      if (field->matrix[x][y])
      {
        tx = x * (TETRIMINO_W + 1);
        ty = y * (TETRIMINO_W + 1);
        al_draw_bitmap(gfx->square, field->x + tx, field->y + ty, 0);
      }
    }
  }
}

void playfield_add_to_matrix(Playfield *field, Tetrimino *mino)
{
  int x, y;

  for (x = 0; x < ROTATIONS; x++)
  {
    for (y = 0; y < ROTATIONS; y++)
    {
      if (mino->rotation & (0x8000 >> (y * ROTATIONS + x)))
      {
        // Add the mino to the matrix
        field->matrix[mino->col + x][mino->row + y] = 1;
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
      for (auxrow = row; auxrow > 0; auxrow--) {
        for (col = 0; col < 10; col++)
        {
          field->matrix[col][auxrow] = field->matrix[col][auxrow - 1];
        }
      }
    }
  }
}
