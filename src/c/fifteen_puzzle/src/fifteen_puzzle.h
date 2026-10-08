/* Fifteen puzzle rules: no input, no output. */
#ifndef FIFTEEN_PUZZLE_H
#define FIFTEEN_PUZZLE_H

#define SIZE 4
#define CELLS (SIZE * SIZE)

/* tiles[i] is the tile in cell i (row by row); 0 is the gap. */
typedef struct {
    int tiles[CELLS];
} Board;

typedef enum { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT } Direction;

/* A small random generator, so that a seed gives the same shuffle every time. */
typedef struct {
    unsigned int state;
} Rng;

void rng_seed(Rng *rng, unsigned int seed);
int rng_below(Rng *rng, int n);

void board_reset(Board *board);
int board_gap(const Board *board);
int board_slide(Board *board, int cell);
int board_move_gap(Board *board, Direction dir);
void board_shuffle(Board *board, Rng *rng);
int board_is_solved(const Board *board);
int board_is_solvable(const Board *board);

#endif
