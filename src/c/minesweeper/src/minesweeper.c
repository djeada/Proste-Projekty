#include "minesweeper.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void game_init(MinesweeperGame *game) {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            game->board[i][j].is_mine = 0;
            game->board[i][j].is_revealed = 0;
            game->board[i][j].is_flagged = 0;
            game->board[i][j].adjacent_mines = 0;
        }
    }
    game->revealed_count = 0;
    game->flagged_count = 0;
    game->game_over = 0;
    game->win = 0;
    srand((unsigned)time(NULL));
    int placed = 0;
    while (placed < MINE_COUNT) {
        int r = rand() % BOARD_SIZE;
        int c = rand() % BOARD_SIZE;
        if (!game->board[r][c].is_mine) {
            game->board[r][c].is_mine = 1;
            placed++;
        }
    }
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (game->board[i][j].is_mine) continue;
            int count = 0;
            for (int dr = -1; dr <= 1; ++dr) {
                for (int dc = -1; dc <= 1; ++dc) {
                    int ni = i + dr, nj = j + dc;
                    if (ni >= 0 && ni < BOARD_SIZE && nj >= 0 && nj < BOARD_SIZE && game->board[ni][nj].is_mine) count++;
                }
            }
            game->board[i][j].adjacent_mines = count;
        }
    }
}

void game_reveal(MinesweeperGame *game, int row, int col) {
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) return;
    Cell *cell = &game->board[row][col];
    if (cell->is_revealed || cell->is_flagged) return;
    cell->is_revealed = 1;
    game->revealed_count++;
    if (cell->is_mine) {
        game->game_over = 1;
        return;
    }
    if (cell->adjacent_mines == 0) {
        for (int dr = -1; dr <= 1; ++dr) {
            for (int dc = -1; dc <= 1; ++dc) {
                if (dr != 0 || dc != 0) game_reveal(game, row + dr, col + dc);
            }
        }
    }
    if (game->revealed_count == BOARD_SIZE * BOARD_SIZE - MINE_COUNT) game->win = 1;
}

void game_flag(MinesweeperGame *game, int row, int col) {
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) return;
    Cell *cell = &game->board[row][col];
    if (cell->is_revealed) return;
    cell->is_flagged = !cell->is_flagged;
    if (cell->is_flagged) game->flagged_count++;
    else game->flagged_count--;
}

void game_print(const MinesweeperGame *game) {
    printf("   ");
    for (int j = 0; j < BOARD_SIZE; ++j) printf("%2d ", j);
    printf("\n");
    for (int i = 0; i < BOARD_SIZE; ++i) {
        printf("%2d ", i);
        for (int j = 0; j < BOARD_SIZE; ++j) {
            const Cell *cell = &game->board[i][j];
            if (cell->is_flagged) printf(" F ");
            else if (!cell->is_revealed) printf(" . ");
            else if (cell->is_mine) printf(" * ");
            else if (cell->adjacent_mines > 0) printf(" %d ", cell->adjacent_mines);
            else printf("   ");
        }
        printf("\n");
    }
}
