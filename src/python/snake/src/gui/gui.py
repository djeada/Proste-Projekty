"""
GUI for Snake game using pygame.
"""
import pygame
import sys
import os

sys.path.insert(
    0, os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
)
from logic.game import SnakeGame, Direction


# Colors
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
GREEN = (0, 200, 0)
DARK_GREEN = (0, 100, 0)
RED = (255, 0, 0)


class Gui:
    """
    Main window for the Snake game using pygame.
    """

    CELL_SIZE = 20
    GAME_SPEED = 10  # frames per second

    def __init__(self, game: SnakeGame) -> None:
        pygame.init()
        pygame.display.set_caption("Snake")
        self.game = game
        self.canvas_width = game.width * self.CELL_SIZE
        self.canvas_height = game.height * self.CELL_SIZE
        self.screen = pygame.display.set_mode(
            (self.canvas_width, self.canvas_height + 40)
        )
        self.clock = pygame.time.Clock()
        self.font = pygame.font.Font(None, 36)
        self.small_font = pygame.font.Font(None, 24)

    def run(self) -> None:
        """Start the main game loop."""
        running = True
        while running:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    running = False
                elif event.type == pygame.KEYDOWN:
                    if event.key == pygame.K_RIGHT:
                        self.game.update_direction(Direction.RIGHT)
                    elif event.key == pygame.K_LEFT:
                        self.game.update_direction(Direction.LEFT)
                    elif event.key == pygame.K_UP:
                        self.game.update_direction(Direction.UP)
                    elif event.key == pygame.K_DOWN:
                        self.game.update_direction(Direction.DOWN)
                    elif event.key == pygame.K_r:
                        self.restart()

            if not self.game.game_over:
                self.game.move()

            self.draw()
            self.clock.tick(self.GAME_SPEED)

        pygame.quit()

    def draw(self) -> None:
        """Draw the game state on the screen."""
        self.screen.fill(BLACK)

        # Draw score
        score_text = self.font.render(
            f"Score: {self.game.score}", True, WHITE
        )
        self.screen.blit(score_text, (10, self.canvas_height + 5))

        # Draw restart hint
        if self.game.game_over:
            hint_text = self.small_font.render("Press R to restart", True, WHITE)
            self.screen.blit(hint_text, (self.canvas_width - 150, self.canvas_height + 10))

        # Draw food
        food_x, food_y = self.game.food
        pygame.draw.rect(
            self.screen,
            RED,
            (
                food_x * self.CELL_SIZE,
                food_y * self.CELL_SIZE,
                self.CELL_SIZE,
                self.CELL_SIZE,
            ),
        )

        # Draw snake head
        head_x, head_y = self.game.get_snake_head()
        pygame.draw.rect(
            self.screen,
            DARK_GREEN,
            (
                head_x * self.CELL_SIZE,
                head_y * self.CELL_SIZE,
                self.CELL_SIZE,
                self.CELL_SIZE,
            ),
        )

        # Draw snake body
        for x, y in self.game.get_snake_body():
            pygame.draw.rect(
                self.screen,
                GREEN,
                (
                    x * self.CELL_SIZE,
                    y * self.CELL_SIZE,
                    self.CELL_SIZE,
                    self.CELL_SIZE,
                ),
            )

        # Draw game over overlay
        if self.game.game_over:
            overlay = pygame.Surface((self.canvas_width, self.canvas_height))
            overlay.set_alpha(128)
            overlay.fill(BLACK)
            self.screen.blit(overlay, (0, 0))

            game_over_text = self.font.render("GAME OVER", True, WHITE)
            text_rect = game_over_text.get_rect(
                center=(self.canvas_width // 2, self.canvas_height // 2)
            )
            self.screen.blit(game_over_text, text_rect)

        pygame.display.flip()

    def restart(self) -> None:
        """Restart the game."""
        self.game.reset()
