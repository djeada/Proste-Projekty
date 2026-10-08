"""Pygame window for Shooting Ducks: drawing, mouse clicks and the main loop."""

import time

import pygame

from shooting_ducks import FIELD_HEIGHT, FIELD_WIDTH, Game

WIDTH, HEIGHT = 800, 500
GROUND_Y = int(HEIGHT * 0.85)
SKY_TOP = (70, 140, 220)
SKY_HORIZON = (200, 230, 250)
GRASS = (76, 160, 60)
GRASS_DARK = (56, 128, 44)
DUCK_YELLOW = (255, 210, 40)
DUCK_ORANGE = (230, 140, 30)
BEAK = (250, 120, 30)
WHITE = (255, 255, 255)
BLACK = (20, 20, 30)
RED = (220, 50, 50)


def to_pixels(x, y):
    return x * WIDTH / FIELD_WIDTH, y * HEIGHT / FIELD_HEIGHT


def from_pixels(px, py):
    return px * FIELD_WIDTH / WIDTH, py * FIELD_HEIGHT / HEIGHT


def make_background():
    background = pygame.Surface((WIDTH, HEIGHT))
    for y in range(GROUND_Y):
        t = y / GROUND_Y
        color = [int(a + (b - a) * t) for a, b in zip(SKY_TOP, SKY_HORIZON)]
        pygame.draw.line(background, color, (0, y), (WIDTH, y))
    for cx, cy in ((120, 70), (400, 45), (660, 90)):
        for dx, r in ((-18, 16), (0, 22), (20, 16)):
            pygame.draw.circle(background, WHITE, (cx + dx, cy), r)
    pygame.draw.rect(background, GRASS, (0, GROUND_Y, WIDTH, HEIGHT - GROUND_Y))
    for x in range(0, WIDTH, 14):
        pygame.draw.polygon(background, GRASS_DARK, [(x, GROUND_Y), (x + 7, GROUND_Y - 10), (x + 14, GROUND_Y)])
    return background


def draw_duck(surface, duck):
    cx, cy = to_pixels(duck.x, duck.y)
    direction = 1 if duck.speed > 0 else -1
    body = pygame.Rect(0, 0, 60, 28)
    body.center = (round(cx), round(cy + 6))
    pygame.draw.ellipse(surface, DUCK_YELLOW, body)
    head = (round(cx + direction * 20), round(cy - 6))
    pygame.draw.circle(surface, DUCK_YELLOW, head, 13)
    beak = [
        (head[0] + direction * 10, head[1] - 3),
        (head[0] + direction * 24, head[1] + 1),
        (head[0] + direction * 10, head[1] + 5),
    ]
    pygame.draw.polygon(surface, BEAK, beak)
    pygame.draw.circle(surface, BLACK, (head[0] + direction * 5, head[1] - 3), 3)
    wing = pygame.Rect(0, 0, 30, 16)
    wing.center = (round(cx - direction * 4), round(cy + 3))
    pygame.draw.ellipse(surface, DUCK_ORANGE, wing)


def draw_text(surface, font, text, center, color=WHITE):
    image = font.render(text, True, color)
    surface.blit(image, image.get_rect(center=center))


def draw_game(surface, background, fonts, game, mouse_pos):
    surface.blit(background, (0, 0))
    for duck in game.ducks:
        draw_duck(surface, duck)

    big, small = fonts
    draw_text(surface, small, f"Wave {game.wave}   Score {game.score}", (110, 22), BLACK)
    for i in range(3):
        color = RED if i < game.lives else (150, 150, 150)
        pygame.draw.circle(surface, color, (WIDTH - 30 - i * 26, 22), 9)

    if game.game_over:
        overlay = pygame.Surface((WIDTH, HEIGHT), pygame.SRCALPHA)
        overlay.fill((0, 0, 0, 150))
        surface.blit(overlay, (0, 0))
        draw_text(surface, big, "GAME OVER", (WIDTH // 2, HEIGHT // 2 - 50))
        draw_text(surface, small, f"Score: {game.score}", (WIDTH // 2, HEIGHT // 2))
        draw_text(surface, small, "Click or press R to play again", (WIDTH // 2, HEIGHT // 2 + 40))
    else:
        if game.break_timer > 0:
            draw_text(surface, big, f"Wave {game.wave} cleared!", (WIDTH // 2, HEIGHT // 2 - 60))
        pygame.draw.circle(surface, RED, mouse_pos, 12, 2)
        pygame.draw.line(surface, RED, (mouse_pos[0] - 18, mouse_pos[1]), (mouse_pos[0] + 18, mouse_pos[1]), 2)
        pygame.draw.line(surface, RED, (mouse_pos[0], mouse_pos[1] - 18), (mouse_pos[0], mouse_pos[1] + 18), 2)


def main():
    pygame.init()
    screen = pygame.display.set_mode((WIDTH, HEIGHT))
    pygame.display.set_caption("Shooting Ducks")
    background = make_background()
    fonts = (pygame.font.Font(None, 56), pygame.font.Font(None, 34))
    clock = pygame.time.Clock()
    game = Game(seed=int(time.time()))

    running = True
    while running:
        dt = min(clock.tick(60) / 1000.0, 0.1)
        for event in pygame.event.get():
            if event.type == pygame.QUIT or (event.type == pygame.KEYDOWN and event.key == pygame.K_q):
                running = False
            elif game.game_over and (
                (event.type == pygame.KEYDOWN and event.key == pygame.K_r)
                or (event.type == pygame.MOUSEBUTTONDOWN and event.button == 1)
            ):
                game = Game(seed=int(time.time()))
            elif event.type == pygame.MOUSEBUTTONDOWN and event.button == 1:
                game.shoot(*from_pixels(*event.pos))
        game.update(dt)
        draw_game(screen, background, fonts, game, pygame.mouse.get_pos())
        pygame.display.flip()

    pygame.quit()


if __name__ == "__main__":
    main()
