"""
Python implementation of the classic Snake game.

1. Control the snake using arrow keys.
2. Eat food to grow longer.
3. Don't hit the walls or yourself.
4. The game ends when you collide with a wall or your own body.
"""
import sys
import os

sys.path.insert(
    0, os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
)

from src.python.snake.src.gui.gui import Gui
from src.python.snake.src.logic.game import SnakeGame


def main() -> None:
    game = SnakeGame()
    gui = Gui(game)
    gui.run()


if __name__ == "__main__":
    main()
