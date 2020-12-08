#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// data points to array[y][x], upper left is 0,0
struct Board
{
  int width;
  int height;
  int *data;
};

struct Board *initialize_board(int width, int height, int seed)
{
  // seed the RNG
  srand(seed);

  // instantiate board
  struct Board *board = (struct Board *)malloc(sizeof(struct Board));

  board->width = width;
  board->height = height;
  board->data = malloc(sizeof(int) * width * height);

  int x, y;
  for (y = 0; y < height; y += 1)
    for (x = 0; x < width; x += 1)
      *(board->data + y * width + x) = rand() % 2;

  return board;
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
      printf("%c", get_cell(board, x, y) ? '#' : ' ');
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
  n += get_neighbor(board, x, y, -1, -1);
  n += get_neighbor(board, x, y, 0, -1);
  n += get_neighbor(board, x, y, 1, -1);
  n += get_neighbor(board, x, y, -1, 0);
  n += get_neighbor(board, x, y, 1, 0);
  n += get_neighbor(board, x, y, -1, 1);
  n += get_neighbor(board, x, y, 0, 1);
  n += get_neighbor(board, x, y, 1, 1);

  // for live cells...
  if (get_cell(board, x, y))
  {
    if (n < 2)
      return 0; // die of underpopulation
    if (n == 2 || n == 3)
      return 1; // live
    if (n > 3)
      return 0; // overpopulation
  }
  // for dead cells...
  else if (n == 3)
    return 1; // reproduction

  return 0; // this should never run
}

void tick(struct Board *board)
{
  int *nextData = malloc(sizeof(int) * board->width * board->height);
  int x, y;

  // build next state
  for (y = 0; y < board->height; y += 1)
    for (x = 0; x < board->width; x += 1)
      *(nextData + y * board->width + x) = get_next_cell_state(board, x, y);

  // make next state into current state
  // free up the memory for the old state
  int *oldData = board->data;
  board->data = nextData;
  free(oldData);
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
  int width = 40;
  int height = 20;
  int seed = 100;

  // initialize board
  struct Board *board = initialize_board(width, height, seed);

  // main loop
  mainLoop(board, 20);

  // clean up
  free(board->data);
  free(board);

  return 0;
}
