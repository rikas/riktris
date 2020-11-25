#ifndef __PLAYER_STATE_H
#define __PLAYER_STATE_H

#include <allegro5/allegro5.h>
#include "tetriminos.h"
#include "playfield.h"
#include "mino_queue.h"

typedef struct PlayerState
{
	bool tetrimino_falling;
	unsigned int speed;
	Tetrimino *on_hold;
	Tetrimino *current_tetrimino;
	Tetrimino *next_batch;
	Playfield *playfield;
	MinoQueue *next_queue;
} PlayerState;

#endif
