"""
Battleship game logic.
"""
import random
from typing import List, Tuple, Optional
from dataclasses import dataclass, field
from enum import Enum, auto


BOARD_SIZE = 10


class CellState(Enum):
    """State of a cell on the board."""

    EMPTY = auto()
    SHIP = auto()
    HIT = auto()
    MISS = auto()


@dataclass
class Ship:
    """Represents a ship."""

    length: int
    positions: List[Tuple[int, int]] = field(default_factory=list)
    hits: int = 0

    def is_sunk(self) -> bool:
        """Check if the ship is sunk."""
        return self.hits >= self.length


class Board:
    """
    Battleship game board.
    """

    SHIP_LENGTHS = [5, 4, 3, 3, 2]  # Standard Battleship fleet

    def __init__(self) -> None:
        self.grid: List[List[CellState]] = [
            [CellState.EMPTY for _ in range(BOARD_SIZE)] for _ in range(BOARD_SIZE)
        ]
        self.ships: List[Ship] = []

    def can_place_ship(
        self, x: int, y: int, length: int, horizontal: bool
    ) -> bool:
        """Check if a ship can be placed at the given position."""
        for i in range(length):
            if horizontal:
                nx, ny = x + i, y
            else:
                nx, ny = x, y + i

            if nx < 0 or nx >= BOARD_SIZE or ny < 0 or ny >= BOARD_SIZE:
                return False
            if self.grid[ny][nx] == CellState.SHIP:
                return False

        return True

    def place_ship(
        self, x: int, y: int, length: int, horizontal: bool
    ) -> bool:
        """
        Place a ship on the board.

        :param x: Starting x coordinate
        :param y: Starting y coordinate
        :param length: Ship length
        :param horizontal: True for horizontal, False for vertical
        :return: True if successfully placed
        """
        if not self.can_place_ship(x, y, length, horizontal):
            return False

        ship = Ship(length=length)

        for i in range(length):
            if horizontal:
                nx, ny = x + i, y
            else:
                nx, ny = x, y + i

            self.grid[ny][nx] = CellState.SHIP
            ship.positions.append((nx, ny))

        self.ships.append(ship)
        return True

    def place_ships_randomly(self) -> None:
        """Place all ships randomly on the board."""
        for length in self.SHIP_LENGTHS:
            placed = False
            attempts = 0
            while not placed and attempts < 1000:
                x = random.randint(0, BOARD_SIZE - 1)
                y = random.randint(0, BOARD_SIZE - 1)
                horizontal = random.choice([True, False])
                placed = self.place_ship(x, y, length, horizontal)
                attempts += 1

    def fire(self, x: int, y: int) -> Tuple[bool, Optional[Ship]]:
        """
        Fire at a position.

        :param x: X coordinate
        :param y: Y coordinate
        :return: Tuple of (hit, sunk_ship or None)
        """
        if x < 0 or x >= BOARD_SIZE or y < 0 or y >= BOARD_SIZE:
            return (False, None)

        cell = self.grid[y][x]

        if cell == CellState.HIT or cell == CellState.MISS:
            return (False, None)  # Already fired here

        if cell == CellState.SHIP:
            self.grid[y][x] = CellState.HIT

            # Find and update the ship
            for ship in self.ships:
                if (x, y) in ship.positions:
                    ship.hits += 1
                    if ship.is_sunk():
                        return (True, ship)
                    return (True, None)
        else:
            self.grid[y][x] = CellState.MISS

        return (False, None)

    def all_ships_sunk(self) -> bool:
        """Check if all ships are sunk."""
        return all(ship.is_sunk() for ship in self.ships)

    def get_remaining_ships(self) -> int:
        """Get the number of ships still floating."""
        return sum(1 for ship in self.ships if not ship.is_sunk())


class BattleshipGame:
    """
    Main Battleship game controller.
    """

    def __init__(self) -> None:
        self.player_board = Board()
        self.enemy_board = Board()
        self.player_turn = True
        self.game_over = False
        self.winner: Optional[str] = None

    def setup(self) -> None:
        """Set up the game with random ship placements."""
        self.player_board.place_ships_randomly()
        self.enemy_board.place_ships_randomly()

    def player_fire(self, x: int, y: int) -> Tuple[bool, Optional[Ship]]:
        """Player fires at enemy board."""
        if self.game_over or not self.player_turn:
            return (False, None)

        hit, sunk_ship = self.enemy_board.fire(x, y)

        if self.enemy_board.all_ships_sunk():
            self.game_over = True
            self.winner = "Player"
        else:
            self.player_turn = False

        return (hit, sunk_ship)

    def enemy_fire(self) -> Tuple[int, int, bool]:
        """
        Enemy AI fires at player board.

        :return: Tuple of (x, y, hit)
        """
        if self.game_over or self.player_turn:
            return (-1, -1, False)

        # Simple random AI
        while True:
            x = random.randint(0, BOARD_SIZE - 1)
            y = random.randint(0, BOARD_SIZE - 1)

            cell = self.player_board.grid[y][x]
            if cell != CellState.HIT and cell != CellState.MISS:
                break

        hit, _ = self.player_board.fire(x, y)

        if self.player_board.all_ships_sunk():
            self.game_over = True
            self.winner = "Enemy"
        else:
            self.player_turn = True

        return (x, y, hit)

    def reset(self) -> None:
        """Reset the game."""
        self.player_board = Board()
        self.enemy_board = Board()
        self.player_turn = True
        self.game_over = False
        self.winner = None
        self.setup()
