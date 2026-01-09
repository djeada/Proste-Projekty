"""
Board logic for Tic-Tac-Toe game.
"""
import random
from typing import Optional, List, Tuple


BOARD_SIZE = 3
PLAYER_X = "X"
PLAYER_O = "O"
EMPTY = " "


class Board:
    """
    Represents the Tic-Tac-Toe game board.
    """

    def __init__(self) -> None:
        self.cells: List[List[str]] = [
            [EMPTY for _ in range(BOARD_SIZE)] for _ in range(BOARD_SIZE)
        ]
        self.current_player: str = PLAYER_X
        self.game_over: bool = False
        self.winner: Optional[str] = None

    def reset(self) -> None:
        """Reset the board to initial state."""
        self.cells = [
            [EMPTY for _ in range(BOARD_SIZE)] for _ in range(BOARD_SIZE)
        ]
        self.current_player = PLAYER_X
        self.game_over = False
        self.winner = None

    def make_move(self, row: int, col: int) -> bool:
        """
        Make a move at the specified position.

        :param row: Row index (0-2)
        :param col: Column index (0-2)
        :return: True if move was valid, False otherwise
        """
        if row < 0 or row >= BOARD_SIZE or col < 0 or col >= BOARD_SIZE:
            return False
        if self.cells[row][col] != EMPTY:
            return False
        if self.game_over:
            return False

        self.cells[row][col] = self.current_player
        return True

    def check_winner(self) -> Optional[str]:
        """
        Check if there is a winner.

        :return: The winning player ('X' or 'O') or None if no winner
        """
        # Check rows
        for i in range(BOARD_SIZE):
            if (
                self.cells[i][0] != EMPTY
                and self.cells[i][0] == self.cells[i][1]
                and self.cells[i][1] == self.cells[i][2]
            ):
                return self.cells[i][0]

        # Check columns
        for j in range(BOARD_SIZE):
            if (
                self.cells[0][j] != EMPTY
                and self.cells[0][j] == self.cells[1][j]
                and self.cells[1][j] == self.cells[2][j]
            ):
                return self.cells[0][j]

        # Check diagonals
        if (
            self.cells[0][0] != EMPTY
            and self.cells[0][0] == self.cells[1][1]
            and self.cells[1][1] == self.cells[2][2]
        ):
            return self.cells[0][0]

        if (
            self.cells[0][2] != EMPTY
            and self.cells[0][2] == self.cells[1][1]
            and self.cells[1][1] == self.cells[2][0]
        ):
            return self.cells[0][2]

        return None

    def is_board_full(self) -> bool:
        """
        Check if the board is full.

        :return: True if all cells are filled, False otherwise
        """
        for i in range(BOARD_SIZE):
            for j in range(BOARD_SIZE):
                if self.cells[i][j] == EMPTY:
                    return False
        return True

    def switch_player(self) -> None:
        """Switch to the other player."""
        self.current_player = PLAYER_O if self.current_player == PLAYER_X else PLAYER_X

    def get_empty_cells(self) -> List[Tuple[int, int]]:
        """
        Get all empty cells on the board.

        :return: List of (row, col) tuples for empty cells
        """
        empty_cells = []
        for i in range(BOARD_SIZE):
            for j in range(BOARD_SIZE):
                if self.cells[i][j] == EMPTY:
                    empty_cells.append((i, j))
        return empty_cells

    def ai_make_move(self) -> Optional[Tuple[int, int]]:
        """
        Simple AI: pick a random empty cell.

        :return: (row, col) of the chosen cell, or None if no move possible
        """
        empty_cells = self.get_empty_cells()
        if not empty_cells:
            return None

        choice = random.choice(empty_cells)
        return choice

    def get_winning_line(self) -> Optional[List[Tuple[int, int]]]:
        """
        Get the winning line coordinates if there's a winner.

        :return: List of (row, col) tuples forming the winning line, or None
        """
        # Check rows
        for i in range(BOARD_SIZE):
            if (
                self.cells[i][0] != EMPTY
                and self.cells[i][0] == self.cells[i][1]
                and self.cells[i][1] == self.cells[i][2]
            ):
                return [(i, 0), (i, 1), (i, 2)]

        # Check columns
        for j in range(BOARD_SIZE):
            if (
                self.cells[0][j] != EMPTY
                and self.cells[0][j] == self.cells[1][j]
                and self.cells[1][j] == self.cells[2][j]
            ):
                return [(0, j), (1, j), (2, j)]

        # Check diagonals
        if (
            self.cells[0][0] != EMPTY
            and self.cells[0][0] == self.cells[1][1]
            and self.cells[1][1] == self.cells[2][2]
        ):
            return [(0, 0), (1, 1), (2, 2)]

        if (
            self.cells[0][2] != EMPTY
            and self.cells[0][2] == self.cells[1][1]
            and self.cells[1][1] == self.cells[2][0]
        ):
            return [(0, 2), (1, 1), (2, 0)]

        return None
