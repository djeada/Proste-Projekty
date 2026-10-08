"""Battleship rules: boards, ship placement, shooting and the computer player."""

BOARD_SIZE = 10
FLEET = (5, 4, 3, 3, 2)

MISS = "miss"
HIT = "hit"
SUNK = "sunk"
REPEAT = "repeat"      # this cell was already shot
INVALID = "invalid"    # outside the board


def in_bounds(x, y):
    return 0 <= x < BOARD_SIZE and 0 <= y < BOARD_SIZE


def ship_cells(length, x, y, horizontal):
    dx, dy = (1, 0) if horizontal else (0, 1)
    return [(x + dx * i, y + dy * i) for i in range(length)]


class Ship:
    def __init__(self, length):
        self.length = length
        self.hits = 0
        self.placed = False

    @property
    def sunk(self):
        return self.placed and self.hits == self.length


class Board:
    def __init__(self):
        self.reset()

    def reset(self):
        self.ships = [Ship(length) for length in FLEET]
        self.ship_at = [[None] * BOARD_SIZE for _ in range(BOARD_SIZE)]
        self.shot = [[False] * BOARD_SIZE for _ in range(BOARD_SIZE)]

    def can_place(self, index, x, y, horizontal):
        ship = self.ships[index]
        if ship.placed:
            return False
        for cx, cy in ship_cells(ship.length, x, y, horizontal):
            if not in_bounds(cx, cy) or self.ship_at[cy][cx] is not None:
                return False
        return True

    def place(self, index, x, y, horizontal):
        if not self.can_place(index, x, y, horizontal):
            return False
        ship = self.ships[index]
        for cx, cy in ship_cells(ship.length, x, y, horizontal):
            self.ship_at[cy][cx] = index
        ship.placed = True
        return True

    def place_randomly(self, rng):
        """Replace the whole fleet with a random valid one. Ships may touch but not overlap."""
        placed = False
        while not placed:
            self.reset()
            placed = all(self._place_one_randomly(index, rng) for index in range(len(FLEET)))

    def _place_one_randomly(self, index, rng):
        for _ in range(1000):
            if self.place(index, rng.randrange(BOARD_SIZE), rng.randrange(BOARD_SIZE), rng.random() < 0.5):
                return True
        return False

    def fleet_placed(self):
        return all(ship.placed for ship in self.ships)

    def fire(self, x, y):
        if not in_bounds(x, y):
            return INVALID
        if self.shot[y][x]:
            return REPEAT
        self.shot[y][x] = True
        index = self.ship_at[y][x]
        if index is None:
            return MISS
        ship = self.ships[index]
        ship.hits += 1
        return SUNK if ship.sunk else HIT

    def all_sunk(self):
        return all(ship.sunk for ship in self.ships)


class Computer:
    """Hunt and target: random shots until a hit, then the neighbours of the hit."""

    def __init__(self):
        self.targets = []

    def choose(self, board, rng):
        while self.targets:
            x, y = self.targets.pop()
            if not board.shot[y][x]:
                return x, y
        unshot = [(x, y) for y in range(BOARD_SIZE) for x in range(BOARD_SIZE) if not board.shot[y][x]]
        return rng.choice(unshot)

    def report(self, board, x, y, result):
        if result == HIT:
            for nx, ny in ((x + 1, y), (x - 1, y), (x, y + 1), (x, y - 1)):
                if in_bounds(nx, ny) and not board.shot[ny][nx]:
                    self.targets.append((nx, ny))
        elif result == SUNK:
            self.targets.clear()
