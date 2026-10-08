"""Snake in a pygame window. Arrow keys or WASD steer, P pauses, R or Space restarts, Q quits."""
import pygame

from snake import HEIGHT, WIDTH, DOWN, LEFT, RIGHT, UP, SnakeGame

CELL = 30
STATUS_HEIGHT = 70
BACKGROUND = (30, 30, 38)
WHITE = (255, 255, 255)
GREY = (120, 120, 120)
GREEN = (0, 200, 0)
DARK_GREEN = (0, 110, 0)
RED = (230, 50, 50)

KEY_DIRECTIONS = {
    pygame.K_UP: UP, pygame.K_w: UP,
    pygame.K_DOWN: DOWN, pygame.K_s: DOWN,
    pygame.K_LEFT: LEFT, pygame.K_a: LEFT,
    pygame.K_RIGHT: RIGHT, pygame.K_d: RIGHT,
}


class Gui:
    def __init__(self, game):
        pygame.init()
        pygame.display.set_caption("Snake")
        self.game = game
        self.screen = pygame.display.set_mode((WIDTH * CELL, HEIGHT * CELL + STATUS_HEIGHT))
        self.clock = pygame.time.Clock()
        self.font = pygame.font.Font(None, 32)
        self.paused = False

    def run(self):
        next_step = pygame.time.get_ticks() + self.game.delay_ms()
        running = True
        while running:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    running = False
                elif event.type == pygame.KEYDOWN:
                    running = self.handle_key(event.key)
            if not self.paused and pygame.time.get_ticks() >= next_step:
                self.game.step()
                next_step = pygame.time.get_ticks() + self.game.delay_ms()
            self.draw()
            self.clock.tick(60)
        pygame.quit()

    def handle_key(self, key):
        """Returns False when the player wants to quit."""
        if key in KEY_DIRECTIONS:
            self.game.turn(KEY_DIRECTIONS[key])
        elif key == pygame.K_p:
            self.paused = not self.paused
        elif key in (pygame.K_r, pygame.K_SPACE) and self.game.game_over:
            self.game.reset()
            self.paused = False
        elif key in (pygame.K_q, pygame.K_ESCAPE):
            return False
        return True

    def draw_cell(self, x, y, color):
        pygame.draw.rect(self.screen, color, (x * CELL + 1, y * CELL + 1, CELL - 2, CELL - 2))

    def draw(self):
        self.screen.fill(BACKGROUND)
        if self.game.food is not None:
            self.draw_cell(*self.game.food, RED)
        for i, (x, y) in enumerate(self.game.body):
            self.draw_cell(x, y, DARK_GREEN if i == 0 else GREEN)

        status = f"Score: {self.game.score}"
        if self.game.game_over:
            status += "   Game over! Press R or Space to restart"
        elif self.paused:
            status += "   Paused (P to continue)"
        text = self.font.render(status, True, WHITE)
        self.screen.blit(text, (10, HEIGHT * CELL + 8))
        hint = self.font.render("Arrows/WASD: move   P: pause   Q: quit", True, GREY)
        self.screen.blit(hint, (10, HEIGHT * CELL + 38))
        pygame.display.flip()


def main():
    Gui(SnakeGame()).run()


if __name__ == "__main__":
    main()
