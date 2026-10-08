"""Minesweeper rules: mines, numbers, reveal with flood fill, flags. No input or output."""

import random


class Game:
    """One game on a rows x cols board. The first reveal places the mines."""

    def __init__(self, rows: int, cols: int, mines: int, rng: random.Random) -> None:
        self.rows = rows
        self.cols = cols
        self.mine_total = mines
        self.rng = rng
        self.mines_placed = False
        self.mine = [[False] * cols for _ in range(rows)]
        self.neighbors = [[0] * cols for _ in range(rows)]
        self.revealed = [[False] * cols for _ in range(rows)]
        self.flagged = [[False] * cols for _ in range(rows)]
        self.revealed_count = 0
        self.flag_count = 0
        self.state = "playing"  # "playing", "won" or "lost"

    @property
    def mines_left(self) -> int:
        return self.mine_total - self.flag_count

    def reveal(self, row: int, col: int) -> None:
        if self.state != "playing" or not self._in_board(row, col):
            return
        if self.revealed[row][col] or self.flagged[row][col]:
            return
        if not self.mines_placed:
            self._place_mines(row, col)
        if self.mine[row][col]:
            for r in range(self.rows):
                for c in range(self.cols):
                    if self.mine[r][c]:
                        self.revealed[r][c] = True
            self.state = "lost"
            return
        self._reveal_cell(row, col)
        if self.revealed_count == self.rows * self.cols - self.mine_total:
            self.state = "won"

    def toggle_flag(self, row: int, col: int) -> None:
        if self.state != "playing" or not self._in_board(row, col) or self.revealed[row][col]:
            return
        self.flagged[row][col] = not self.flagged[row][col]
        self.flag_count += 1 if self.flagged[row][col] else -1

    def _in_board(self, row: int, col: int) -> bool:
        return 0 <= row < self.rows and 0 <= col < self.cols

    def _place_mines(self, safe_row: int, safe_col: int) -> None:
        """Places random mines, skipping the first clicked cell and its neighbors."""
        placed = 0
        while placed < self.mine_total:
            row = self.rng.randrange(self.rows)
            col = self.rng.randrange(self.cols)
            if self.mine[row][col] or (abs(row - safe_row) <= 1 and abs(col - safe_col) <= 1):
                continue
            self.mine[row][col] = True
            placed += 1
        for r in range(self.rows):
            for c in range(self.cols):
                self.neighbors[r][c] = self._count_mines_around(r, c)
        self.mines_placed = True

    def _count_mines_around(self, row: int, col: int) -> int:
        return sum(
            self.mine[r][c]
            for r in range(row - 1, row + 2)
            for c in range(col - 1, col + 2)
            if (r, c) != (row, col) and self._in_board(r, c)
        )

    def _reveal_cell(self, row: int, col: int) -> None:
        """Opens a cell. An empty cell (no mines around) also opens its neighbors: flood fill."""
        if not self._in_board(row, col) or self.revealed[row][col] or self.flagged[row][col]:
            return
        self.revealed[row][col] = True
        self.revealed_count += 1
        if self.neighbors[row][col] == 0:
            for r in range(row - 1, row + 2):
                for c in range(col - 1, col + 2):
                    self._reveal_cell(r, c)
