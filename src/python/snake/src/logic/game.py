"""
Game logic for Snake game.
"""
import random
from typing import List, Tuple
from enum import Enum, auto


class Direction(Enum):
    """Direction the snake is moving."""

    RIGHT = auto()
    LEFT = auto()
    UP = auto()
    DOWN = auto()


class SnakeGame:
    """
    Snake game logic.
    """

    MAX_SNAKE_LENGTH = 100

    def __init__(self, width: int = 20, height: int = 20) -> None:
        self.width = width
        self.height = height
        self.snake: List[Tuple[int, int]] = []
        self.direction = Direction.RIGHT
        self.food: Tuple[int, int] = (0, 0)
        self.game_over = False
        self.score = 0
        self.reset()

    def reset(self) -> None:
        """Reset the game to initial state."""
        start_x = self.width // 2
        start_y = self.height // 2
        self.snake = [(start_x, start_y)]
        self.direction = Direction.RIGHT
        self.game_over = False
        self.score = 0
        self.place_food()

    def place_food(self) -> None:
        """Place food at a random location not occupied by the snake."""
        while True:
            x = random.randint(0, self.width - 1)
            y = random.randint(0, self.height - 1)
            if (x, y) not in self.snake:
                self.food = (x, y)
                break

    def update_direction(self, new_direction: Direction) -> None:
        """
        Update snake direction if it's not opposite to current direction.

        :param new_direction: The new direction to set
        """
        opposites = {
            Direction.RIGHT: Direction.LEFT,
            Direction.LEFT: Direction.RIGHT,
            Direction.UP: Direction.DOWN,
            Direction.DOWN: Direction.UP,
        }
        if new_direction != opposites.get(self.direction):
            self.direction = new_direction

    def move(self) -> None:
        """Move the snake one step in the current direction."""
        if self.game_over:
            return

        head_x, head_y = self.snake[0]

        # Calculate new head position
        if self.direction == Direction.RIGHT:
            new_head = (head_x + 1, head_y)
        elif self.direction == Direction.LEFT:
            new_head = (head_x - 1, head_y)
        elif self.direction == Direction.UP:
            new_head = (head_x, head_y - 1)
        elif self.direction == Direction.DOWN:
            new_head = (head_x, head_y + 1)
        else:
            new_head = (head_x, head_y)

        # Check wall collision
        if (
            new_head[0] < 0
            or new_head[0] >= self.width
            or new_head[1] < 0
            or new_head[1] >= self.height
        ):
            self.game_over = True
            return

        # Check self collision
        if new_head in self.snake:
            self.game_over = True
            return

        # Add new head
        self.snake.insert(0, new_head)

        # Check if food eaten
        if new_head == self.food:
            self.score += 10
            if len(self.snake) < self.MAX_SNAKE_LENGTH:
                self.place_food()
            # Don't remove tail when eating food (snake grows)
        else:
            # Remove tail (snake moves)
            self.snake.pop()

    def get_snake_head(self) -> Tuple[int, int]:
        """Get the position of the snake's head."""
        return self.snake[0]

    def get_snake_body(self) -> List[Tuple[int, int]]:
        """Get the positions of the snake's body (excluding head)."""
        return self.snake[1:]
