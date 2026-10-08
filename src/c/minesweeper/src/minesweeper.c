#include "minesweeper.h"

#include <stdlib.h>
#include <string.h>

/* xorshift32: a small generator that gives the same numbers for the same seed. */
static unsigned int next_random(unsigned int *state) {
    unsigned int x = *state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    *state = x;
    return x;
}

static int in_board(const Game *game, int row, int col) {
    return row >= 0 && row < game->rows && col >= 0 && col < game->cols;
}

static int is_near(int row, int col, int other_row, int other_col) {
    return abs(row - other_row) <= 1 && abs(col - other_col) <= 1;
}

static int count_mines_around(const Game *game, int row, int col) {
    int count = 0;
    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            if ((dr != 0 || dc != 0) && in_board(game, row + dr, col + dc)) {
                count += game->mine[row + dr][col + dc];
            }
        }
    }
    return count;
}

/* Places the mines at random, skipping the first clicked cell and its neighbors. */
static void place_mines(Game *game, int safe_row, int safe_col) {
    int placed = 0;
    while (placed < game->mine_total) {
        int row = (int)(next_random(&game->rng) % (unsigned int)game->rows);
        int col = (int)(next_random(&game->rng) % (unsigned int)game->cols);
        if (game->mine[row][col] || is_near(row, col, safe_row, safe_col)) {
            continue;
        }
        game->mine[row][col] = 1;
        placed++;
    }
    for (int row = 0; row < game->rows; row++) {
        for (int col = 0; col < game->cols; col++) {
            game->neighbors[row][col] = count_mines_around(game, row, col);
        }
    }
    game->mines_placed = 1;
}

/* Opens a cell. An empty cell (no mines around) also opens its neighbors: flood fill. */
static void reveal_cell(Game *game, int row, int col) {
    if (!in_board(game, row, col) || game->revealed[row][col] || game->flagged[row][col]) {
        return;
    }
    game->revealed[row][col] = 1;
    game->revealed_count++;
    if (game->neighbors[row][col] == 0) {
        for (int dr = -1; dr <= 1; dr++) {
            for (int dc = -1; dc <= 1; dc++) {
                reveal_cell(game, row + dr, col + dc);
            }
        }
    }
}

void game_start(Game *game, int rows, int cols, int mines, unsigned int seed) {
    game->rows = rows;
    game->cols = cols;
    game->mine_total = mines;
    game->mines_placed = 0;
    game->revealed_count = 0;
    game->flag_count = 0;
    game->rng = seed ? seed : 1;
    game->state = PLAYING;
    memset(game->mine, 0, sizeof game->mine);
    memset(game->neighbors, 0, sizeof game->neighbors);
    memset(game->revealed, 0, sizeof game->revealed);
    memset(game->flagged, 0, sizeof game->flagged);
}

void game_reveal(Game *game, int row, int col) {
    if (game->state != PLAYING || !in_board(game, row, col)) {
        return;
    }
    if (game->revealed[row][col] || game->flagged[row][col]) {
        return;
    }
    if (!game->mines_placed) {
        place_mines(game, row, col);
    }
    if (game->mine[row][col]) {
        for (int r = 0; r < game->rows; r++) {
            for (int c = 0; c < game->cols; c++) {
                if (game->mine[r][c]) {
                    game->revealed[r][c] = 1;
                }
            }
        }
        game->state = LOST;
        return;
    }
    reveal_cell(game, row, col);
    if (game->revealed_count == game->rows * game->cols - game->mine_total) {
        game->state = WON;
    }
}

void game_toggle_flag(Game *game, int row, int col) {
    if (game->state != PLAYING || !in_board(game, row, col) || game->revealed[row][col]) {
        return;
    }
    game->flagged[row][col] = !game->flagged[row][col];
    game->flag_count += game->flagged[row][col] ? 1 : -1;
}

int game_mines_left(const Game *game) {
    return game->mine_total - game->flag_count;
}
