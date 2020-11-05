#ifndef __MINO_QUEUE_H
#define __MINO_QUEUE_H

#include "tetriminos.h"
#include "game.h"

#define QUEUE_SIZE 14

typedef struct MinoQueue {
  GameState *game_state;
  Tetrimino *next_minos[QUEUE_SIZE]; // double batch of tetriminos
} MinoQueue;
#endif

MinoQueue *queue_init(GameState *state);
