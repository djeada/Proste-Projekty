"""
Game logic for 2048 game.
"""
import random
from typing import List, Tuple
from enum import Enum, auto


class Move(Enum):
    """Direction of move."""

    UP = auto()
    DOWN = auto()
    LEFT = auto()
    RIGHT = auto()


GRID_SIZE = 4


class Game2048:
    """
    2048 game logic.
    """

    def __init__(self) -> None:
        self.cells: List[List[int]] = [
            [0 for _ in range(GRID_SIZE)] for _ in range(GRID_SIZE)
        ]
        self.score: int = 0
        self.game_over: bool = False
        self.won: bool = False
        self.reset()

    def reset(self) -> None:
        """Reset the game to initial state."""
        self.cells = [[0 for _ in range(GRID_SIZE)] for _ in range(GRID_SIZE)]
        self.score = 0
        self.game_over = False
        self.won = False
        self.spawn_random()
        self.spawn_random()

    def spawn_random(self) -> None:
        """Spawn a 2 (90%) or 4 (10%) on a random empty cell."""
        empty_cells: List[Tuple[int, int]] = []
        for row in range(GRID_SIZE):
            for col in range(GRID_SIZE):
                if self.cells[row][col] == 0:
                    empty_cells.append((row, col))

        if not empty_cells:
            return

        row, col = random.choice(empty_cells)
        self.cells[row][col] = 4 if random.random() < 0.1 else 2

    def _slide_and_merge_line(self, line: List[int]) -> Tuple[List[int], int, bool]:
        """
        Slide and merge a single line to the left.

        :param line: List of 4 integers
        :return: Tuple of (new_line, score_gained, moved)
        """
        # Remove zeros
        filtered = [x for x in line if x != 0]

        # Merge adjacent equal values
        merged: List[int] = []
        score_gained = 0
        skip = False

        for i in range(len(filtered)):
            if skip:
                skip = False
                continue
            if i + 1 < len(filtered) and filtered[i] == filtered[i + 1]:
                new_val = filtered[i] * 2
                merged.append(new_val)
                score_gained += new_val
                skip = True
            else:
                merged.append(filtered[i])

        # Pad with zeros
        while len(merged) < GRID_SIZE:
            merged.append(0)

        moved = merged != line
        return merged, score_gained, moved

    def move(self, direction: Move) -> bool:
        """
        Make a move in the specified direction.

        :param direction: The direction to move
        :return: True if any tile moved, False otherwise
        """
        if self.game_over:
            return False

        moved = False

        if direction == Move.LEFT:
            for row in range(GRID_SIZE):
                line = self.cells[row][:]
                new_line, score, line_moved = self._slide_and_merge_line(line)
                self.cells[row] = new_line
                self.score += score
                if line_moved:
                    moved = True

        elif direction == Move.RIGHT:
            for row in range(GRID_SIZE):
                line = self.cells[row][::-1]
                new_line, score, line_moved = self._slide_and_merge_line(line)
                self.cells[row] = new_line[::-1]
                self.score += score
                if line_moved:
                    moved = True

        elif direction == Move.UP:
            for col in range(GRID_SIZE):
                line = [self.cells[row][col] for row in range(GRID_SIZE)]
                new_line, score, line_moved = self._slide_and_merge_line(line)
                for row in range(GRID_SIZE):
                    self.cells[row][col] = new_line[row]
                self.score += score
                if line_moved:
                    moved = True

        elif direction == Move.DOWN:
            for col in range(GRID_SIZE):
                line = [self.cells[GRID_SIZE - 1 - row][col] for row in range(GRID_SIZE)]
                new_line, score, line_moved = self._slide_and_merge_line(line)
                for row in range(GRID_SIZE):
                    self.cells[GRID_SIZE - 1 - row][col] = new_line[row]
                self.score += score
                if line_moved:
                    moved = True

        if moved:
            self.spawn_random()
            self._check_game_state()

        return moved

    def _check_game_state(self) -> None:
        """Check if the game is won or over."""
        # Check for 2048 tile
        for row in range(GRID_SIZE):
            for col in range(GRID_SIZE):
                if self.cells[row][col] >= 2048:
                    self.won = True

        # Check if can move
        if not self.can_move():
            self.game_over = True

    def can_move(self) -> bool:
        """
        Check if any move is possible.

        :return: True if a move is possible, False otherwise
        """
        # Check for empty cells
        for row in range(GRID_SIZE):
            for col in range(GRID_SIZE):
                if self.cells[row][col] == 0:
                    return True

        # Check for adjacent equal values
        for row in range(GRID_SIZE):
            for col in range(GRID_SIZE):
                val = self.cells[row][col]
                # Check right neighbor
                if col + 1 < GRID_SIZE and self.cells[row][col + 1] == val:
                    return True
                # Check bottom neighbor
                if row + 1 < GRID_SIZE and self.cells[row + 1][col] == val:
                    return True

        return False

    def get(self, row: int, col: int) -> int:
        """Get value at position."""
        if 0 <= row < GRID_SIZE and 0 <= col < GRID_SIZE:
            return self.cells[row][col]
        return 0

    def set(self, row: int, col: int, val: int) -> None:
        """Set value at position."""
        if 0 <= row < GRID_SIZE and 0 <= col < GRID_SIZE:
            self.cells[row][col] = val
