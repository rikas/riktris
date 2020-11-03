#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/debug.h>
#include <allegro5/allegro_font.h>
#include "globals.h"
#include "playfield.h"
#include "tetriminos.h"

void game_main_loop(ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_FONT *font)
{
  bool running = true;
  bool redraw = false;
  bool tetrimino_dropping = false;
  int speed = 1;

	char str[20];
	char rot_str[20];
	int rotation = 0;
	int tetrimino_num = rand() % 7;

  ALLEGRO_EVENT event;
  Tetrimino *current_mino;
  Playfield *playfield = playfield_init();

  while (running)
  {
    al_wait_for_event(queue, &event);

    if (event.type == ALLEGRO_EVENT_TIMER)
    {
      if (!tetrimino_dropping) {
        playfield->current_mino = tetrimino_generate(tetrimino_num, speed);
        tetrimino_dropping = true;
      }
      redraw = true;
    }
    else if (event.type == ALLEGRO_EVENT_KEY_DOWN)
    {
      switch (event.keyboard.keycode)
      {
      case ALLEGRO_KEY_I:
        rotation = (rotation - 1) % ROTATIONS;
        if (rotation < 0)
        {
          rotation = ROTATIONS - 1;
        }
        break;
      case ALLEGRO_KEY_O:
        rotation = (rotation + 1) % ROTATIONS;
        break;
      case ALLEGRO_KEY_ESCAPE:
        running = false;
        break;
      case ALLEGRO_KEY_SPACE:
        tetrimino_num = rand() % 7;
        break;
      }
    }
    else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
    {
      break;
    }

    if (redraw && al_is_event_queue_empty(queue))
    {
      al_clear_to_color(al_map_rgb(0, 0, 0));

      playfield_draw(playfield);

      sprintf(str, "Tetrimino: %d", tetrimino_num);
      sprintf(rot_str, "Rotation #%d", rotation);

      al_draw_text(font, al_map_rgb(255, 255, 255), WINDOW_MARGIN, WINDOW_MARGIN + LINE_HEIGHT * 18, 0, str);
      al_draw_text(font, al_map_rgb(255, 145, 255), WINDOW_MARGIN, WINDOW_MARGIN + LINE_HEIGHT * 19, 0, rot_str);

      // if (tetrimino_dropping) {
      //   tetrimino_draw(playfield, tetrimino_num, rotation);
      // }

      al_flip_display();

      redraw = false;
    }
  }
}
