import random

from src.python.fifteen_puzzle.src.logic.utils import Position


class PuzzleBoard:
    """
    This class represents a FIFTEEN PUZZLE game.
    """

    def __init__(self):
        """
        Initializes the board with the tiles in the correct order.
        """
        self.size = 4
        self.board = [
            [i + self.size * j for i in range(1, self.size + 1)]
            for j in range(self.size)
        ]
        self.board[self.size - 1][self.size - 1] = " "
        # shuffle all the tiles on the board
        # tiles can also switch rows
        for _ in range(self.size * self.size):
            pos_a = Position(
                random.randint(0, self.size - 1), random.randint(0, self.size - 1)
            )
            pos_b = Position(
                random.randint(0, self.size - 1), random.randint(0, self.size - 1)
            )
            self.swap_positions(pos_a, pos_b)

    def swap_positions(self, pos_a: Position, pos_b: Position) -> None:
        """
        Swaps the tiles at the given positions.
        """
        self.board[pos_a.x][pos_a.y], self.board[pos_b.x][pos_b.y] = (
            self.board[pos_b.x][pos_b.y],
            self.board[pos_a.x][pos_a.y],
        )

    def blank_position(self) -> Position:
        """
        Returns the position of the blank tile.

        return: Position of the blank tile.
        raise: Exception if the blank tile is not found.
        """

        for row in range(self.size):
            for col in range(self.size):
                if self.board[row][col] == " ":
                    return Position(row, col)

        raise Exception("No blank tile found.")

    def can_be_moved(self, position: Position) -> bool:
        """
        Checks if the tile at the given position can be moved.

        param position: Position of the tile to be moved.
        return: True if a given tile can be moved and False otherwise.

        """
        return self.are_adjacent(self.blank_position(), position)

    def are_adjacent(self, pos_a: Position, pos_b: Position) -> bool:
        """
        Checks if the two given positions are adjacent.

        param pos_a: First position.
        param pos_b: Second position.
        return: True if the two positions are adjacent and False otherwise.
        """
        return abs(pos_a.x - pos_b.x) + abs(pos_a.y - pos_b.y) == 1

    def move(self, position: Position) -> None:
        """
        Moves the tile at the given position to the blank space.

        param position: Position of the tile to be moved.
        """
        if not self.can_be_moved(position):
            raise Exception("Cannot move a given tile.")

        self.swap_positions(self.blank_position(), position)

    def move_up(self) -> None:
        """
        Moves the blank tile up.
        """
        pos = self.blank_position() + Position(1, 0)
        if pos.x >= self.size:
            return
        self.move(pos)

    def move_down(self) -> None:
        """
        Moves the blank tile down.
        """
        pos = self.blank_position() + Position(-1, 0)
        if pos.x < 0:
            return
        self.move(pos)

    def move_left(self) -> None:
        """
        Moves the blank tile left.
        """
        pos = self.blank_position() + Position(0, 1)
        if pos.y >= self.size:
            return
        self.move(pos)

    def move_right(self) -> None:
        """
        Moves the blank tile right.
        """
        pos = self.blank_position() + Position(0, -1)
        if pos.y < 0:
            return
        self.move(pos)

    def is_on_correct_position(self, position: Position) -> bool:
        """
        Checks if the tile at the given position is on the correct position.

        param position: Position of the tile to be checked.
        return: True if the tile is on the correct position and False otherwise.
        """
        return (
            self.board[position.x][position.y]
            == position.y + self.size * position.x + 1
        )

    def is_solved(self) -> bool:
        """
        Checks if the board is solved.

        return: True if the board is solved and False otherwise.
        """
        for row in range(self.size):
            for col in range(self.size):
                if not self.is_on_correct_position(Position(row, col)):
                    return False

        return True

    def __str__(self) -> str:
        """
        Returns a string representation of the board.
        """
        result = ""
        for row in self.board:
            for elem in row:
                elem = str(elem)
                if len(elem) == 1:
                    elem += " "
                result += elem + " "

            result += "\n"

        return result
