#include <sys/time.h>
#include <stdio.h>
#include "mino_queue.h"
#include "tetriminos.h"
#include "game.h"

// Because a tetromino is basically a set of 'big pixels' that can be either on or off, it is quite
// suitable and efficient to represent it as a bitmask rather than a matrix of integers.

// Example for the S shape:

// X . . .     1 0 0 0
// X X . .  =  1 1 0 0  =  1000110001000000 (in binary)  =  0x8C40 (in hexadecimal)
// . X . .     0 1 0 0
// . . . .     0 0 0 0

// . X X .     0 1 1 0
// X X . .  =  1 1 0 0  =  0110110000000000 (in binary)  =  0x6C00 (in hexadecimal)
// . . . .     0 0 0 0
// . . . .     0 0 0 0
int TETRIMINOS[TETRIMINO_COUNT][ROTATIONS] = {
    {0x4E00, 0x4640, 0x0E40, 0x4C40}, // T
    {0x8C40, 0x6C00, 0x8C40, 0x6C00}, // S
    {0x4C80, 0xC600, 0x4C80, 0xC600}, // Z
    {0x4444, 0x0F00, 0x4444, 0x0F00}, // I
    {0x44C0, 0x8E00, 0xC880, 0xE200}, // J
    {0x88C0, 0xE800, 0xC440, 0x2E00}, // L
    {0xCC00, 0xCC00, 0xCC00, 0xCC00}  // O
};

void shuffle(int *array, size_t n)
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

Tetrimino *tetrimino_generate(int type, int speed)
{
  //int type = rand() % TETRIMINO_COUNT;
  Tetrimino *mino = (Tetrimino *)malloc(sizeof(Tetrimino));

  memcpy(mino->rotations, TETRIMINOS[type], sizeof(TETRIMINOS[type]));
  mino->type = type;
  mino->speed = speed;
  mino->rotation_index = 0;
  mino->rotation = TETRIMINOS[type][0];
  mino->row = 0;
  mino->col = 0;
  mino->imminent_drop = false;
  mino->dropped = false;

  return mino;
}

void generate_next_batch(MinoQueue *queue) {
  int new_indexes[TETRIMINO_COUNT] = {0, 1, 2, 3, 4, 5, 6};
  shuffle(&new_indexes[0], TETRIMINO_COUNT);

  int i, took = 0;

  for (i=0; i<QUEUE_SIZE && took < TETRIMINO_COUNT; i++) {
    if (queue->next_minos[i] == INVALID_TETRIMINO) {
      queue->next_minos[i] = new_indexes[took];
      took++;
    }
  }
}

MinoQueue *queue_init(GameState *state)
{
  int i;
  int indexes[TETRIMINO_COUNT] = {0, 1, 2, 3, 4, 5, 6};
  shuffle(&indexes[0], TETRIMINO_COUNT);

  MinoQueue *queue = (MinoQueue *)malloc(sizeof(MinoQueue));

  for (i=0; i<=QUEUE_SIZE; i++) {
    queue->next_minos[i] = INVALID_TETRIMINO;
  }

  generate_next_batch(queue);
  generate_next_batch(queue);

  queue->game_state = state;
  queue->minos_served = 0;

  return queue;
}

Tetrimino *pop_mino(MinoQueue *queue)
{
  int i;
  int index = queue->next_minos[0];

  for (i = 0; i < QUEUE_SIZE; i++)
  {
    queue->next_minos[i] = queue->next_minos[i + 1];
  }

  queue->minos_served += 1;

  if (queue->minos_served % 7 == 0)
  {
    generate_next_batch(queue);
  }

  return tetrimino_generate(index, queue->game_state->speed);
}

void queue_draw(MinoQueue *queue)
{
  int i, index;
  Tetrimino *mino;

  for (i = 0; i < 3; i++)
  {
    int index = queue->next_minos[i];
    mino = tetrimino_generate(index, 0);
    tetrimino_draw(mino, 450, 80 + (120 * i));
  }
}
