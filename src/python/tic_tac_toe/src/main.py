"""
Python implementation of the classic Tic-Tac-Toe game.

1. A 3x3 grid is shown.
2. Two players take turns marking spaces with X or O.
3. The first player to get 3 marks in a row wins.
4. If all 9 squares are filled without a winner, the game is a draw.
"""
import sys
import os

sys.path.insert(
    0, os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
)

from src.python.tic_tac_toe.src.gui.gui import Gui
from src.python.tic_tac_toe.src.logic.board import Board


def main() -> None:
    board = Board()
    gui = Gui(board)
    gui.run()


if __name__ == "__main__":
    main()
