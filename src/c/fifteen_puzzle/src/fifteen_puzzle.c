/* Fifteen puzzle rules: moves, shuffle, solved and solvable checks. */
#include "fifteen_puzzle.h"

#include <stdlib.h>

#define SHUFFLE_MOVES 500

void rng_seed(Rng *rng, unsigned int seed) {
    rng->state = seed;
}

/* Linear congruential generator; uses bits 16-30 because the low bits are weak. */
int rng_below(Rng *rng, int n) {
    rng->state = rng->state * 1103515245u + 12345u;
    return (int)((rng->state >> 16) & 0x7fff) % n;
}

void board_reset(Board *board) {
    for (int i = 0; i < CELLS - 1; i++) {
        board->tiles[i] = i + 1;
    }
    board->tiles[CELLS - 1] = 0;
}

int board_gap(const Board *board) {
    for (int i = 0; i < CELLS; i++) {
        if (board->tiles[i] == 0) {
            return i;
        }
    }
    return -1;
}

/* The tile in `cell` slides into the gap if they are next to each other. */
int board_slide(Board *board, int cell) {
    int gap = board_gap(board);
    if (cell < 0 || cell >= CELLS || cell == gap) {
        return 0;
    }
    int dr = cell / SIZE - gap / SIZE;
    int dc = cell % SIZE - gap % SIZE;
    if (abs(dr) + abs(dc) != 1) {
        return 0;
    }
    board->tiles[gap] = board->tiles[cell];
    board->tiles[cell] = 0;
    return 1;
}

/* The gap moves in `dir`: the tile on that side slides into it. */
int board_move_gap(Board *board, Direction dir) {
    static const int row_step[] = {-1, 1, 0, 0};
    static const int col_step[] = {0, 0, -1, 1};
    int gap = board_gap(board);
    int row = gap / SIZE + row_step[dir];
    int col = gap % SIZE + col_step[dir];
    if (row < 0 || row >= SIZE || col < 0 || col >= SIZE) {
        return 0;
    }
    return board_slide(board, row * SIZE + col);
}

/* Random legal moves from the solved board, so every result can be solved. */
void board_shuffle(Board *board, Rng *rng) {
    do {
        board_reset(board);
        for (int moves = 0; moves < SHUFFLE_MOVES;) {
            moves += board_move_gap(board, (Direction)rng_below(rng, 4));
        }
    } while (board_is_solved(board));
}

int board_is_solved(const Board *board) {
    Board solved;
    board_reset(&solved);
    for (int i = 0; i < CELLS; i++) {
        if (board->tiles[i] != solved.tiles[i]) {
            return 0;
        }
    }
    return 1;
}

/*
 * On a board with an even width, the puzzle is solvable when the number of
 * inversions (pairs of tiles out of order, the gap ignored) plus the gap's
 * row counted from the top (0-based) is odd.
 */
int board_is_solvable(const Board *board) {
    int inversions = 0;
    for (int i = 0; i < CELLS; i++) {
        for (int j = i + 1; j < CELLS; j++) {
            if (board->tiles[i] != 0 && board->tiles[j] != 0 && board->tiles[i] > board->tiles[j]) {
                inversions++;
            }
        }
    }
    return (inversions + board_gap(board) / SIZE) % 2 == 1;
}
