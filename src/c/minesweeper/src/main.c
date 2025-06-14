#include "minesweeper.h"
#include <stdio.h>

int main() {
    MinesweeperGame game;
    game_init(&game);
    printf("Welcome to Minesweeper!\nEnter r c to reveal, f r c to flag.\n\n");
    while (!game.game_over && !game.win) {
        game_print(&game);
        char cmd;
        int r, c;
        printf("Move: ");
        int n = scanf(" %c %d %d", &cmd, &r, &c);
        if (n < 3) {
            printf("Invalid input!\n");
            while (getchar() != '\n');
            continue;
        }
        if (cmd == 'f') game_flag(&game, r, c);
        else game_reveal(&game, r, c);
    }
    game_print(&game);
    if (game.win) printf("Congratulations! You won!\n");
    else printf("Game over!\n");
    return 0;
}
