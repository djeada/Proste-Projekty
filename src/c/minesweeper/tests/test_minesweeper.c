/* Tests of the rules in minesweeper.c. Returns 0 when all tests pass. */
#include "minesweeper.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

static int count_all_mines(const Game *game) {
    int count = 0;
    for (int r = 0; r < game->rows; r++) {
        for (int c = 0; c < game->cols; c++) {
            count += game->mine[r][c];
        }
    }
    return count;
}

static void test_start_has_no_mines_yet(void) {
    Game game;
    game_start(&game, 9, 9, 10, 42);
    assert(game.state == PLAYING);
    assert(game.mines_placed == 0);
    assert(count_all_mines(&game) == 0);
    assert(game_mines_left(&game) == 10);
}

static void test_first_reveal_is_never_a_mine_or_next_to_one(void) {
    for (unsigned int seed = 1; seed <= 50; seed++) {
        Game game;
        game_start(&game, 9, 9, 10, seed);
        game_reveal(&game, 0, 0);
        assert(game.state == PLAYING);
        assert(count_all_mines(&game) == 10);
        for (int r = 0; r <= 1; r++) {
            for (int c = 0; c <= 1; c++) {
                assert(!game.mine[r][c]);
            }
        }
    }
}

static void test_first_reveal_in_middle_of_expert_board(void) {
    Game game;
    game_start(&game, 16, 30, 99, 7);
    game_reveal(&game, 8, 15);
    assert(count_all_mines(&game) == 99);
    for (int r = 7; r <= 9; r++) {
        for (int c = 14; c <= 16; c++) {
            assert(!game.mine[r][c]);
        }
    }
}

static void test_same_seed_gives_same_layout(void) {
    Game first;
    Game second;
    game_start(&first, 16, 16, 40, 123);
    game_start(&second, 16, 16, 40, 123);
    game_reveal(&first, 3, 3);
    game_reveal(&second, 3, 3);
    assert(memcmp(first.mine, second.mine, sizeof first.mine) == 0);
}

static void test_neighbor_counts_match_mines(void) {
    Game game;
    game_start(&game, 16, 16, 40, 99);
    game_reveal(&game, 0, 0);
    for (int r = 0; r < game.rows; r++) {
        for (int c = 0; c < game.cols; c++) {
            int expected = 0;
            for (int dr = -1; dr <= 1; dr++) {
                for (int dc = -1; dc <= 1; dc++) {
                    int nr = r + dr;
                    int nc = c + dc;
                    if (nr >= 0 && nr < game.rows && nc >= 0 && nc < game.cols) {
                        expected += game.mine[nr][nc];
                    }
                }
            }
            if (!game.mine[r][c]) {
                assert(game.neighbors[r][c] == expected);
            }
        }
    }
}

static void test_flood_fill_opens_empty_area(void) {
    /* 5x5 board with one mine in the top-right corner: all 24 safe cells are reachable. */
    Game game;
    game_start(&game, 5, 5, 1, 1);
    game.mines_placed = 1;
    game.mine[0][4] = 1;
    game.neighbors[0][3] = 1;
    game.neighbors[1][3] = 1;
    game.neighbors[1][4] = 1;

    game_reveal(&game, 4, 0);
    assert(game.revealed_count == 24);
    assert(game.state == WON);
    assert(!game.revealed[0][4]);
    assert(game.revealed[0][3]);
}

static void test_flood_fill_stops_at_numbers(void) {
    /* Mine in the middle of a 3x3 board: the zero cells are not all open, the numbers stop. */
    Game game;
    game_start(&game, 3, 3, 1, 1);
    game.mines_placed = 1;
    game.mine[1][1] = 1;
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            if (!game.mine[r][c]) {
                game.neighbors[r][c] = 1;
            }
        }
    }

    game_reveal(&game, 0, 0);
    assert(game.revealed_count == 1);
    assert(game.state == PLAYING);
}

static void test_revealing_a_mine_loses_and_shows_all_mines(void) {
    Game game;
    game_start(&game, 9, 9, 10, 5);
    game_reveal(&game, 4, 4);
    int row = -1;
    int col = -1;
    for (int r = 0; r < 9 && row < 0; r++) {
        for (int c = 0; c < 9; c++) {
            if (game.mine[r][c]) {
                row = r;
                col = c;
                break;
            }
        }
    }
    assert(row >= 0);
    game_reveal(&game, row, col);
    assert(game.state == LOST);
    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            if (game.mine[r][c]) {
                assert(game.revealed[r][c]);
            }
        }
    }
}

static void test_revealing_all_safe_cells_wins(void) {
    Game game;
    game_start(&game, 9, 9, 10, 77);
    game_reveal(&game, 4, 4);
    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            if (!game.mine[r][c]) {
                game_reveal(&game, r, c);
            }
        }
    }
    assert(game.state == WON);
    assert(game.revealed_count == 71);
}

static void test_flags_count_down_and_toggle(void) {
    Game game;
    game_start(&game, 9, 9, 10, 3);
    game_toggle_flag(&game, 0, 0);
    game_toggle_flag(&game, 8, 8);
    assert(game.flagged[0][0]);
    assert(game_mines_left(&game) == 8);
    game_toggle_flag(&game, 0, 0);
    assert(!game.flagged[0][0]);
    assert(game_mines_left(&game) == 9);
}

static void test_flagged_cell_cannot_be_revealed(void) {
    Game game;
    game_start(&game, 9, 9, 10, 3);
    game_toggle_flag(&game, 2, 2);
    game_reveal(&game, 2, 2);
    assert(game.mines_placed == 0);
    assert(game.revealed_count == 0);
}

static void test_flag_on_revealed_cell_is_ignored(void) {
    Game game;
    game_start(&game, 9, 9, 10, 3);
    game_reveal(&game, 4, 4);
    int revealed_before = game.revealed_count;
    game_toggle_flag(&game, 4, 4);
    assert(!game.flagged[4][4]);
    assert(game.revealed_count == revealed_before);
}

static void test_out_of_board_moves_are_ignored(void) {
    Game game;
    game_start(&game, 9, 9, 10, 3);
    game_reveal(&game, -1, 0);
    game_reveal(&game, 9, 9);
    game_toggle_flag(&game, 0, 30);
    assert(game.state == PLAYING);
    assert(game.mines_placed == 0);
    assert(game.flag_count == 0);
}

int main(void) {
    test_start_has_no_mines_yet();
    test_first_reveal_is_never_a_mine_or_next_to_one();
    test_first_reveal_in_middle_of_expert_board();
    test_same_seed_gives_same_layout();
    test_neighbor_counts_match_mines();
    test_flood_fill_opens_empty_area();
    test_flood_fill_stops_at_numbers();
    test_revealing_a_mine_loses_and_shows_all_mines();
    test_revealing_all_safe_cells_wins();
    test_flags_count_down_and_toggle();
    test_flagged_cell_cannot_be_revealed();
    test_flag_on_revealed_cell_is_ignored();
    test_out_of_board_moves_are_ignored();
    printf("All minesweeper logic tests passed.\n");
    return 0;
}
