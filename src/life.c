#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// data points to array[y][x], upper left is 0,0
struct Board
{
  int width;
  int height;
  int *data;
  int *nextData;
};

struct Board *initialize_board(int width, int height, int seed)
{
  // pointer to board
  struct Board *board;

  // seed the RNG
  srand(seed);

  // allocate board
  board = malloc(sizeof(struct Board));
  if (board == NULL)
    goto error;

  board->width = width;
  board->height = height;
  // TODO: find some way to do bit packing here?
  board->data = malloc(sizeof(int) * width * height);
  board->nextData = malloc(sizeof(int) * width * height);

  if (board->data == NULL || board->nextData == NULL)
    goto error;

  int x, y;
  for (y = 0; y < height; y += 1)
    for (x = 0; x < width; x += 1)
      *(board->data + y * width + x) = rand() % 2;

  return board;

error:
  printf("Not enough memory!");
  exit(1);
}

int get_cell(struct Board *board, int x, int y)
{
  return *(board->data + y * board->width + x);
}

void print_board(struct Board *board)
{
  int x, y;
  for (y = 0; y < board->height; y += 1)
  {
    for (x = 0; x < board->width; x += 1)
    {
      int count = get_cell(board, x, y);
      if (count == 2)
        printf("%s", "\U0001F421");
      else if (count == 3)
        printf("%s", "\U0001F419");
      else
        printf("%s", "\U0001F30A");
    }
    printf("\n");
  }
}

void clear()
{
  printf("\e[1;1H\e[2J");
}

void delay(int ms)
{
  clock_t then = clock();
  while ((clock() - then) < ms * (CLOCKS_PER_SEC / 1000))
    ;
}

int is_in_bounds(int width, int height, int x, int y)
{
  return (x >= 0 && x < width && y >= 0 && y < height);
}

int get_neighbor(struct Board *board, int x, int y, int dx, int dy)
{
  return is_in_bounds(board->width, board->height, x + dx, y + dy) ? get_cell(board, x + dx, y + dy) : 0;
}

int get_next_cell_state(struct Board *board, int x, int y)
{
  int n = 0; // neighbor count
  n += get_neighbor(board, x, y, -1, -1) > 0;
  n += get_neighbor(board, x, y, 0, -1) > 0;
  n += get_neighbor(board, x, y, 1, -1) > 0;
  n += get_neighbor(board, x, y, -1, 0) > 0;
  n += get_neighbor(board, x, y, 1, 0) > 0;
  n += get_neighbor(board, x, y, -1, 1) > 0;
  n += get_neighbor(board, x, y, 0, 1) > 0;
  n += get_neighbor(board, x, y, 1, 1) > 0;

  // for live cells...
  if (get_cell(board, x, y))
  {
    if (n < 2)
      return 0; // die of underpopulation
    if (n == 2 || n == 3)
      return n; // live
    if (n > 3)
      return 0; // overpopulation
  }
  // for dead cells...
  else if (n == 3)
    return n; // reproduction

  return 0; // this should never run
}

void tick(struct Board *board)
{
  int x, y;

  // build next state
  for (y = 0; y < board->height; y += 1)
    for (x = 0; x < board->width; x += 1)
      *(board->nextData + y * board->width + x) = get_next_cell_state(board, x, y);

  // flip the pointers between nextData and data
  int *oldData = board->data;
  board->data = board->nextData;
  board->nextData = oldData;
}

void mainLoop(struct Board *board, int delayMs)
{
  while (1)
  {
    clear();
    print_board(board);
    tick(board);
    delay(delayMs);
  }
}

int main(int argc, char *argv[])
{
  int width = 20;
  int height = 20;
  int seed = 1;

  // initialize board
  struct Board *board = initialize_board(width, height, seed);

  // main loop
  mainLoop(board, 60);

  // clean up
  free(board->data);
  free(board->nextData);
  free(board);

  return 0;
}
