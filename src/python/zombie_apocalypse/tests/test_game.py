import unittest
import sys
import os
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '../src')))
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '../src/game')))
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '../src/entities')))
from src.game.game import Game
import pygame

class DummyScreen:
    def __init__(self):
        pass

class DummyPlayer:
    def __init__(self):
        self.health = 3
    def draw(self):
        pass
    def update(self):
        pass

class DummyZombie:
    def __init__(self):
        self.updated = False
    def draw(self):
        pass
    def update(self):
        self.updated = True

class TestGame(unittest.TestCase):
    def setUp(self):
        pygame.init()
        self.game = Game()
        self.game.player = DummyPlayer()
        self.game.zombies = []

    def test_initial_score(self):
        self.assertEqual(self.game.score, 0)

    def test_add_zombie(self):
        self.game.zombies.append(DummyZombie())
        self.assertEqual(len(self.game.zombies), 1)

if __name__ == '__main__':
    unittest.main()
