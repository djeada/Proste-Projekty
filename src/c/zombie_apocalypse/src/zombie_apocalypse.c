#include "zombie_apocalypse.h"
#include <stdlib.h>
#include <string.h>

// --- Only new ncurses-based ZombieGame logic below ---

static void spawn_zombies(ZombieGame *game) {
    for (int i = 0; i < MAX_ZOMBIES; ++i) {
        game->zombies[i].x = rand() % game->max_x;
        game->zombies[i].y = rand() % game->max_y;
        game->zombies[i].alive = 1;
    }
    game->zombie_count = MAX_ZOMBIES;
}

void zombie_game_init(ZombieGame *game, int max_x, int max_y) {
    memset(game, 0, sizeof(ZombieGame));
    game->max_x = max_x;
    game->max_y = max_y;
    game->player.x = max_x / 2;
    game->player.y = max_y / 2;
    game->player.health = PLAYER_START_HEALTH;
    spawn_zombies(game);
    for (int i = 0; i < MAX_BULLETS; ++i) {
        game->bullets[i].active = 0;
    }
    game->game_over = 0;
}

static void move_player(Player *player, int key, int max_x, int max_y) {
    switch (key) {
        case KEY_UP:
        case 'w':
            if (player->y > 0) { player->y--; }
            break;
        case KEY_DOWN:
        case 's':
            if (player->y < max_y - 1) { player->y++; }
            break;
        case KEY_LEFT:
        case 'a':
            if (player->x > 0) { player->x--; }
            break;
        case KEY_RIGHT:
        case 'd':
            if (player->x < max_x - 1) { player->x++; }
            break;
        default:
            break;
    }
}

static void shoot_bullet(ZombieGame *game, Direction dir) {
    for (int i = 0; i < MAX_BULLETS; ++i) {
        if (!game->bullets[i].active) {
            game->bullets[i].x = game->player.x;
            game->bullets[i].y = game->player.y;
            game->bullets[i].dir = dir;
            game->bullets[i].active = 1;
            break;
        }
    }
}

static void update_bullets(ZombieGame *game) {
    for (int i = 0; i < MAX_BULLETS; ++i) {
        if (!game->bullets[i].active) { continue; }
        switch (game->bullets[i].dir) {
            case DIR_UP:    game->bullets[i].y--; break;
            case DIR_DOWN:  game->bullets[i].y++; break;
            case DIR_LEFT:  game->bullets[i].x--; break;
            case DIR_RIGHT: game->bullets[i].x++; break;
            default: break;
        }
        if (game->bullets[i].x < 0 || game->bullets[i].x >= game->max_x ||
            game->bullets[i].y < 0 || game->bullets[i].y >= game->max_y) {
            game->bullets[i].active = 0;
        }
    }
}

static void update_zombies(ZombieGame *game) {
    for (int i = 0; i < MAX_ZOMBIES; ++i) {
        if (!game->zombies[i].alive) { continue; }
        int dx = game->player.x - game->zombies[i].x;
        int dy = game->player.y - game->zombies[i].y;
        if (abs(dx) > abs(dy)) {
            game->zombies[i].x += (dx > 0) ? 1 : -1;
        } else if (dy != 0) {
            game->zombies[i].y += (dy > 0) ? 1 : -1;
        }
    }
}

static void handle_collisions(ZombieGame *game) {
    // Bullets vs zombies
    for (int i = 0; i < MAX_BULLETS; ++i) {
        if (!game->bullets[i].active) { continue; }
        for (int j = 0; j < MAX_ZOMBIES; ++j) {
            if (game->zombies[j].alive &&
                game->bullets[i].x == game->zombies[j].x &&
                game->bullets[i].y == game->zombies[j].y) {
                game->zombies[j].alive = 0;
                game->bullets[i].active = 0;
                break;
            }
        }
    }
    // Zombies vs player
    for (int i = 0; i < MAX_ZOMBIES; ++i) {
        if (game->zombies[i].alive &&
            game->zombies[i].x == game->player.x &&
            game->zombies[i].y == game->player.y) {
            game->player.health--;
            game->zombies[i].alive = 0;
            if (game->player.health <= 0) {
                game->game_over = 1;
            }
        }
    }
}

void zombie_game_update(ZombieGame *game, int key) {
    if (game->game_over) { return; }
    if (key == ' ') {
        // Shoot in last moved direction (for simplicity, always right)
        shoot_bullet(game, DIR_RIGHT);
    } else {
        move_player(&game->player, key, game->max_x, game->max_y);
    }
    handle_collisions(game);
    update_bullets(game);
    update_zombies(game);
    // Check if all zombies are dead
    int alive = 0;
    for (int i = 0; i < MAX_ZOMBIES; ++i) {
        if (game->zombies[i].alive) { alive = 1; }
    }
    if (!alive) { game->game_over = 1; }
}

void zombie_game_draw(const ZombieGame *game) {
    // Draw player
    mvprintw(game->player.y, game->player.x, "@");
    // Draw zombies
    for (int i = 0; i < MAX_ZOMBIES; ++i) {
        if (game->zombies[i].alive) {
            mvprintw(game->zombies[i].y, game->zombies[i].x, "Z");
        }
    }
    // Draw bullets
    for (int i = 0; i < MAX_BULLETS; ++i) {
        if (game->bullets[i].active) {
            mvprintw(game->bullets[i].y, game->bullets[i].x, "-");
        }
    }
    // Draw health
    mvprintw(0, 0, "Health: %d", game->player.health);
}
