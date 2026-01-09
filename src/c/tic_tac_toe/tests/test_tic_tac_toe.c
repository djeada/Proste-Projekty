#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "../src/tic_tac_toe.h"

void test_game_init() {
    Game game;
    game_init(&game);

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            assert(game.cells[i][j] == EMPTY);
        }
    }
    assert(game.current_player == PLAYER_X);
    assert(game.game_over == 0);
    assert(game.winner == EMPTY);
}

void test_make_move() {
    Game game;
    game_init(&game);

    assert(make_move(&game, 0, 0) == 1);
    assert(game.cells[0][0] == PLAYER_X);

    // Invalid move - cell occupied
    assert(make_move(&game, 0, 0) == 0);

    // Invalid move - out of bounds
    assert(make_move(&game, -1, 0) == 0);
    assert(make_move(&game, 0, 3) == 0);
}

void test_check_winner_row() {
    Game game;
    game_init(&game);

    game.cells[0][0] = PLAYER_X;
    game.cells[0][1] = PLAYER_X;
    game.cells[0][2] = PLAYER_X;

    assert(check_winner(&game) == 1);
}

void test_check_winner_col() {
    Game game;
    game_init(&game);

    game.cells[0][0] = PLAYER_O;
    game.cells[1][0] = PLAYER_O;
    game.cells[2][0] = PLAYER_O;

    assert(check_winner(&game) == 1);
}

void test_check_winner_diagonal() {
    Game game;
    game_init(&game);

    game.cells[0][0] = PLAYER_X;
    game.cells[1][1] = PLAYER_X;
    game.cells[2][2] = PLAYER_X;

    assert(check_winner(&game) == 1);
}

void test_no_winner() {
    Game game;
    game_init(&game);

    game.cells[0][0] = PLAYER_X;
    game.cells[0][1] = PLAYER_O;
    game.cells[0][2] = PLAYER_X;

    assert(check_winner(&game) == 0);
}

void test_is_board_full() {
    Game game;
    game_init(&game);

    assert(is_board_full(&game) == 0);

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            game.cells[i][j] = PLAYER_X;
        }
    }

    assert(is_board_full(&game) == 1);
}

void test_switch_player() {
    Game game;
    game_init(&game);

    assert(game.current_player == PLAYER_X);
    switch_player(&game);
    assert(game.current_player == PLAYER_O);
    switch_player(&game);
    assert(game.current_player == PLAYER_X);
}

int main() {
    test_game_init();
    test_make_move();
    test_check_winner_row();
    test_check_winner_col();
    test_check_winner_diagonal();
    test_no_winner();
    test_is_board_full();
    test_switch_player();
    printf("All tests passed!\n");
    return 0;
}
