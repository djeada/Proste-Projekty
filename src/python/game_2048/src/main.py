"""
Python implementation of the 2048 puzzle game.

1. Use arrow keys to slide tiles in that direction.
2. When two tiles with the same number touch, they merge into one.
3. The goal is to create a tile with the number 2048.
4. The game ends when no more moves are possible.
"""
from src.python.game_2048.src.gui.gui import Gui
from src.python.game_2048.src.logic.game import Game2048


def main() -> None:
    game = Game2048()
    gui = Gui(game)
    gui.run()


if __name__ == "__main__":
    main()
