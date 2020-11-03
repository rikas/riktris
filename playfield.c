#include "globals.h"
#include "playfield.h"

#define PLAYFIELD_WIDTH 272u
#define PLAYFIELD_HEIGHT 534u

#define PLAYFIELD_BORDER_H 6u
#define PLAYFIELD_BORDER_V 8u

Playfield *playfield_init()
{
  Playfield *field = (Playfield *)malloc((sizeof(Playfield)));

  ALLEGRO_BITMAP *background = al_load_bitmap("gfx/field.png");
  ALLEGRO_BITMAP *mino = al_load_bitmap("gfx/mino.png");

  playfield_reset(field);

  field->squares[9][9] = 1;
  field->squares[6][2] = 1;
  field->squares[9][0] = 1;
  field->background = background;
  field->mino = mino;

  return field;
}

void playfield_reset(Playfield *field)
{
  unsigned int x, y;

  for (x = 0; x < FIELD_SQUARES_W; x++)
  {
    for (y = 0; y < FIELD_SQUARES_H; y++)
    {
      field->squares[x][y] = 0;
    }
  }
}

void playfield_draw(Playfield *playfield)
{
  unsigned int x, y, tx, ty;
  unsigned int fx, fy;
  unsigned int marginh = 6;
  unsigned int marginv = 8;

  fx = WINDOW_W / 2 - PLAYFIELD_WIDTH / 2;
  fy = WINDOW_H / 2 - PLAYFIELD_HEIGHT / 2;

  al_draw_bitmap(playfield->background, fx, fy, 0);

  // Draw the squares already in place
  for (x = 0; x < 10; x++)
  {
    for (y = 0; y < 20; y++)
    {
      if (playfield->squares[x][y])
      {
        tx = x * (TETRIMINO_W + 1);
        ty = y * (TETRIMINO_W + 1);
        al_draw_bitmap(playfield->mino, fx + marginh + tx, fy + marginv + ty, 0);
      }

      if (playfield->current_mino->rotation & (0x8000 >> (y * ROTATIONS + x))) {
        al_draw_bitmap(playfield->mino, tx, ty, 0);
      }
    }
  }
}
