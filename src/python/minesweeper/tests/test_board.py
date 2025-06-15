import unittest
import sys
import os
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '../src/logic')))
from board import Board

class TestBoard(unittest.TestCase):
    def test_board_size(self):
        board = Board(5, 5, 5)
        self.assertEqual(len(board.board), 5)
        self.assertTrue(all(len(row) == 5 for row in board.board))

    def test_mine_count(self):
        board = Board(5, 5, 5)
        mine_count = len(board.get_all_mines())
        self.assertEqual(mine_count, 5)

if __name__ == '__main__':
    unittest.main()
