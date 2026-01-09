import unittest
import sys
import os

sys.path.insert(
    0, os.path.abspath(os.path.join(os.path.dirname(__file__), "../src/logic"))
)
from board import Board, PLAYER_X, PLAYER_O, EMPTY, BOARD_SIZE


class TestBoard(unittest.TestCase):
    def test_board_initialization(self):
        board = Board()
        self.assertEqual(len(board.cells), BOARD_SIZE)
        self.assertTrue(all(len(row) == BOARD_SIZE for row in board.cells))
        self.assertEqual(board.current_player, PLAYER_X)
        self.assertFalse(board.game_over)

    def test_make_valid_move(self):
        board = Board()
        result = board.make_move(0, 0)
        self.assertTrue(result)
        self.assertEqual(board.cells[0][0], PLAYER_X)

    def test_make_invalid_move_occupied(self):
        board = Board()
        board.make_move(0, 0)
        result = board.make_move(0, 0)
        self.assertFalse(result)

    def test_make_invalid_move_out_of_bounds(self):
        board = Board()
        result = board.make_move(-1, 0)
        self.assertFalse(result)
        result = board.make_move(3, 0)
        self.assertFalse(result)

    def test_check_winner_row(self):
        board = Board()
        board.cells[0] = [PLAYER_X, PLAYER_X, PLAYER_X]
        winner = board.check_winner()
        self.assertEqual(winner, PLAYER_X)

    def test_check_winner_column(self):
        board = Board()
        for i in range(BOARD_SIZE):
            board.cells[i][0] = PLAYER_O
        winner = board.check_winner()
        self.assertEqual(winner, PLAYER_O)

    def test_check_winner_diagonal(self):
        board = Board()
        board.cells[0][0] = PLAYER_X
        board.cells[1][1] = PLAYER_X
        board.cells[2][2] = PLAYER_X
        winner = board.check_winner()
        self.assertEqual(winner, PLAYER_X)

    def test_check_no_winner(self):
        board = Board()
        winner = board.check_winner()
        self.assertIsNone(winner)

    def test_is_board_full(self):
        board = Board()
        for i in range(BOARD_SIZE):
            for j in range(BOARD_SIZE):
                board.cells[i][j] = PLAYER_X if (i + j) % 2 == 0 else PLAYER_O
        self.assertTrue(board.is_board_full())

    def test_is_board_not_full(self):
        board = Board()
        self.assertFalse(board.is_board_full())

    def test_switch_player(self):
        board = Board()
        self.assertEqual(board.current_player, PLAYER_X)
        board.switch_player()
        self.assertEqual(board.current_player, PLAYER_O)
        board.switch_player()
        self.assertEqual(board.current_player, PLAYER_X)

    def test_reset(self):
        board = Board()
        board.make_move(0, 0)
        board.switch_player()
        board.game_over = True
        board.reset()
        self.assertTrue(all(cell == EMPTY for row in board.cells for cell in row))
        self.assertEqual(board.current_player, PLAYER_X)
        self.assertFalse(board.game_over)


if __name__ == "__main__":
    unittest.main()
