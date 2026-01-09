"""
Python implementation of the classic Battleship game.
"""
import sys
import os

sys.path.insert(
    0, os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
)

from src.python.battleship.src.gui.gui import Gui
from src.python.battleship.src.logic.game import BattleshipGame


def main() -> None:
    game = BattleshipGame()
    game.setup()
    gui = Gui(game)
    gui.run()


if __name__ == "__main__":
    main()
