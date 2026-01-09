import unittest
import sys
import os

sys.path.insert(
    0, os.path.abspath(os.path.join(os.path.dirname(__file__), "../src/logic"))
)
from game import DuckGame, Duck, Direction


class TestDuck(unittest.TestCase):
    def test_duck_creation(self):
        duck = Duck(x=100, y=100, alive=True, direction=Direction.RIGHT)
        self.assertEqual(duck.x, 100)
        self.assertEqual(duck.y, 100)
        self.assertTrue(duck.alive)

    def test_duck_move_right(self):
        duck = Duck(x=100, y=100, alive=True, direction=Direction.RIGHT, speed=5)
        duck.move(600, 400)
        self.assertEqual(duck.x, 105)

    def test_duck_move_left(self):
        duck = Duck(x=100, y=100, alive=True, direction=Direction.LEFT, speed=5)
        duck.move(600, 400)
        self.assertEqual(duck.x, 95)

    def test_duck_bounce_off_wall(self):
        duck = Duck(x=0, y=100, alive=True, direction=Direction.LEFT, speed=5)
        duck.move(600, 400)
        self.assertEqual(duck.direction, Direction.RIGHT)

    def test_duck_contains_point(self):
        duck = Duck(x=100, y=100, alive=True, direction=Direction.RIGHT)
        self.assertTrue(duck.contains_point(110, 110))
        self.assertFalse(duck.contains_point(50, 50))

    def test_dead_duck_doesnt_move(self):
        duck = Duck(x=100, y=100, alive=False, direction=Direction.RIGHT, speed=5)
        duck.move(600, 400)
        self.assertEqual(duck.x, 100)


class TestDuckGame(unittest.TestCase):
    def test_game_initialization(self):
        game = DuckGame()
        self.assertEqual(game.lives, 3)
        self.assertEqual(game.score, 0)
        self.assertEqual(game.level, 1)
        self.assertFalse(game.game_over)

    def test_spawn_ducks(self):
        game = DuckGame()
        self.assertGreater(len(game.ducks), 0)

    def test_shoot_hit(self):
        game = DuckGame()
        # Force a duck at known position
        game.ducks = [Duck(x=100, y=100, alive=True, direction=Direction.RIGHT)]
        hit = game.shoot(110, 110)
        self.assertTrue(hit)
        self.assertFalse(game.ducks[0].alive)

    def test_shoot_miss(self):
        game = DuckGame()
        game.ducks = [Duck(x=100, y=100, alive=True, direction=Direction.RIGHT)]
        initial_lives = game.lives
        hit = game.shoot(500, 500)
        self.assertFalse(hit)
        self.assertEqual(game.lives, initial_lives - 1)

    def test_game_over_on_no_lives(self):
        game = DuckGame()
        game.lives = 1
        game.ducks = [Duck(x=100, y=100, alive=True, direction=Direction.RIGHT)]
        game.shoot(500, 500)  # Miss
        self.assertTrue(game.game_over)

    def test_toggle_pause(self):
        game = DuckGame()
        self.assertFalse(game.paused)
        game.toggle_pause()
        self.assertTrue(game.paused)
        game.toggle_pause()
        self.assertFalse(game.paused)

    def test_reset(self):
        game = DuckGame()
        game.score = 100
        game.lives = 0
        game.game_over = True
        game.reset()
        self.assertEqual(game.score, 0)
        self.assertEqual(game.lives, 3)
        self.assertFalse(game.game_over)


if __name__ == "__main__":
    unittest.main()
