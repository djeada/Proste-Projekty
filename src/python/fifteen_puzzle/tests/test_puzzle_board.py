import unittest
import sys
import os
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '../src/logic')))
from puzzle_board import PuzzleBoard
from utils import Position

class TestPuzzleBoard(unittest.TestCase):
    def test_board_size(self):
        board = PuzzleBoard()
        self.assertEqual(len(board.board), 4)
        self.assertTrue(all(len(row) == 4 for row in board.board))

    def test_blank_tile(self):
        board = PuzzleBoard()
        found = any(' ' in row for row in board.board)
        self.assertTrue(found)

    def test_swap_positions(self):
        board = PuzzleBoard()
        a = board.board[0][0]
        b = board.board[1][1]
        board.swap_positions(Position(0,0), Position(1,1))
        self.assertEqual(board.board[0][0], b)
        self.assertEqual(board.board[1][1], a)

if __name__ == '__main__':
    unittest.main()
