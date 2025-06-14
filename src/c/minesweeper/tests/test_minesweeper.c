#include "../src/minesweeper.h"
#include <assert.h>
#include <stdio.h>

void test_game_init_and_reveal() {
    MinesweeperGame game;
    game_init(&game);
    int revealed = 0;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (!game.board[i][j].is_mine) {
                game_reveal(&game, i, j);
                revealed = 1;
                break;
            }
        }
        if (revealed) break;
    }
    assert(game.revealed_count > 0);
}

void test_game_flag() {
    MinesweeperGame game;
    game_init(&game);
    game_flag(&game, 0, 0);
    assert(game.board[0][0].is_flagged);
    game_flag(&game, 0, 0);
    assert(!game.board[0][0].is_flagged);
}

int main() {
    test_game_init_and_reveal();
    test_game_flag();
    printf("Minesweeper logic tests passed.\n");
    return 0;
}
