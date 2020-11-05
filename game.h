#ifndef __GAME_H
#define __GAME_H

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include "tetriminos.h"

typedef struct GameState {
  bool running;
  bool tetrimino_dropping;
  unsigned int speed;
  Tetrimino *next_batch;
} GameState;

void game_main_loop(ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_FONT *font);
#endif
