#include "fifteen.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void puzzle_init(FifteenPuzzle *puzzle) {
    int value = 1;
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            puzzle->board[i][j] = value;
            value++;
        }
    }
    puzzle->board[SIZE-1][SIZE-1] = 0;
    puzzle->empty_row = SIZE-1;
    puzzle->empty_col = SIZE-1;
}

void puzzle_shuffle(FifteenPuzzle *puzzle) {
    srand((unsigned)time(NULL));
    for (int i = 0; i < 1000; ++i) {
        char moves[] = {'w', 'a', 's', 'd'};
        puzzle_move(puzzle, moves[rand() % 4]);
    }
}

int puzzle_move(FifteenPuzzle *puzzle, char move) {
    int dr = 0, dc = 0;
    if (move == 'w') dr = 1;
    else if (move == 's') dr = -1;
    else if (move == 'a') dc = 1;
    else if (move == 'd') dc = -1;
    else return 0;
    int new_row = puzzle->empty_row + dr;
    int new_col = puzzle->empty_col + dc;
    if (new_row < 0 || new_row >= SIZE || new_col < 0 || new_col >= SIZE) return 0;
    puzzle->board[puzzle->empty_row][puzzle->empty_col] = puzzle->board[new_row][new_col];
    puzzle->board[new_row][new_col] = 0;
    puzzle->empty_row = new_row;
    puzzle->empty_col = new_col;
    return 1;
}

int puzzle_is_solved(const FifteenPuzzle *puzzle) {
    int value = 1;
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (i == SIZE-1 && j == SIZE-1) {
                if (puzzle->board[i][j] != 0) return 0;
            } else {
                if (puzzle->board[i][j] != value) return 0;
                value++;
            }
        }
    }
    return 1;
}

void puzzle_print(const FifteenPuzzle *puzzle) {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (puzzle->board[i][j] == 0) printf("   ");
            else printf("%2d ", puzzle->board[i][j]);
        }
        printf("\n");
    }
}
