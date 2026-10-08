"""The rules of Snake: no input or output here."""
import random

WIDTH = 20
HEIGHT = 15

UP = (0, -1)
DOWN = (0, 1)
LEFT = (-1, 0)
RIGHT = (1, 0)


def opposite(direction):
    return (-direction[0], -direction[1])


class SnakeGame:
    def __init__(self, random_below=random.randrange):
        """random_below(n) returns a random number in [0, n); tests pass a fake one."""
        self.random_below = random_below
        self.reset()

    def reset(self):
        self.body = [(WIDTH // 2, HEIGHT // 2)]  # body[0] is the head
        self.direction = RIGHT  # the direction of the last step
        self.next_direction = RIGHT  # the direction of the next step
        self.score = 0
        self.game_over = False
        self.food = None
        self.place_food()

    def turn(self, direction):
        # A turn that would reverse the last step is ignored, so the snake never runs into its neck.
        if direction != opposite(self.direction):
            self.next_direction = direction

    def step(self):
        if self.game_over:
            return
        self.direction = self.next_direction
        head_x, head_y = self.body[0]
        dx, dy = self.direction
        head = (head_x + dx, head_y + dy)

        if not (0 <= head[0] < WIDTH and 0 <= head[1] < HEIGHT) or head in self.body:
            self.game_over = True
            return

        eats = head == self.food
        self.body.insert(0, head)
        if eats:
            self.score += 10
            self.place_food()
        else:
            self.body.pop()  # without eating, the tail leaves its cell

    def place_food(self):
        """Put the food on a random free cell."""
        free = [(x, y) for y in range(HEIGHT) for x in range(WIDTH) if (x, y) not in self.body]
        if not free:
            self.game_over = True  # the board is full
            self.food = None
            return
        self.food = free[self.random_below(len(free))]

    def delay_ms(self):
        """The time between two steps: the snake gets faster as it grows, but not faster than 60 ms."""
        return max(60, 150 - 5 * (len(self.body) - 1))
