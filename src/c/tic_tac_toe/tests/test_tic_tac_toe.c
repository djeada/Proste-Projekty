/* Tests of the rules. Each test returns nothing and stops the program with assert() on failure. */
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "tic_tac_toe.h"

/* Builds a board from 9 characters: 'X', 'O' or '.' for an empty cell. */
static Board parse(const char *text)
{
    Board board;
    int cell;
    for (cell = 0; cell < 9; cell++) {
        board.cells[cell] = text[cell] == 'X' ? X : text[cell] == 'O' ? O : EMPTY;
    }
    return board;
}

static void test_new_board_has_nine_legal_moves(void)
{
    Board board = board_new();
    int moves[9];
    assert(board_legal_moves(&board, moves) == 9);
    assert(board_winner(&board) == EMPTY);
    assert(!board_is_over(&board));
}

static void test_play_rejects_taken_and_invalid_cells(void)
{
    Board board = board_new();
    assert(board_play(&board, 4, X));
    assert(!board_play(&board, 4, O));
    assert(board.cells[4] == X);
    assert(!board_play(&board, -1, O));
    assert(!board_play(&board, 9, O));
}

static void test_detects_every_kind_of_line(void)
{
    static const char *wins[] = {"XXX......", "...XXX...", "......XXX",
                                 "X..X..X..", ".X..X..X.", "..X..X..X",
                                 "X...X...X", "..X.X.X.."};
    static const int first_cell[] = {0, 3, 6, 0, 1, 2, 0, 2};
    int i;
    for (i = 0; i < 8; i++) {
        Board board = parse(wins[i]);
        const int *line = board_winning_line(&board);
        assert(board_winner(&board) == X);
        assert(line != NULL && line[0] == first_cell[i]);
    }
}

static void test_open_game_has_no_winner(void)
{
    Board board = parse("XO.......");
    assert(board_winning_line(&board) == NULL);
    assert(board_winner(&board) == EMPTY);
    assert(!board_is_over(&board));
}

static void test_full_board_without_winner_is_draw(void)
{
    Board board = parse("XOXXOOOXX");
    assert(board_is_full(&board));
    assert(board_is_draw(&board));
    assert(board_is_over(&board));
}

static void test_full_board_with_winner_is_not_draw(void)
{
    Board board = parse("XXXOOXOXO");
    assert(board_winner(&board) == X);
    assert(!board_is_draw(&board));
    assert(board_is_over(&board));
}

static void test_computer_takes_immediate_win(void)
{
    /* X has 0 and 1, O has 3 and 4: O wins on cell 5. */
    Board board = parse("XX.OO....");
    assert(board_best_move(&board, O) == 5);
}

static void test_computer_blocks_immediate_loss(void)
{
    /* X threatens 0-1-2. O cannot win at once, so it must block cell 2. */
    Board board = parse("XX..O...O");
    assert(board_best_move(&board, O) == 2);
}

static void test_two_computers_always_draw(void)
{
    Board board = board_new();
    Mark turn = X;
    while (!board_is_over(&board)) {
        int cell = board_best_move(&board, turn);
        assert(board_play(&board, cell, turn));
        turn = other_mark(turn);
    }
    assert(board_is_draw(&board));
}

static void test_best_move_on_full_board_is_minus_one(void)
{
    Board board = parse("XOXXOOOXX");
    assert(board_best_move(&board, X) == -1);
}

int main(void)
{
    test_new_board_has_nine_legal_moves();
    test_play_rejects_taken_and_invalid_cells();
    test_detects_every_kind_of_line();
    test_open_game_has_no_winner();
    test_full_board_without_winner_is_draw();
    test_full_board_with_winner_is_not_draw();
    test_computer_takes_immediate_win();
    test_computer_blocks_immediate_loss();
    test_two_computers_always_draw();
    test_best_move_on_full_board_is_minus_one();
    printf("All tic_tac_toe tests passed.\n");
    return 0;
}
