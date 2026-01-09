import unittest
import sys
import os

sys.path.insert(
    0, os.path.abspath(os.path.join(os.path.dirname(__file__), "../src/logic"))
)
from game import SnakeGame, Direction


class TestSnakeGame(unittest.TestCase):
    def test_game_initialization(self):
        game = SnakeGame(20, 20)
        self.assertEqual(game.width, 20)
        self.assertEqual(game.height, 20)
        self.assertEqual(len(game.snake), 1)
        self.assertFalse(game.game_over)
        self.assertEqual(game.score, 0)

    def test_snake_starts_in_center(self):
        game = SnakeGame(20, 20)
        head = game.get_snake_head()
        self.assertEqual(head, (10, 10))

    def test_move_right(self):
        game = SnakeGame(20, 20)
        game.direction = Direction.RIGHT
        initial_head = game.get_snake_head()
        game.move()
        new_head = game.get_snake_head()
        self.assertEqual(new_head[0], initial_head[0] + 1)
        self.assertEqual(new_head[1], initial_head[1])

    def test_move_left(self):
        game = SnakeGame(20, 20)
        game.direction = Direction.LEFT
        initial_head = game.get_snake_head()
        game.move()
        new_head = game.get_snake_head()
        self.assertEqual(new_head[0], initial_head[0] - 1)

    def test_move_up(self):
        game = SnakeGame(20, 20)
        game.direction = Direction.UP
        initial_head = game.get_snake_head()
        game.move()
        new_head = game.get_snake_head()
        self.assertEqual(new_head[1], initial_head[1] - 1)

    def test_move_down(self):
        game = SnakeGame(20, 20)
        game.direction = Direction.DOWN
        initial_head = game.get_snake_head()
        game.move()
        new_head = game.get_snake_head()
        self.assertEqual(new_head[1], initial_head[1] + 1)

    def test_wall_collision(self):
        game = SnakeGame(20, 20)
        game.snake = [(0, 10)]
        game.direction = Direction.LEFT
        game.move()
        self.assertTrue(game.game_over)

    def test_self_collision(self):
        game = SnakeGame(20, 20)
        game.snake = [(5, 5), (4, 5), (3, 5), (3, 6), (4, 6), (5, 6)]
        game.direction = Direction.DOWN
        game.move()
        self.assertTrue(game.game_over)

    def test_cannot_reverse_direction(self):
        game = SnakeGame(20, 20)
        game.direction = Direction.RIGHT
        game.update_direction(Direction.LEFT)
        self.assertEqual(game.direction, Direction.RIGHT)

    def test_can_change_to_perpendicular_direction(self):
        game = SnakeGame(20, 20)
        game.direction = Direction.RIGHT
        game.update_direction(Direction.UP)
        self.assertEqual(game.direction, Direction.UP)

    def test_eating_food_increases_score(self):
        game = SnakeGame(20, 20)
        game.food = (11, 10)  # Place food right next to snake head
        game.direction = Direction.RIGHT
        game.move()
        self.assertEqual(game.score, 10)

    def test_reset(self):
        game = SnakeGame(20, 20)
        game.score = 100
        game.game_over = True
        game.reset()
        self.assertEqual(game.score, 0)
        self.assertFalse(game.game_over)
        self.assertEqual(len(game.snake), 1)


if __name__ == "__main__":
    unittest.main()
