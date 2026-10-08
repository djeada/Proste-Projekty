"""Tests of the fifteen puzzle rules."""
import random

from fifteen_puzzle import (
    gap_index,
    is_solvable,
    is_solved,
    move_gap,
    shuffle,
    slide,
    solved_board,
)


def test_solved_board_is_solved_and_solvable():
    board = solved_board()
    assert board[:3] == [1, 2, 3]
    assert board[-2:] == [15, 0]
    assert gap_index(board) == 15
    assert is_solved(board)
    assert is_solvable(board)


def test_slide_only_next_to_the_gap():
    board = solved_board()
    assert not slide(board, 10)  # diagonal to the gap
    assert not slide(board, 3)  # far away
    assert not slide(board, 16)  # outside the board
    assert slide(board, 11)  # tile 12 is above the gap
    assert board[15] == 12 and board[11] == 0


def test_move_gap_stops_at_the_edges():
    board = solved_board()
    assert not move_gap(board, "down")
    assert not move_gap(board, "right")
    assert move_gap(board, "up")
    assert gap_index(board) == 11
    assert move_gap(board, "left")
    assert gap_index(board) == 10


def test_undo_returns_to_solved():
    board = solved_board()
    assert move_gap(board, "left")
    assert not is_solved(board)
    assert move_gap(board, "right")
    assert is_solved(board)


def test_one_move_keeps_board_solvable():
    board = solved_board()
    move_gap(board, "up")
    move_gap(board, "left")
    assert is_solvable(board)


def test_swapped_14_and_15_is_unsolvable():
    board = list(range(1, 14)) + [15, 14, 0]
    assert not is_solvable(board)
    assert not is_solved(board)


def test_shuffle_is_always_solvable_and_not_solved():
    board = []
    for seed in range(50):
        shuffle(board, random.Random(seed))
        assert is_solvable(board)
        assert not is_solved(board)


def test_same_seed_gives_same_shuffle():
    first, second = [], []
    shuffle(first, random.Random(42))
    shuffle(second, random.Random(42))
    assert first == second


def test_shuffle_keeps_every_tile_once():
    board = []
    shuffle(board, random.Random(7))
    assert sorted(board) == list(range(16))
