"""Tests of the rules: no display, no input, no network."""

import pytest

from tic_tac_toe import (
    EMPTY, best_move, is_draw, is_over, legal_moves, new_board, other_mark, play, winner, winning_line,
)


def parse(text):
    """Build a board from 9 characters: 'X', 'O' or '.' for an empty cell."""
    return [EMPTY if char == "." else char for char in text]


def test_new_board_has_nine_legal_moves():
    board = new_board()
    assert legal_moves(board) == list(range(9))
    assert winner(board) is None
    assert not is_over(board)


def test_play_puts_mark_on_a_copy():
    board = new_board()
    after = play(board, 4, "X")
    assert after[4] == "X"
    assert board[4] == EMPTY


def test_play_rejects_taken_cell():
    board = play(new_board(), 4, "X")
    with pytest.raises(ValueError):
        play(board, 4, "O")


@pytest.mark.parametrize(
    "text, line",
    [
        ("XXX......", (0, 1, 2)),
        ("...XXX...", (3, 4, 5)),
        ("......XXX", (6, 7, 8)),
        ("X..X..X..", (0, 3, 6)),
        (".X..X..X.", (1, 4, 7)),
        ("..X..X..X", (2, 5, 8)),
        ("X...X...X", (0, 4, 8)),
        ("..X.X.X..", (2, 4, 6)),
    ],
)
def test_detects_every_kind_of_line(text, line):
    board = parse(text)
    assert winning_line(board) == line
    assert winner(board) == "X"
    assert is_over(board)


def test_open_game_has_no_winner():
    board = parse("XO.......")
    assert winning_line(board) is None
    assert winner(board) is None
    assert not is_over(board)


def test_full_board_without_winner_is_draw():
    board = parse("XOXXOOOXX")
    assert is_draw(board)
    assert is_over(board)


def test_full_board_with_winner_is_not_draw():
    board = parse("XXXOOXOXO")
    assert winner(board) == "X"
    assert not is_draw(board)
    assert is_over(board)


def test_computer_takes_immediate_win():
    # X has 0 and 1, O has 3 and 4: O wins on cell 5.
    assert best_move(parse("XX.OO...."), "O") == 5


def test_computer_blocks_immediate_loss():
    # X threatens 0-1-2 and O cannot win at once, so O must take cell 2.
    assert best_move(parse("XX..O...O"), "O") == 2


def test_two_computers_always_draw():
    board = new_board()
    turn = "X"
    while not is_over(board):
        board = play(board, best_move(board, turn), turn)
        turn = other_mark(turn)
    assert is_draw(board)
