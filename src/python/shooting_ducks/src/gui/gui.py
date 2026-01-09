"""
GUI for Shooting Ducks game using pygame.
"""
import pygame
import sys
import os

sys.path.insert(
    0, os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
)
from logic.game import DuckGame


# Colors
SKY_BLUE = (135, 206, 235)
GREEN = (34, 139, 34)
ORANGE = (255, 165, 0)
DARK_ORANGE = (255, 140, 0)
YELLOW = (255, 255, 0)
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
RED = (255, 0, 0)


class Gui:
    """
    Main window for the Shooting Ducks game using pygame.
    """

    GAME_SPEED = 60  # frames per second

    def __init__(self, game: DuckGame) -> None:
        pygame.init()
        pygame.display.set_caption("Shooting Ducks")
        self.game = game
        self.screen = pygame.display.set_mode((game.width, game.height + 40))
        self.clock = pygame.time.Clock()
        self.font = pygame.font.Font(None, 24)
        self.large_font = pygame.font.Font(None, 48)

    def run(self) -> None:
        """Start the main game loop."""
        running = True
        while running:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    running = False
                elif event.type == pygame.MOUSEBUTTONDOWN:
                    if event.button == 1:  # Left click
                        self.on_click(event.pos)
                elif event.type == pygame.KEYDOWN:
                    if event.key == pygame.K_p:
                        self.game.toggle_pause()
                    elif event.key == pygame.K_r:
                        self.restart()

            self.game.update()
            self.draw()
            self.clock.tick(self.GAME_SPEED)

        pygame.quit()

    def draw(self) -> None:
        """Draw the game state on the screen."""
        # Draw sky background
        self.screen.fill(SKY_BLUE)

        # Draw ground
        pygame.draw.rect(
            self.screen,
            GREEN,
            (0, self.game.height - 50, self.game.width, 50),
        )

        # Draw ducks
        for duck in self.game.ducks:
            if duck.alive:
                # Draw duck body (oval shape approximated with ellipse)
                pygame.draw.ellipse(
                    self.screen,
                    ORANGE,
                    (duck.x, duck.y, duck.width, duck.height),
                )
                pygame.draw.ellipse(
                    self.screen,
                    DARK_ORANGE,
                    (duck.x, duck.y, duck.width, duck.height),
                    2,
                )
                # Draw duck head
                pygame.draw.ellipse(
                    self.screen,
                    ORANGE,
                    (duck.x + duck.width - 15, duck.y - 5, 20, 20),
                )
                pygame.draw.ellipse(
                    self.screen,
                    DARK_ORANGE,
                    (duck.x + duck.width - 15, duck.y - 5, 20, 20),
                    2,
                )
                # Draw beak
                pygame.draw.polygon(
                    self.screen,
                    YELLOW,
                    [
                        (duck.x + duck.width + 5, duck.y + 5),
                        (duck.x + duck.width + 15, duck.y + 5),
                        (duck.x + duck.width + 5, duck.y + 10),
                    ],
                )

        # Draw crosshair hint
        if not self.game.game_over and not self.game.paused:
            hint_text = self.font.render("Click on ducks to shoot!", True, WHITE)
            hint_rect = hint_text.get_rect(center=(self.game.width // 2, 20))
            self.screen.blit(hint_text, hint_rect)

        # Draw status bar
        status_text = self.font.render(self.game.get_status(), True, BLACK)
        self.screen.blit(status_text, (10, self.game.height + 10))

        # Draw game over overlay
        if self.game.game_over:
            overlay = pygame.Surface((self.game.width, self.game.height))
            overlay.set_alpha(128)
            overlay.fill(BLACK)
            self.screen.blit(overlay, (0, 0))

            game_over_text = self.large_font.render("GAME OVER", True, WHITE)
            text_rect = game_over_text.get_rect(
                center=(self.game.width // 2, self.game.height // 2 - 20)
            )
            self.screen.blit(game_over_text, text_rect)

            score_text = self.font.render(
                f"Final Score: {self.game.score}", True, WHITE
            )
            score_rect = score_text.get_rect(
                center=(self.game.width // 2, self.game.height // 2 + 20)
            )
            self.screen.blit(score_text, score_rect)

            restart_text = self.font.render("Press R to restart", True, WHITE)
            restart_rect = restart_text.get_rect(
                center=(self.game.width // 2, self.game.height // 2 + 50)
            )
            self.screen.blit(restart_text, restart_rect)

        # Draw pause overlay
        if self.game.paused and not self.game.game_over:
            overlay = pygame.Surface((self.game.width, self.game.height))
            overlay.set_alpha(128)
            overlay.fill(BLACK)
            self.screen.blit(overlay, (0, 0))

            pause_text = self.large_font.render("PAUSED", True, WHITE)
            text_rect = pause_text.get_rect(
                center=(self.game.width // 2, self.game.height // 2)
            )
            self.screen.blit(pause_text, text_rect)

        pygame.display.flip()

    def on_click(self, pos: tuple) -> None:
        """Handle mouse click."""
        x, y = pos
        if y < self.game.height:  # Only count clicks in game area
            self.game.shoot(x, y)

    def restart(self) -> None:
        """Restart the game."""
        self.game.reset()
