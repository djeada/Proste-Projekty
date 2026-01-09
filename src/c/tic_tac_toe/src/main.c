#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tic_tac_toe.h"

int main() {
    Game game;
    int row, col;
    int vs_ai = 0;
    char choice;

    srand((unsigned int)time(NULL));

    printf("Tic-Tac-Toe\n");
    printf("Play against AI? (y/n): ");
    if (scanf(" %c", &choice) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    vs_ai = (choice == 'y' || choice == 'Y');

    game_init(&game);

    printf("\nEnter moves as: row col (0-2 each)\n");
    printf("Player X goes first.\n");

    while (!game.game_over) {
        print_board(&game);
        printf("Player %c's turn.\n", game.current_player);

        if (vs_ai && game.current_player == PLAYER_O) {
            printf("AI is thinking...\n");
            if (!ai_make_move(&game)) {
                printf("AI failed to make a move.\n");
                break;
            }
        } else {
            printf("Enter row and column: ");
            if (scanf("%d %d", &row, &col) != 2) {
                printf("Invalid input. Try again.\n");
                while (getchar() != '\n');
                continue;
            }

            if (!make_move(&game, row, col)) {
                printf("Invalid move. Try again.\n");
                continue;
            }
        }

        if (check_winner(&game)) {
            game.game_over = 1;
            game.winner = game.current_player;
            print_board(&game);
            printf("Player %c wins!\n", game.winner);
        } else if (is_board_full(&game)) {
            game.game_over = 1;
            print_board(&game);
            printf("It's a draw!\n");
        } else {
            switch_player(&game);
        }
    }

    return 0;
}
