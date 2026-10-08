/* Rules of tic-tac-toe: the board, legal moves, winner, draw and the computer player. */
#ifndef TIC_TAC_TOE_H
#define TIC_TAC_TOE_H

#include <stdbool.h>

/* Cells are numbered 0..8, row by row:
 *   0 1 2
 *   3 4 5
 *   6 7 8
 */
typedef enum { EMPTY, X, O } Mark;

typedef struct {
    Mark cells[9];
} Board;

Board board_new(void);
Mark other_mark(Mark mark);

/* Places mark on an empty cell. Returns false if the cell is taken or out of range. */
bool board_play(Board *board, int cell, Mark mark);

/* Writes the empty cells into moves and returns how many there are. */
int board_legal_moves(const Board *board, int moves[9]);

/* Returns the three cells of a complete line, or NULL if nobody has three in a row. */
const int *board_winning_line(const Board *board);

/* Returns X or O for the winner, or EMPTY if there is none. */
Mark board_winner(const Board *board);

bool board_is_full(const Board *board);
bool board_is_over(const Board *board);
bool board_is_draw(const Board *board);

/* Returns the best cell for mark according to minimax, or -1 if the board is full. */
int board_best_move(const Board *board, Mark mark);

#endif
