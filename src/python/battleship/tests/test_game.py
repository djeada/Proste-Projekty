import unittest
import sys
import os

sys.path.insert(
    0, os.path.abspath(os.path.join(os.path.dirname(__file__), "../src/logic"))
)
from game import Board, Ship, BattleshipGame, CellState, BOARD_SIZE


class TestShip(unittest.TestCase):
    def test_ship_creation(self):
        ship = Ship(length=3)
        self.assertEqual(ship.length, 3)
        self.assertEqual(ship.hits, 0)
        self.assertFalse(ship.is_sunk())

    def test_ship_sinking(self):
        ship = Ship(length=2)
        ship.hits = 1
        self.assertFalse(ship.is_sunk())
        ship.hits = 2
        self.assertTrue(ship.is_sunk())


class TestBoard(unittest.TestCase):
    def test_board_initialization(self):
        board = Board()
        self.assertEqual(len(board.grid), BOARD_SIZE)
        self.assertEqual(len(board.grid[0]), BOARD_SIZE)
        self.assertEqual(len(board.ships), 0)

    def test_place_ship_horizontal(self):
        board = Board()
        result = board.place_ship(0, 0, 3, horizontal=True)
        self.assertTrue(result)
        self.assertEqual(len(board.ships), 1)
        self.assertEqual(board.grid[0][0], CellState.SHIP)
        self.assertEqual(board.grid[0][1], CellState.SHIP)
        self.assertEqual(board.grid[0][2], CellState.SHIP)

    def test_place_ship_vertical(self):
        board = Board()
        result = board.place_ship(0, 0, 3, horizontal=False)
        self.assertTrue(result)
        self.assertEqual(board.grid[0][0], CellState.SHIP)
        self.assertEqual(board.grid[1][0], CellState.SHIP)
        self.assertEqual(board.grid[2][0], CellState.SHIP)

    def test_place_ship_overlap(self):
        board = Board()
        board.place_ship(0, 0, 3, horizontal=True)
        result = board.place_ship(1, 0, 3, horizontal=True)
        self.assertFalse(result)

    def test_fire_hit(self):
        board = Board()
        board.place_ship(0, 0, 3, horizontal=True)
        hit, sunk = board.fire(0, 0)
        self.assertTrue(hit)
        self.assertIsNone(sunk)
        self.assertEqual(board.grid[0][0], CellState.HIT)

    def test_fire_miss(self):
        board = Board()
        hit, sunk = board.fire(5, 5)
        self.assertFalse(hit)
        self.assertIsNone(sunk)
        self.assertEqual(board.grid[5][5], CellState.MISS)

    def test_fire_sinks_ship(self):
        board = Board()
        board.place_ship(0, 0, 2, horizontal=True)
        board.fire(0, 0)
        hit, sunk = board.fire(1, 0)
        self.assertTrue(hit)
        self.assertIsNotNone(sunk)
        self.assertTrue(sunk.is_sunk())


class TestBattleshipGame(unittest.TestCase):
    def test_game_setup(self):
        game = BattleshipGame()
        game.setup()
        self.assertEqual(len(game.player_board.ships), 5)
        self.assertEqual(len(game.enemy_board.ships), 5)

    def test_player_turn(self):
        game = BattleshipGame()
        game.setup()
        self.assertTrue(game.player_turn)


if __name__ == "__main__":
    unittest.main()
