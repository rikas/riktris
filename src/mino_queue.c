#if defined(_WIN32) || defined(_WIN64) 
#include <winsock.h>
#include <time.h>  
#else
#include <sys/time.h>
#endif
#include <stdio.h>
#include "mino_queue.h"
#include "tetriminos.h"
#include "game.h"

#if defined(_WIN32) || defined(_WIN64)
void shuffle(int* array, size_t n)
{
    if (n > 1)
    {
        size_t i;
        for (i = 0; i < n - 1; i++)
        {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}  
#else
static void shuffle(int* array, size_t n)
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
#endif

static void generate_next_batch(MinoQueue *queue)
{
  int new_indexes[TETRIMINO_COUNT] = {0, 1, 2, 3, 4, 5, 6};
  shuffle(&new_indexes[0], TETRIMINO_COUNT);

  int took = 0;

  for (int i = 0; i < QUEUE_SIZE && took < TETRIMINO_COUNT; i++)
  {
    if (queue->next_minos[i] == INVALID_TETRIMINO)
    {
      queue->next_minos[i] = new_indexes[took];
      took++;
    }
  }
}

MinoQueue *queue_init()
{
  int indexes[TETRIMINO_COUNT] = {0, 1, 2, 3, 4, 5, 6};
  shuffle(&indexes[0], TETRIMINO_COUNT);

  MinoQueue *queue = (MinoQueue *)malloc(sizeof(MinoQueue));

  for (int i = 0; i <= QUEUE_SIZE; i++)
  {
    queue->next_minos[i] = INVALID_TETRIMINO;
  }

  generate_next_batch(queue);
  generate_next_batch(queue);

  queue->minos_served = 0;

  return queue;
}

Tetrimino *pop_mino(MinoQueue *queue, unsigned int speed)
{
  int index = queue->next_minos[0];

  for (int i = 0; i < QUEUE_SIZE; i++)
  {
    queue->next_minos[i] = queue->next_minos[i + 1];
  }

  queue->minos_served += 1;

  if (queue->minos_served % 7 == 0)
  {
    generate_next_batch(queue);
  }

  return t_generate(index, speed);
}

void queue_draw(MinoQueue *queue)
{
  Tetrimino *mino;

  for (int i = 0; i < 3; i++)
  {
    int index = queue->next_minos[i];
    // TODO: we are always generating new minos here! We should not do this and keep the generated
    // minos in memory!
    mino = t_generate(index, 0);
    mino->col = 0;
    t_draw(mino, 450, 80 + (120 * i), MINO_BLOCK);
  }
}
