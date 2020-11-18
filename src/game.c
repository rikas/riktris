#include <stdio.h>
#include "globals.h"
#include "game.h"
#include "playfield.h"
#include "mino_queue.h"

void game_main_loop(ALLEGRO_EVENT_QUEUE *al_queue, ALLEGRO_TIMER *timer, ALLEGRO_FONT *font)
{
  GameState *state = (GameState *)malloc(sizeof(GameState));
  state->running = true;
  state->tetrimino_dropping = false;
  state->speed = 80;

  bool running = true;
  bool redraw = false;
  bool tetrimino_dropping = false;

	char str[20];

  unsigned char key[ALLEGRO_KEY_MAX];
  memset(key, 0, sizeof(key));

  ALLEGRO_EVENT event;
  Playfield *playfield = playfield_init();
  MinoQueue *mino_queue = queue_init(state);

  // Timer to make the tetriminos fall
  ALLEGRO_TIMER *tetrimino_drop_timer = al_create_timer(1);
  al_register_event_source(al_queue, al_get_timer_event_source(tetrimino_drop_timer));
  al_start_timer(tetrimino_drop_timer);

  ALLEGRO_KEYBOARD_STATE *keyboard;
  double ticks = al_get_timer_count(timer);

  while (state->running)
  {
    al_wait_for_event(al_queue, &event);
    if (event.type == ALLEGRO_EVENT_TIMER && event.timer.source == tetrimino_drop_timer) {
      playfield_move_mino_down(playfield);
    }
    if (event.type == ALLEGRO_EVENT_TIMER && event.timer.source != tetrimino_drop_timer)
    {
      if (!state->tetrimino_dropping) {
        playfield->current_mino = pop_mino(mino_queue);
        state->tetrimino_dropping = true;
      } else {
        if (key[ALLEGRO_KEY_S])
          playfield_move_mino_down(playfield);
        if (key[ALLEGRO_KEY_D])
          if (!is_touching_right(playfield, playfield->current_mino))
            tetrimino_move(playfield->current_mino, RIGHT);
        if (key[ALLEGRO_KEY_A])
          if (!is_touching_left(playfield, playfield->current_mino))
            tetrimino_move(playfield->current_mino, LEFT);

        for(int i = 0; i < ALLEGRO_KEY_MAX; i++)
          key[i] &= KEY_SEEN;
      }
      redraw = true;
    }
    else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
      key[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;

      switch (event.keyboard.keycode) {
        case ALLEGRO_KEY_I:
          tetrimino_rotate(playfield->current_mino, LEFT);
          break;
        case ALLEGRO_KEY_O:
          tetrimino_rotate(playfield->current_mino, RIGHT);
          break;
        case ALLEGRO_KEY_ESCAPE:
          state->running = false;
          break;
        case ALLEGRO_KEY_SPACE:
          state->tetrimino_dropping = false;
        // case ALLEGRO_KEY_S:
        //   playfield_move_mino_down(playfield);
        //   break;
        // case ALLEGRO_KEY_A:
        //   if (!is_touching_left(playfield, playfield->current_mino))
        //     tetrimino_move(playfield->current_mino, LEFT);
        //   break;
        // case ALLEGRO_KEY_D:
        //   if (!is_touching_right(playfield, playfield->current_mino))
        //     tetrimino_move(playfield->current_mino, RIGHT);
        //   break;
      }
      redraw = true;
    }
    else if (event.type == ALLEGRO_EVENT_KEY_UP) {
      key[event.keyboard.keycode] &= KEY_RELEASED;
    }
    else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
    {
      state->running = false;
    }

    // if (frames % state->speed <= 0) {
    //   playfield_move_mino_down(playfield);

    //   redraw = true;
    // }

    // If the current mino dropped state is true we want to also update the game state so we can pop
    // another tetrimino from the queue.
    state->tetrimino_dropping = !playfield->current_mino->dropped;

    if (redraw && al_is_event_queue_empty(al_queue))
    {
      al_clear_to_color(al_map_rgb(0, 0, 0));

      playfield_draw(playfield);

      int line = 2;
      sprintf(str, "Tetrimino: %d", playfield->current_mino->type);
      al_draw_text(font, al_map_rgb(255, 255, 255), WINDOW_MARGIN, WINDOW_MARGIN + LINE_HEIGHT * line++, 0, str);

      sprintf(str, "Rotation #%d", playfield->current_mino->rotation_index);
      al_draw_text(font, al_map_rgb(255, 145, 255), WINDOW_MARGIN, WINDOW_MARGIN + LINE_HEIGHT * line++, 0, str);

      sprintf(str, "Rotation HEX: %x", playfield->current_mino->rotation);
      al_draw_text(font, al_map_rgb(255, 120, 120), WINDOW_MARGIN, WINDOW_MARGIN + LINE_HEIGHT * line++, 0, str);

      sprintf(str, "Played: %d", mino_queue->minos_served);
      al_draw_text(font, al_map_rgb(100, 255, 200), WINDOW_MARGIN, WINDOW_MARGIN + LINE_HEIGHT * line++, 0, str);

      sprintf(str, "Position: %d, %d", playfield->current_mino->row, playfield->current_mino->col);
      al_draw_text(font, al_map_rgb(255, 255, 100), WINDOW_MARGIN, WINDOW_MARGIN + LINE_HEIGHT * line++, 0, str);

      sprintf(str, "Touching left?: %d", is_touching_left(playfield, playfield->current_mino));
      al_draw_text(font, al_map_rgb(245, 155, 50), WINDOW_MARGIN, WINDOW_MARGIN + LINE_HEIGHT * line++, 0, str);

      sprintf(str, "Touching right?: %d", is_touching_right(playfield, playfield->current_mino));
      al_draw_text(font, al_map_rgb(245, 155, 50), WINDOW_MARGIN, WINDOW_MARGIN + LINE_HEIGHT * line++, 0, str);

      sprintf(str, "Touching down?: %d", is_touching_down(playfield, playfield->current_mino));
      al_draw_text(font, al_map_rgb(245, 155, 50), WINDOW_MARGIN, WINDOW_MARGIN + LINE_HEIGHT * line++, 0, str);

      sprintf(str, "Imminent drop?: %d", playfield->current_mino->imminent_drop);
      al_draw_text(font, al_map_rgb(145, 155, 250), WINDOW_MARGIN, WINDOW_MARGIN + LINE_HEIGHT * line++, 0, str);

      sprintf(str, "Timer speed: %d", al_get_timer_speed(tetrimino_drop_timer));
      al_draw_text(font, al_map_rgb(245, 215, 210), WINDOW_MARGIN, WINDOW_MARGIN + LINE_HEIGHT * line++, 0, str);

      sprintf(str, "Timer count: %d", al_get_timer_count(timer));
      al_draw_text(font, al_map_rgb(245, 215, 210), WINDOW_MARGIN, WINDOW_MARGIN + LINE_HEIGHT * line++, 0, str);

      queue_draw(mino_queue);

      al_draw_text(font, al_map_rgb(240, 240, 240), 450, WINDOW_MARGIN + LINE_HEIGHT, 0, "Next");

      if (state->tetrimino_dropping) {
        playfield_tetrimino_draw(playfield, playfield->current_mino);
      }

      playfield_remove_completed_lines(playfield);

      al_flip_display();

      redraw = false;
    }
  }
}
