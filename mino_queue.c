#include <sys/time.h>
#include <stdio.h>
#include "mino_queue.h"
#include "tetriminos.h"
#include "game.h"

void shuffle_available_minos(int *array, size_t n)
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  int usec = tv.tv_usec;
  srand48(usec);

  if (n > 1)
  {
    size_t i;
    for (i = n - 1; i > 0; i--)
    {
      size_t j = (unsigned int)(drand48() * (i + 1));
      int t = array[j];
      array[j] = array[i];
      array[i] = t;
    }
  }
}

MinoQueue *queue_init(GameState *state)
{
  MinoQueue *queue = (MinoQueue *)malloc(sizeof(MinoQueue));
  int i;

  for (i = 0; i < QUEUE_SIZE; i++)
  {
    queue->next_minos[i] = tetrimino_generate(state->speed);
  }

  queue->game_state = state;

  return queue;
}
