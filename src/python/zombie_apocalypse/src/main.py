"""The pygame window of Zombie Apocalypse: draws the game and reads the keyboard and mouse."""

import math
import random

import pygame

from zombie_apocalypse import MAX_HEALTH, WORLD_HEIGHT, WORLD_WIDTH, Game, Input, new_game, update

SCALE = 20  # pixels per world unit
HUD_HEIGHT = 36
FPS = 60
BACKGROUND = (34, 44, 34)
GRID = (44, 56, 44)
HUD_COLOR = (20, 24, 20)
WHITE = (240, 240, 240)
BAR_BACK = (70, 70, 70)
BAR_COLOR = (200, 60, 60)
PLAYER_COLOR = (70, 150, 240)
PLAYER_OUTLINE = (20, 50, 100)
GUN_COLOR = (150, 150, 160)
ZOMBIE_COLOR = (90, 170, 70)
ZOMBIE_OUTLINE = (30, 80, 25)
EYE_COLOR = (235, 240, 200)
BULLET_COLOR = (255, 240, 60)
PICKUP_COLOR = (250, 250, 250)
CROSS_COLOR = (220, 40, 40)
ZOMBIE_RADIUS = 14
PLAYER_RADIUS = 10


def to_screen(x, y):
    return int(x * SCALE), int(HUD_HEIGHT + y * SCALE)


def unit(vector, fallback):
    length = math.hypot(*vector)
    if length < 1e-6:
        return fallback
    return vector[0] / length, vector[1] / length


def read_input(game: Game, clicked: bool) -> Input:
    keys = pygame.key.get_pressed()
    right = keys[pygame.K_d] or keys[pygame.K_RIGHT]
    left = keys[pygame.K_a] or keys[pygame.K_LEFT]
    down = keys[pygame.K_s] or keys[pygame.K_DOWN]
    up = keys[pygame.K_w] or keys[pygame.K_UP]
    mouse_x, mouse_y = pygame.mouse.get_pos()
    return Input(
        move_x=float(right) - float(left),
        move_y=float(down) - float(up),
        aim_x=mouse_x / SCALE - game.player[0],
        aim_y=(mouse_y - HUD_HEIGHT) / SCALE - game.player[1],
        fire=clicked,
    )


def draw_ground(screen) -> None:
    screen.fill(BACKGROUND)
    width, height = screen.get_size()
    for x in range(0, width, SCALE):
        pygame.draw.line(screen, GRID, (x, HUD_HEIGHT), (x, height))
    for y in range(HUD_HEIGHT, height, SCALE):
        pygame.draw.line(screen, GRID, (0, y), (width, y))


def draw_zombie(screen, center, target) -> None:
    dx, dy = unit((target[0] - center[0], target[1] - center[1]), (1.0, 0.0))
    pygame.draw.circle(screen, ZOMBIE_COLOR, center, ZOMBIE_RADIUS)
    pygame.draw.circle(screen, ZOMBIE_OUTLINE, center, ZOMBIE_RADIUS, 3)
    for side in (-1, 1):  # two eyes, looking towards the player
        ex = center[0] + dx * 5 - dy * 4.5 * side
        ey = center[1] + dy * 5 + dx * 4.5 * side
        pygame.draw.circle(screen, EYE_COLOR, (round(ex), round(ey)), 4)
        pygame.draw.circle(screen, (20, 20, 20), (round(ex + dx * 1.5), round(ey + dy * 1.5)), 2)


def draw_player(screen, center, facing, aim) -> None:
    dx, dy = unit((aim[0] - center[0], aim[1] - center[1]), facing)
    barrel_end = (round(center[0] + dx * 24), round(center[1] + dy * 24))
    pygame.draw.line(screen, GUN_COLOR, center, barrel_end, 7)
    pygame.draw.circle(screen, PLAYER_COLOR, center, PLAYER_RADIUS)
    pygame.draw.circle(screen, PLAYER_OUTLINE, center, PLAYER_RADIUS, 3)


def draw_pickup(screen, center) -> None:
    cx, cy = center
    pygame.draw.rect(screen, PICKUP_COLOR, (cx - 8, cy - 8, 16, 16))
    pygame.draw.rect(screen, CROSS_COLOR, (cx - 2, cy - 6, 4, 12))
    pygame.draw.rect(screen, CROSS_COLOR, (cx - 6, cy - 2, 12, 4))


def draw_hud(screen, small_font, game: Game) -> None:
    width = screen.get_width()
    pygame.draw.rect(screen, HUD_COLOR, (0, 0, width, HUD_HEIGHT))
    info = small_font.render(f"Wave {game.wave}   Score {game.score}", True, WHITE)
    screen.blit(info, (10, 10))
    bar_x = width - 170
    label = small_font.render("Health", True, WHITE)
    screen.blit(label, (bar_x - label.get_width() - 10, 10))
    pygame.draw.rect(screen, BAR_BACK, (bar_x, 12, 160, 12))
    pygame.draw.rect(screen, BAR_COLOR, (bar_x, 12, 160 * game.health // MAX_HEALTH, 12))


def draw_game(screen, small_font, big_font, game: Game, flash: float, aim) -> None:
    draw_ground(screen)
    for pickup in game.pickups:
        draw_pickup(screen, to_screen(*pickup))
    player = to_screen(*game.player)
    for zombie in game.zombies:
        draw_zombie(screen, to_screen(*zombie), player)
    for bullet in game.bullets:
        pygame.draw.circle(screen, BULLET_COLOR, to_screen(*bullet.pos), 4)
    draw_player(screen, player, game.facing, aim)
    draw_hud(screen, small_font, game)

    if flash > 0:
        overlay = pygame.Surface(screen.get_size())
        overlay.set_alpha(int(120 * flash / 0.3))
        overlay.fill((200, 0, 0))
        screen.blit(overlay, (0, 0))

    if game.game_over:
        message = "GAME OVER - press R to play again"
    elif game.wave_delay > 0:
        message = f"Wave {game.wave} cleared!"
    else:
        return
    text = big_font.render(message, True, WHITE)
    screen.blit(text, text.get_rect(center=(screen.get_width() // 2, screen.get_height() // 2)))


def main() -> None:
    pygame.init()
    screen = pygame.display.set_mode((WORLD_WIDTH * SCALE, HUD_HEIGHT + WORLD_HEIGHT * SCALE))
    pygame.display.set_caption("Zombie Apocalypse")
    small_font = pygame.font.Font(None, 28)
    big_font = pygame.font.Font(None, 56)
    clock = pygame.time.Clock()

    game = new_game(random.randrange(2**32))
    last_health = game.health
    flash = 0.0
    running = True
    while running:
        dt = min(clock.tick(FPS) / 1000, 0.05)
        clicked = False
        for event in pygame.event.get():
            if event.type == pygame.QUIT or (event.type == pygame.KEYDOWN and event.key == pygame.K_ESCAPE):
                running = False
            elif event.type == pygame.KEYDOWN and event.key == pygame.K_r and game.game_over:
                game = new_game(random.randrange(2**32))
            elif event.type == pygame.MOUSEBUTTONDOWN and event.button == 1:
                clicked = True

        update(game, read_input(game, clicked), dt)
        if game.health < last_health:
            flash = 0.3
        last_health = game.health
        flash = max(0.0, flash - dt)
        draw_game(screen, small_font, big_font, game, flash, pygame.mouse.get_pos())
        pygame.display.flip()
    pygame.quit()


if __name__ == "__main__":
    main()
