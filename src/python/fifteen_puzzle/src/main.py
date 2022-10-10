"""
Fifteen puzzle is a game where you are given a board and you need to move all the
tiles to the empty space to get the board in order.
At the start of the game, you are given a board with some tiles in random order.
There is only one empty space on the board.
You can move the tiles to the empty space by swapping them.
Once all the tiles are in order, you win the game.
"""
from src.python.fifteen_puzzle.src.gui.gui import PuzzleGui
from src.python.fifteen_puzzle.src.logic.puzzle_board import PuzzleBoard


def main() -> None:
    puzzle_board = PuzzleBoard()
    gui = PuzzleGui(puzzle_board)
    gui.run()


if __name__ == "__main__":
    main()
