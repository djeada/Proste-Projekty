#include "fifteen.h"
#include <stdio.h>

int main() {
    FifteenPuzzle puzzle;
    puzzle_init(&puzzle);
    puzzle_shuffle(&puzzle);
    printf("Welcome to the 15 Puzzle!\nUse w/a/s/d to move tiles.\n\n");
    while (!puzzle_is_solved(&puzzle)) {
        puzzle_print(&puzzle);
        printf("Move: ");
        char move;
        scanf(" %c", &move);
        if (!puzzle_move(&puzzle, move)) {
            printf("Invalid move!\n");
        }
    }
    printf("Congratulations! You solved the puzzle!\n");
    return 0;
}
