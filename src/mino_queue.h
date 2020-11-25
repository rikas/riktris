#ifndef __MINO_QUEUE_H
#define __MINO_QUEUE_H

#include "tetriminos.h"

#define QUEUE_SIZE 14

typedef struct MinoQueue {
  int next_minos[QUEUE_SIZE + 1]; // double batch of tetriminos + 1 extra
  int minos_served;
} MinoQueue;
#endif

MinoQueue *queue_init(void);
Tetrimino *pop_mino(MinoQueue *queue, unsigned int speed);
void queue_draw(MinoQueue *queue);
