import unittest
from ..src.entities.player import Player
from ..src.utils.utils import Point

class DummyScreen:
    def __init__(self):
        pass

class TestPlayer(unittest.TestCase):
    def setUp(self):
        self.screen = DummyScreen()
        self.player = Player(self.screen)

    def test_initial_health(self):
        self.assertEqual(self.player.health, 3)

    def test_initial_position(self):
        self.assertEqual(self.player.position.x, 400)
        self.assertEqual(self.player.position.y, 300)

    def test_bullet_list_empty(self):
        self.assertEqual(len(self.player.bullets), 0)

if __name__ == '__main__':
    unittest.main()
