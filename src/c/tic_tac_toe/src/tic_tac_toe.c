#include "tic_tac_toe.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void game_init(Game *game) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            game->cells[i][j] = EMPTY;
        }
    }
    game->current_player = PLAYER_X;
    game->game_over = 0;
    game->winner = EMPTY;
}

int make_move(Game *game, int row, int col) {
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
        return 0;
    }
    if (game->cells[row][col] != EMPTY) {
        return 0;
    }
    if (game->game_over) {
        return 0;
    }
    game->cells[row][col] = game->current_player;
    return 1;
}

int check_winner(const Game *game) {
    // Check rows
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (game->cells[i][0] != EMPTY &&
            game->cells[i][0] == game->cells[i][1] &&
            game->cells[i][1] == game->cells[i][2]) {
            return 1;
        }
    }
    // Check columns
    for (int j = 0; j < BOARD_SIZE; j++) {
        if (game->cells[0][j] != EMPTY &&
            game->cells[0][j] == game->cells[1][j] &&
            game->cells[1][j] == game->cells[2][j]) {
            return 1;
        }
    }
    // Check diagonals
    if (game->cells[0][0] != EMPTY &&
        game->cells[0][0] == game->cells[1][1] &&
        game->cells[1][1] == game->cells[2][2]) {
        return 1;
    }
    if (game->cells[0][2] != EMPTY &&
        game->cells[0][2] == game->cells[1][1] &&
        game->cells[1][1] == game->cells[2][0]) {
        return 1;
    }
    return 0;
}

int is_board_full(const Game *game) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (game->cells[i][j] == EMPTY) {
                return 0;
            }
        }
    }
    return 1;
}

void switch_player(Game *game) {
    game->current_player = (game->current_player == PLAYER_X) ? PLAYER_O : PLAYER_X;
}

void print_board(const Game *game) {
    printf("\n");
    for (int i = 0; i < BOARD_SIZE; i++) {
        printf(" %c | %c | %c \n", game->cells[i][0], game->cells[i][1], game->cells[i][2]);
        if (i < BOARD_SIZE - 1) {
            printf("-----------\n");
        }
    }
    printf("\n");
}

int ai_make_move(Game *game) {
    // Simple AI: pick a random empty cell
    int empty_cells[BOARD_SIZE * BOARD_SIZE][2];
    int count = 0;

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (game->cells[i][j] == EMPTY) {
                empty_cells[count][0] = i;
                empty_cells[count][1] = j;
                count++;
            }
        }
    }

    if (count == 0) {
        return 0;
    }

    int choice = rand() % count;
    return make_move(game, empty_cells[choice][0], empty_cells[choice][1]);
}
