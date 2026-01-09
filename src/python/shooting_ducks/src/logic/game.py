"""
Shooting ducks game logic.
"""
import random
from typing import List, Tuple
from dataclasses import dataclass
from enum import Enum, auto


class Direction(Enum):
    """Direction for duck movement."""

    LEFT = auto()
    RIGHT = auto()
    UP = auto()
    DOWN = auto()
    NONE = auto()


@dataclass
class Duck:
    """Represents a duck target."""

    x: int
    y: int
    alive: bool
    direction: Direction
    speed: int = 2
    width: int = 40
    height: int = 30

    def move(self, max_x: int, max_y: int) -> None:
        """Move the duck according to its direction."""
        if not self.alive:
            return

        if self.direction == Direction.LEFT:
            self.x -= self.speed
            if self.x < 0:
                self.direction = Direction.RIGHT
        elif self.direction == Direction.RIGHT:
            self.x += self.speed
            if self.x > max_x - self.width:
                self.direction = Direction.LEFT
        elif self.direction == Direction.UP:
            self.y -= self.speed
            if self.y < 0:
                self.direction = Direction.DOWN
        elif self.direction == Direction.DOWN:
            self.y += self.speed
            if self.y > max_y - self.height:
                self.direction = Direction.UP

    def contains_point(self, px: int, py: int) -> bool:
        """Check if a point is within the duck's bounds."""
        return (
            self.x <= px <= self.x + self.width
            and self.y <= py <= self.y + self.height
        )


class DuckGame:
    """
    Duck shooting game logic.
    """

    MAX_DUCKS = 32
    INITIAL_LIVES = 3
    BASE_DUCKS = 6
    DUCKS_PER_LEVEL = 2

    def __init__(self, width: int = 600, height: int = 400) -> None:
        self.width = width
        self.height = height
        self.ducks: List[Duck] = []
        self.lives = self.INITIAL_LIVES
        self.score = 0
        self.level = 1
        self.game_over = False
        self.paused = False
        self.tick = 0
        self.reset()

    def reset(self) -> None:
        """Reset the game to initial state."""
        self.ducks = []
        self.lives = self.INITIAL_LIVES
        self.score = 0
        self.level = 1
        self.game_over = False
        self.paused = False
        self.spawn_ducks()

    def spawn_ducks(self) -> None:
        """Spawn ducks for the current level."""
        num_ducks = min(
            self.BASE_DUCKS + (self.level - 1) * self.DUCKS_PER_LEVEL,
            self.MAX_DUCKS,
        )

        self.ducks = []
        for _ in range(num_ducks):
            x = random.randint(0, self.width - 50)
            y = random.randint(50, self.height // 2)
            direction = random.choice([Direction.LEFT, Direction.RIGHT])
            speed = 1 + self.level // 2

            self.ducks.append(
                Duck(x=x, y=y, alive=True, direction=direction, speed=speed)
            )

    def update(self) -> None:
        """Update game state (move ducks, etc.)."""
        if self.game_over or self.paused:
            return

        self.tick += 1

        for duck in self.ducks:
            if duck.alive:
                duck.move(self.width, self.height)

        # Check if all ducks are shot
        if all(not duck.alive for duck in self.ducks):
            self.next_level()

    def shoot(self, x: int, y: int) -> bool:
        """
        Attempt to shoot at the given coordinates.

        :param x: X coordinate of shot
        :param y: Y coordinate of shot
        :return: True if a duck was hit
        """
        if self.game_over or self.paused:
            return False

        for duck in self.ducks:
            if duck.alive and duck.contains_point(x, y):
                duck.alive = False
                self.score += 10 * self.level
                return True

        # Missed shot
        self.lives -= 1
        if self.lives <= 0:
            self.game_over = True

        return False

    def next_level(self) -> None:
        """Advance to the next level."""
        self.level += 1
        self.lives = min(self.lives + 1, self.INITIAL_LIVES)
        self.spawn_ducks()

    def toggle_pause(self) -> None:
        """Toggle pause state."""
        self.paused = not self.paused

    def get_alive_duck_count(self) -> int:
        """Get number of alive ducks."""
        return sum(1 for duck in self.ducks if duck.alive)

    def get_status(self) -> str:
        """Get game status string."""
        if self.game_over:
            return f"Game Over! Final Score: {self.score}"
        if self.paused:
            return "PAUSED"
        return f"Level: {self.level} | Score: {self.score} | Lives: {self.lives}"
