#include <stdio.h>
#include "globals.h"
#include "game.h"
#include "playfield.h"
#include "mino_queue.h"
#include "input.h"
#include "config.h"

void init_game_state(GameState *state)
{
  state->running = true;
  state->tetrimino_falling = false;
  state->speed = 80;
  state->playfield = playfield_init();
  state->next_queue = queue_init();
}

void read_inputs(GameState *state)
{
  if (delayed_press(config->key_down))
  {
    playfield_move_mino_down(state->playfield, state->current_tetrimino);
  }

  if (delayed_press(config->key_right))
  {
    if (!is_touching_right(state->playfield, state->current_tetrimino))
      t_move(state->current_tetrimino, RIGHT);
  }

  if (delayed_press(config->key_left))
  {
    if (!is_touching_left(state->playfield, state->current_tetrimino))
      t_move(state->current_tetrimino, LEFT);
  }

  if (delayed_press(config->key_rotate_right))
  {
    t_rotate(state->current_tetrimino, RIGHT);
  }

  if (delayed_press(config->key_rotate_left))
  {
    t_rotate(state->current_tetrimino, LEFT);
  }

  if (delayed_press(ALLEGRO_KEY_ESCAPE))
  {
    state->running = false;
  }

  if (delayed_press(ALLEGRO_KEY_ENTER))
  {
    state->current_tetrimino->dropped = true;
    state->tetrimino_falling = false;
  }

  if (delayed_press(config->key_hard_drop))
  {
    playfield_hard_drop(state->playfield, state->current_tetrimino);
  }
}

void update_game_state(GameState *state)
{
  if (!state->tetrimino_falling)
  {
    state->current_tetrimino = pop_mino(state->next_queue, state->speed);
    state->tetrimino_falling = true;
  }

  read_inputs(state);
  input_refresh();

  // If the current mino dropped state is true we want to also update the game state so we can pop
  // another tetrimino from the queue.
  state->tetrimino_falling = !state->current_tetrimino->dropped;

  playfield_remove_completed_lines(state->playfield);
}

void draw_game_state()
{
}

void draw_debug_info(GameState *state, ALLEGRO_FONT *font)
{
  int line = 2;
  char str[20];

  sprintf(str, "Tetrimino: %d", state->current_tetrimino->type);
  al_draw_text(font, al_map_rgb(255, 255, 255), WINDOW_MARGIN, WINDOW_MARGIN + LINE_HEIGHT * line++, 0, str);

  sprintf(str, "Rotation #%d", state->current_tetrimino->rotation_index);
  al_draw_text(font, al_map_rgb(255, 145, 255), WINDOW_MARGIN, WINDOW_MARGIN + LINE_HEIGHT * line++, 0, str);

  sprintf(str, "Rotation HEX: %x", state->current_tetrimino->rotation);
  al_draw_text(font, al_map_rgb(255, 120, 120), WINDOW_MARGIN, WINDOW_MARGIN + LINE_HEIGHT * line++, 0, str);

  sprintf(str, "Played: %d", state->next_queue->minos_served);
  al_draw_text(font, al_map_rgb(100, 255, 200), WINDOW_MARGIN, WINDOW_MARGIN + LINE_HEIGHT * line++, 0, str);

  sprintf(str, "Position: %d, %d", state->current_tetrimino->row, state->current_tetrimino->col);
  al_draw_text(font, al_map_rgb(255, 255, 100), WINDOW_MARGIN, WINDOW_MARGIN + LINE_HEIGHT * line++, 0, str);

  sprintf(str, "Touching left?: %d", is_touching_left(state->playfield, state->current_tetrimino));
  al_draw_text(font, al_map_rgb(245, 155, 50), WINDOW_MARGIN, WINDOW_MARGIN + LINE_HEIGHT * line++, 0, str);

  sprintf(str, "Touching right?: %d", is_touching_right(state->playfield, state->current_tetrimino));
  al_draw_text(font, al_map_rgb(245, 155, 50), WINDOW_MARGIN, WINDOW_MARGIN + LINE_HEIGHT * line++, 0, str);

  sprintf(str, "Touching down?: %d", is_touching_down(state->playfield, state->current_tetrimino));
  al_draw_text(font, al_map_rgb(245, 155, 50), WINDOW_MARGIN, WINDOW_MARGIN + LINE_HEIGHT * line++, 0, str);

  sprintf(str, "Imminent drop?: %d", state->current_tetrimino->imminent_drop);
  al_draw_text(font, al_map_rgb(145, 155, 250), WINDOW_MARGIN, WINDOW_MARGIN + LINE_HEIGHT * line++, 0, str);

  sprintf(str, "HARD DROP: %d", config->key_hard_drop);
  al_draw_text(font, al_map_rgb(145, 155, 250), WINDOW_MARGIN, WINDOW_MARGIN + LINE_HEIGHT * line++, 0, str);
}

void game_main_loop(ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer, ALLEGRO_FONT *font)
{
  GameState *state = (GameState *)malloc(sizeof(GameState));
  init_game_state(state);

  input_init();
  config_load();

  bool should_redraw = false;

  // Timer to make the tetriminos fall
  ALLEGRO_TIMER *tetrimino_drop_timer = al_create_timer(1);
  al_register_event_source(event_queue, al_get_timer_event_source(tetrimino_drop_timer));
  al_start_timer(tetrimino_drop_timer);

  while (state->running)
  {
    ALLEGRO_EVENT event;
    al_wait_for_event(event_queue, &event);

    switch (event.type)
    {
    case ALLEGRO_EVENT_TIMER:
      should_redraw = true;

      if (event.timer.source == tetrimino_drop_timer) {
        // playfield_move_mino_down(state->playfield, state->current_tetrimino);
      } else {
        update_game_state(state);
      }
      // } else {
      //
      // playfield_move_mino_down(state->playfield, state->current_tetrimino);
      // }
      break;
    case ALLEGRO_EVENT_KEY_DOWN:
      key_down(event.keyboard.keycode);
      break;
    case ALLEGRO_EVENT_KEY_UP:
      key_up(event.keyboard.keycode);
      break;
    case ALLEGRO_EVENT_DISPLAY_CLOSE:
      state->running = false;
      break;
    }

    if (should_redraw && al_is_event_queue_empty(event_queue))
    {
      should_redraw = false;

      al_clear_to_color(al_map_rgb(0, 0, 0));

      playfield_draw(state->playfield);

      draw_debug_info(state, font);

      queue_draw(state->next_queue);

      al_draw_text(font, al_map_rgb(240, 240, 240), 450, WINDOW_MARGIN + LINE_HEIGHT, 0, "Next");

      if (state->tetrimino_falling)
      {
        playfield_ghost_draw(state->playfield, state->current_tetrimino);
        playfield_tetrimino_draw(state->playfield, state->current_tetrimino);
      }

      al_flip_display();
    }
  }
}
