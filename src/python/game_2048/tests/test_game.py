import unittest
import sys
import os

sys.path.insert(
    0, os.path.abspath(os.path.join(os.path.dirname(__file__), "../src/logic"))
)
from game import Game2048, Move, GRID_SIZE


class TestGame2048(unittest.TestCase):
    def test_game_initialization(self):
        game = Game2048()
        # After reset, should have exactly 2 tiles spawned
        non_zero_count = sum(
            1 for row in game.cells for cell in row if cell != 0
        )
        self.assertEqual(non_zero_count, 2)
        self.assertEqual(game.score, 0)
        self.assertFalse(game.game_over)

    def test_slide_left(self):
        game = Game2048()
        game.cells = [
            [0, 2, 0, 2],
            [0, 0, 0, 0],
            [0, 0, 0, 0],
            [0, 0, 0, 0],
        ]
        game.move(Move.LEFT)
        # After sliding left, 2+2 should merge to 4 at position [0][0]
        self.assertEqual(game.cells[0][0], 4)
        self.assertEqual(game.cells[0][1], 0)

    def test_slide_right(self):
        game = Game2048()
        game.cells = [
            [2, 2, 0, 0],
            [0, 0, 0, 0],
            [0, 0, 0, 0],
            [0, 0, 0, 0],
        ]
        game.move(Move.RIGHT)
        # After merging, 4 should be at the rightmost position
        self.assertEqual(game.cells[0][3], 4)

    def test_slide_up(self):
        game = Game2048()
        game.cells = [
            [2, 0, 0, 0],
            [2, 0, 0, 0],
            [0, 0, 0, 0],
            [0, 0, 0, 0],
        ]
        game.move(Move.UP)
        self.assertEqual(game.cells[0][0], 4)
        self.assertEqual(game.cells[1][0], 0)

    def test_slide_down(self):
        game = Game2048()
        game.cells = [
            [2, 0, 0, 0],
            [2, 0, 0, 0],
            [0, 0, 0, 0],
            [0, 0, 0, 0],
        ]
        game.move(Move.DOWN)
        self.assertEqual(game.cells[3][0], 4)
        self.assertEqual(game.cells[0][0], 0)

    def test_score_increases_on_merge(self):
        game = Game2048()
        game.cells = [
            [2, 2, 0, 0],
            [0, 0, 0, 0],
            [0, 0, 0, 0],
            [0, 0, 0, 0],
        ]
        game.score = 0
        game.move(Move.LEFT)
        self.assertEqual(game.score, 4)

    def test_can_move_with_empty_cells(self):
        game = Game2048()
        game.cells = [
            [2, 0, 0, 0],
            [0, 0, 0, 0],
            [0, 0, 0, 0],
            [0, 0, 0, 0],
        ]
        self.assertTrue(game.can_move())

    def test_can_move_with_mergeable_cells(self):
        game = Game2048()
        game.cells = [
            [2, 4, 8, 16],
            [4, 8, 16, 32],
            [8, 16, 32, 64],
            [16, 32, 64, 64],  # 64 and 64 can merge
        ]
        self.assertTrue(game.can_move())

    def test_cannot_move_when_stuck(self):
        game = Game2048()
        game.cells = [
            [2, 4, 8, 16],
            [4, 8, 16, 32],
            [8, 16, 32, 64],
            [16, 32, 64, 128],
        ]
        self.assertFalse(game.can_move())

    def test_reset(self):
        game = Game2048()
        game.score = 1000
        game.game_over = True
        game.reset()
        self.assertEqual(game.score, 0)
        self.assertFalse(game.game_over)


if __name__ == "__main__":
    unittest.main()
