"""Rules of 2048: sliding, merging, spawning and win/lose checks. No input or output here."""
import random

SIZE = 4
PLAYING = "playing"
WON = "won"
LOST = "lost"


def slide_row(line):
    """Merge one line towards its start. Returns (new_line, points gained)."""
    tiles = [value for value in line if value]
    merged = []
    points = 0
    i = 0
    while i < len(tiles):
        if i + 1 < len(tiles) and tiles[i] == tiles[i + 1]:
            # Two equal tiles merge once; the next tile is not looked at again.
            merged.append(2 * tiles[i])
            points += 2 * tiles[i]
            i += 2
        else:
            merged.append(tiles[i])
            i += 1
    return merged + [0] * (len(line) - len(merged)), points


def _line_cells(direction, index):
    """Cells of one row or column, ordered from the side the tiles slide towards."""
    last = SIZE - 1
    if direction == "left":
        return [(index, k) for k in range(SIZE)]
    if direction == "right":
        return [(index, last - k) for k in range(SIZE)]
    if direction == "up":
        return [(k, index) for k in range(SIZE)]
    return [(last - k, index) for k in range(SIZE)]  # down


def slide_board(board, direction):
    """Slide every line of the board in place. Returns the points gained."""
    points = 0
    for index in range(SIZE):
        cells = _line_cells(direction, index)
        new_line, gained = slide_row([board[r][c] for r, c in cells])
        points += gained
        for (r, c), value in zip(cells, new_line):
            board[r][c] = value
    return points


class Game:
    """A game of 2048. The board is a list of rows; 0 means an empty cell."""

    def __init__(self, random_below=random.randrange):
        self.random_below = random_below  # returns an integer in [0, n)
        self.board = [[0] * SIZE for _ in range(SIZE)]
        self.score = 0
        self.keep_playing = False
        self.spawn_tile()
        self.spawn_tile()

    def spawn_tile(self):
        """Put a 2 (90%) or a 4 (10%) on a random empty cell. Returns False if the board is full."""
        empty = [(r, c) for r in range(SIZE) for c in range(SIZE) if self.board[r][c] == 0]
        if not empty:
            return False
        r, c = empty[self.random_below(len(empty))]
        self.board[r][c] = 4 if self.random_below(10) == 0 else 2
        return True

    def move(self, direction):
        """Slide in a direction. Returns True if the board changed (then a new tile appears)."""
        if self.status() != PLAYING:
            return False
        before = [row[:] for row in self.board]
        self.score += slide_board(self.board, direction)
        if self.board == before:
            return False
        self.spawn_tile()
        return True

    def can_move(self):
        for r in range(SIZE):
            for c in range(SIZE):
                value = self.board[r][c]
                if value == 0:
                    return True
                if r + 1 < SIZE and self.board[r + 1][c] == value:
                    return True
                if c + 1 < SIZE and self.board[r][c + 1] == value:
                    return True
        return False

    def status(self):
        reached_2048 = any(value >= 2048 for row in self.board for value in row)
        if reached_2048 and not self.keep_playing:
            return WON
        if not self.can_move():
            return LOST
        return PLAYING
