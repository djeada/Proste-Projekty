"""
Python implementation of the classic game of minesweeper.


1. A 2D grid of squares is shown.
2. The player is aware of the number of squares containing mines, but not of their location.
3. The goal of the game is to find all of the squares that do not have a mine in them.
4. If the player attempts to expose a mined square, the game is lost.
5. The player wins when the last safe square is revealed.
"""
from projekty.python.minesweeper.src.gui.gui import Gui
from projekty.python.minesweeper.src.logic.board import Board


def main() -> None:
    board = Board()
    gui = Gui(board)
    gui.run()


if __name__ == "__main__":
    main()
