import random

from battleship import (BOARD_SIZE, FLEET, HIT, INVALID, MISS, REPEAT, SUNK, Board, Computer, ship_cells)


def test_ship_cells_horizontal_and_vertical():
    assert ship_cells(3, 1, 2, True) == [(1, 2), (2, 2), (3, 2)]
    assert ship_cells(3, 1, 2, False) == [(1, 2), (1, 3), (1, 4)]


def test_place_inside_board():
    board = Board()
    assert board.place(0, 0, 0, True)
    assert not board.place(1, 8, 0, True)      # 4 cells would go past the right edge
    assert not board.place(1, 0, 9, False)     # 4 cells would go past the bottom edge
    assert board.place(1, 9, 0, False)         # exactly fits in the last column


def test_ships_may_touch_but_not_overlap():
    board = Board()
    assert board.place(0, 0, 0, True)          # cells (0..4, 0)
    assert not board.place(1, 4, 0, False)     # overlaps at (4, 0)
    assert board.place(1, 5, 0, True)          # touches the first ship, allowed
    assert not board.place(1, 5, 0, True)      # the same ship cannot be placed twice


def test_fire_miss_hit_and_sunk():
    board = Board()
    board.place(4, 0, 0, True)                 # ship of length 2 at (0,0)-(1,0)
    assert board.fire(5, 5) == MISS
    assert board.fire(0, 0) == HIT
    assert board.fire(1, 0) == SUNK
    assert not board.all_sunk()                # the other four ships are not placed yet


def test_repeat_and_invalid_shots_change_nothing():
    board = Board()
    board.place(4, 0, 0, True)
    assert board.fire(0, 0) == HIT
    assert board.fire(0, 0) == REPEAT
    assert board.ships[4].hits == 1
    assert board.fire(10, 0) == INVALID
    assert board.fire(-1, 3) == INVALID


def test_all_sunk_when_whole_fleet_is_destroyed():
    board = Board()
    board.place_randomly(random.Random(1))
    assert board.fleet_placed()
    for y in range(BOARD_SIZE):
        for x in range(BOARD_SIZE):
            board.fire(x, y)
    assert board.all_sunk()


def test_random_fleet_is_valid_and_repeatable():
    for seed in range(20):
        board = Board()
        board.place_randomly(random.Random(seed))
        cells = [cell for row in board.ship_at for cell in row if cell is not None]
        assert len(cells) == sum(FLEET)
        assert board.fleet_placed()
    first = Board()
    first.place_randomly(random.Random(7))
    again = Board()
    again.place_randomly(random.Random(7))
    assert first.ship_at == again.ship_at


def test_computer_follows_a_hit_to_its_neighbours():
    board = Board()
    board.place(0, 0, 0, True)
    computer = Computer()
    rng = random.Random(3)
    board.fire(3, 3)
    computer.report(board, 3, 3, MISS)
    board.fire(5, 5)
    computer.report(board, 5, 5, HIT)
    assert board.fire(6, 6) == MISS            # not a neighbour
    computer.report(board, 6, 6, MISS)
    x, y = computer.choose(board, rng)
    assert (x, y) in {(4, 5), (6, 5), (5, 4), (5, 6)}


def test_computer_forgets_targets_when_ship_sinks():
    board = Board()
    computer = Computer()
    computer.report(board, 5, 5, HIT)
    assert len(computer.targets) == 4
    computer.report(board, 5, 5, SUNK)
    assert computer.targets == []


def test_computer_never_repeats_a_shot_and_finishes_the_game():
    rng = random.Random(42)
    board = Board()
    board.place_randomly(rng)
    computer = Computer()
    shots = 0
    while not board.all_sunk():
        x, y = computer.choose(board, rng)
        result = board.fire(x, y)
        assert result in (MISS, HIT, SUNK)
        computer.report(board, x, y, result)
        shots += 1
    assert shots <= BOARD_SIZE * BOARD_SIZE
