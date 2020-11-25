#ifndef __GAME_H
#define __GAME_H

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include "tetriminos.h"
#include "playfield.h"
#include "mino_queue.h"

typedef struct GameState
{
	bool running;
	bool tetrimino_falling;
	bool held_tetrimino;
	unsigned int speed;
	unsigned int complete_lines;
	Tetrimino *tetrimino_on_hold;
	Tetrimino *current_tetrimino;
	Tetrimino *next_batch;
	Playfield *playfield;
	MinoQueue *next_queue;
} GameState;

void game_main_loop(ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_FONT *font);

#endif
