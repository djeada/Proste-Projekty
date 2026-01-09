"""
Python implementation of a duck shooting game.
"""
import sys
import os

sys.path.insert(
    0, os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
)

from src.python.shooting_ducks.src.gui.gui import Gui
from src.python.shooting_ducks.src.logic.game import DuckGame


def main() -> None:
    game = DuckGame()
    gui = Gui(game)
    gui.run()


if __name__ == "__main__":
    main()
