"""Tests of the rules in src/minesweeper.py."""

import random

from minesweeper import Game


def count_all_mines(game: Game) -> int:
    return sum(sum(row) for row in game.mine)


def test_start_has_no_mines_yet() -> None:
    game = Game(9, 9, 10, random.Random(42))
    assert game.state == "playing"
    assert not game.mines_placed
    assert count_all_mines(game) == 0
    assert game.mines_left == 10


def test_first_reveal_is_never_a_mine_or_next_to_one() -> None:
    for seed in range(1, 51):
        game = Game(9, 9, 10, random.Random(seed))
        game.reveal(0, 0)
        assert game.state == "playing"
        assert count_all_mines(game) == 10
        assert not any(game.mine[r][c] for r in range(2) for c in range(2))


def test_first_reveal_in_middle_of_expert_board() -> None:
    game = Game(16, 30, 99, random.Random(7))
    game.reveal(8, 15)
    assert count_all_mines(game) == 99
    assert not any(game.mine[r][c] for r in range(7, 10) for c in range(14, 17))


def test_same_seed_gives_same_layout() -> None:
    first = Game(16, 16, 40, random.Random(123))
    second = Game(16, 16, 40, random.Random(123))
    first.reveal(3, 3)
    second.reveal(3, 3)
    assert first.mine == second.mine


def test_neighbor_counts_match_mines() -> None:
    game = Game(16, 16, 40, random.Random(99))
    game.reveal(0, 0)
    for r in range(game.rows):
        for c in range(game.cols):
            expected = sum(
                game.mine[nr][nc]
                for nr in range(max(r - 1, 0), min(r + 2, game.rows))
                for nc in range(max(c - 1, 0), min(c + 2, game.cols))
                if (nr, nc) != (r, c)
            )
            assert game.neighbors[r][c] == expected


def test_flood_fill_opens_empty_area() -> None:
    # 5x5 board with one mine in the top-right corner: all 24 safe cells are reachable.
    game = Game(5, 5, 1, random.Random(1))
    game.mines_placed = True
    game.mine[0][4] = True
    game.neighbors[0][3] = game.neighbors[1][3] = game.neighbors[1][4] = 1

    game.reveal(4, 0)
    assert game.revealed_count == 24
    assert game.state == "won"
    assert not game.revealed[0][4]


def test_flood_fill_stops_at_numbers() -> None:
    # Mine in the middle of a 3x3 board: every other cell shows a 1, so nothing spreads.
    game = Game(3, 3, 1, random.Random(1))
    game.mines_placed = True
    game.mine[1][1] = True
    for r in range(3):
        for c in range(3):
            if not game.mine[r][c]:
                game.neighbors[r][c] = 1

    game.reveal(0, 0)
    assert game.revealed_count == 1
    assert game.state == "playing"


def test_revealing_a_mine_loses_and_shows_all_mines() -> None:
    game = Game(9, 9, 10, random.Random(5))
    game.reveal(4, 4)
    row, col = next((r, c) for r in range(9) for c in range(9) if game.mine[r][c])
    game.reveal(row, col)
    assert game.state == "lost"
    assert all(game.revealed[r][c] for r in range(9) for c in range(9) if game.mine[r][c])


def test_revealing_all_safe_cells_wins() -> None:
    game = Game(9, 9, 10, random.Random(77))
    game.reveal(4, 4)
    for r in range(9):
        for c in range(9):
            if not game.mine[r][c]:
                game.reveal(r, c)
    assert game.state == "won"
    assert game.revealed_count == 71


def test_flags_count_down_and_toggle() -> None:
    game = Game(9, 9, 10, random.Random(3))
    game.toggle_flag(0, 0)
    game.toggle_flag(8, 8)
    assert game.flagged[0][0]
    assert game.mines_left == 8
    game.toggle_flag(0, 0)
    assert not game.flagged[0][0]
    assert game.mines_left == 9


def test_flagged_cell_cannot_be_revealed() -> None:
    game = Game(9, 9, 10, random.Random(3))
    game.toggle_flag(2, 2)
    game.reveal(2, 2)
    assert not game.mines_placed
    assert game.revealed_count == 0


def test_flag_on_revealed_cell_is_ignored() -> None:
    game = Game(9, 9, 10, random.Random(3))
    game.reveal(4, 4)
    game.toggle_flag(4, 4)
    assert not game.flagged[4][4]
    assert game.flag_count == 0


def test_out_of_board_moves_are_ignored() -> None:
    game = Game(9, 9, 10, random.Random(3))
    game.reveal(-1, 0)
    game.reveal(9, 9)
    game.toggle_flag(0, 30)
    assert game.state == "playing"
    assert not game.mines_placed
    assert game.flag_count == 0
