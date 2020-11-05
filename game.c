#include <stdio.h>
#include "globals.h"
#include "game.h"
#include "playfield.h"
#include "mino_queue.h"

void game_main_loop(ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_FONT *font)
{
  GameState *state = (GameState *)malloc(sizeof(GameState));
  state->running = true;
  state->tetrimino_dropping = false;

  bool running = true;
  bool redraw = false;
  bool tetrimino_dropping = false;
  unsigned int speed = 1;

	char str[20];
	char rot_str[20];

  ALLEGRO_EVENT event;
  Playfield *playfield = playfield_init();
  MinoQueue *mino_queue = queue_init(state);

  while (state->running)
  {
    al_wait_for_event(queue, &event);

    if (event.type == ALLEGRO_EVENT_TIMER)
    {
      if (!state->tetrimino_dropping) {
        playfield->current_mino = tetrimino_generate(speed);
        state->tetrimino_dropping = true;
      }
      redraw = true;
    }
    else if (event.type == ALLEGRO_EVENT_KEY_DOWN)
    {
      switch (event.keyboard.keycode)
      {
      case ALLEGRO_KEY_I:
        tetrimino_rotate(playfield->current_mino, LEFT);
        break;
      case ALLEGRO_KEY_O:
        tetrimino_rotate(playfield->current_mino, RIGHT);
        break;
      case ALLEGRO_KEY_S:
        tetrimino_move_down(playfield->current_mino);
        break;
      case ALLEGRO_KEY_A:
        tetrimino_move(playfield->current_mino, LEFT);
        break;
      case ALLEGRO_KEY_D:
        tetrimino_move(playfield->current_mino, RIGHT);
        break;
      case ALLEGRO_KEY_ESCAPE:
        state->running = false;
        break;
      case ALLEGRO_KEY_SPACE:
        state->tetrimino_dropping = false;
        break;
      }
    }
    else if (event.type == ALLEGRO_EVENT_KEY_CHAR) {
      if (event.keyboard.repeat) {
        switch (event.keyboard.keycode)
        {
          case ALLEGRO_KEY_A:
            tetrimino_move(playfield->current_mino, LEFT);
            break;
          case ALLEGRO_KEY_D:
            tetrimino_move(playfield->current_mino, RIGHT);
            break;
          case ALLEGRO_KEY_S:
            tetrimino_move_down(playfield->current_mino);
            break;
        }
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

      sprintf(str, "Tetrimino: %d", playfield->current_mino->type);
      al_draw_text(font, al_map_rgb(255, 255, 255), WINDOW_MARGIN, WINDOW_MARGIN + LINE_HEIGHT * 18, 0, str);

      sprintf(str, "Rotation #%d", playfield->current_mino->rotation_index);
      al_draw_text(font, al_map_rgb(255, 145, 255), WINDOW_MARGIN, WINDOW_MARGIN + LINE_HEIGHT * 19, 0, str);

      sprintf(str, "Rotation HEX: %x", playfield->current_mino->rotation);
      al_draw_text(font, al_map_rgb(255, 120, 120), WINDOW_MARGIN, WINDOW_MARGIN + LINE_HEIGHT * 20, 0, str);

      bool drop = is_tetrimino_drop(playfield, playfield->current_mino);
      sprintf(str, "Is touching?: %d", drop);
      al_draw_text(font, al_map_rgb(100, 255, 200), WINDOW_MARGIN, WINDOW_MARGIN + LINE_HEIGHT * 21, 0, str);

      sprintf(str, "Tetrimino: %d, %d", playfield->current_mino->row, playfield->current_mino->col);
      al_draw_text(font, al_map_rgb(255, 255, 100), WINDOW_MARGIN, WINDOW_MARGIN + LINE_HEIGHT * 22, 0, str);

      if (state->tetrimino_dropping) {
        playfield_tetrimino_draw(playfield, playfield->current_mino);
      }

      if (is_tetrimino_drop(playfield, playfield->current_mino)) {
        playfield_add_to_matrix(playfield, playfield->current_mino);
        state->tetrimino_dropping = false;
      }

      playfield_remove_completed_lines(playfield);

      al_flip_display();

      redraw = false;
    }
  }
}
