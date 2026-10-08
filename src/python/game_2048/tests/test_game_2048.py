"""Tests of the 2048 rules. A scripted random source makes every spawn predictable."""
import pytest

from game_2048 import LOST, PLAYING, WON, Game, slide_board, slide_row


def scripted(values):
    """A random source that returns the given values one by one."""
    values = iter(values)

    def random_below(n):
        value = next(values)
        assert 0 <= value < n
        return value

    return random_below


def no_random(n):
    raise AssertionError("no random number should be drawn")


def make_game(rows, draws=()):
    game = Game(random_below=scripted([0, 0, 0, 0]))  # the start tiles are not under test
    game.board = [list(row) for row in rows]
    game.random_below = scripted(draws)
    return game


EMPTY = [[0] * 4 for _ in range(4)]


@pytest.mark.parametrize(
    "line, expected, points",
    [
        ([2, 2, 2, 2], [4, 4, 0, 0], 8),  # pairs merge from the start
        ([4, 4, 8, 0], [8, 8, 0, 0], 8),  # the new 8 does not merge again
        ([2, 0, 2, 4], [4, 4, 0, 0], 4),  # gaps close before merging
        ([0, 0, 0, 2], [2, 0, 0, 0], 0),  # sliding without merging
        ([2, 4, 8, 16], [2, 4, 8, 16], 0),  # nothing to do
    ],
)
def test_slide_row(line, expected, points):
    assert slide_row(line) == (expected, points)


def test_slide_board_in_all_directions():
    start = [
        [2, 0, 0, 0],
        [2, 0, 0, 0],
        [0, 0, 0, 0],
        [0, 0, 0, 4],
    ]

    board = [row[:] for row in start]
    assert slide_board(board, "up") == 4
    assert board == [[4, 0, 0, 4], [0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0]]

    board = [row[:] for row in start]
    assert slide_board(board, "down") == 4
    assert board == [[0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0], [4, 0, 0, 4]]

    board = [row[:] for row in start]
    assert slide_board(board, "left") == 0
    assert board == [[2, 0, 0, 0], [2, 0, 0, 0], [0, 0, 0, 0], [4, 0, 0, 0]]

    board = [row[:] for row in start]
    assert slide_board(board, "right") == 0
    assert board == [[0, 0, 0, 2], [0, 0, 0, 2], [0, 0, 0, 0], [0, 0, 0, 4]]


def test_move_that_changes_nothing_spawns_nothing():
    game = make_game([[2, 4, 0, 0]] + EMPTY[1:])
    game.random_below = no_random
    assert game.move("left") is False
    assert game.board[0] == [2, 4, 0, 0]
    assert game.score == 0


def test_move_merges_and_spawns_one_tile():
    game = make_game([[2, 2, 0, 0]] + EMPTY[1:], draws=[0, 1])  # first empty cell, roll 1 gives 2
    assert game.move("left") is True
    assert game.score == 4
    assert game.board[0] == [4, 2, 0, 0]


def test_spawn_is_four_on_a_roll_of_zero():
    game = make_game(EMPTY, draws=[0, 0])
    assert game.spawn_tile() is True
    assert game.board[0][0] == 4


def test_spawn_on_full_board_does_nothing():
    game = make_game([[2] * 4 for _ in range(4)])
    game.random_below = no_random
    assert game.spawn_tile() is False


def test_full_board_without_merges_is_lost():
    game = make_game([[2, 4, 2, 4], [4, 2, 4, 2], [2, 4, 2, 4], [4, 2, 4, 2]])
    assert game.status() == LOST
    assert game.can_move() is False


def test_equal_neighbours_keep_the_game_going():
    game = make_game([[2, 2, 2, 4], [4, 2, 4, 2], [2, 4, 2, 4], [4, 2, 4, 2]])
    assert game.status() == PLAYING


def test_reaching_2048_wins_until_player_continues():
    game = make_game([[1024, 1024, 0, 0]] + EMPTY[1:], draws=[0, 5])
    assert game.move("left") is True
    assert game.board[0][0] == 2048
    assert game.status() == WON

    game.random_below = no_random
    assert game.move("right") is False  # moves are ignored after winning

    game.keep_playing = True
    assert game.status() == PLAYING
