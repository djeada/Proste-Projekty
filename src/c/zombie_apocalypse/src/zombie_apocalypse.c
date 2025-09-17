#include "zombie_apocalypse.h"
#include <stdlib.h>
#include <string.h>

// --- Only new ncurses-based ZombieGame logic below ---

static void spawn_zombies(ZombieGame *game) {
    int target = BASE_ZOMBIES + (game->level - 1) * ZOMBIES_PER_LEVEL;
    if (target > MAX_ZOMBIES) target = MAX_ZOMBIES;
    for (int i = 0; i < MAX_ZOMBIES; ++i) {
        game->zombies[i].x = rand() % game->max_x;
        game->zombies[i].y = rand() % game->max_y;
        game->zombies[i].alive = (i < target) ? 1 : 0;
    }
    game->zombie_count = target;
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
    game->wave_cleared = 0;
    game->level = 1;
    game->score = 0;
    game->tick = 0;
    game->zombie_move_period = 3; // zombies move every 3 ticks initially
    game->last_dir = DIR_RIGHT;
    game->paused = 0;
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
    if (game->zombie_move_period <= 0) game->zombie_move_period = 1;
    if ((game->tick % game->zombie_move_period) != 0) return;
    for (int i = 0; i < MAX_ZOMBIES; ++i) {
        if (!game->zombies[i].alive) { continue; }
        int dx = game->player.x - game->zombies[i].x;
        int dy = game->player.y - game->zombies[i].y;
        if (abs(dx) > abs(dy)) {
            game->zombies[i].x += (dx > 0) ? 1 : -1;
        } else if (dy != 0) {
            game->zombies[i].y += (dy > 0) ? 1 : -1;
        }
        if (game->zombies[i].x < 0) game->zombies[i].x = 0;
        if (game->zombies[i].x >= game->max_x) game->zombies[i].x = game->max_x - 1;
        if (game->zombies[i].y < 0) game->zombies[i].y = 0;
        if (game->zombies[i].y >= game->max_y) game->zombies[i].y = game->max_y - 1;
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
                game->score += 10;
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

    // Pause 'p', restart 'r' (resets entire game), proceed to next level 'n' when cleared
    if (key == 'r') {
        zombie_game_init(game, game->max_x, game->max_y);
        return;
    }
    if (key == 'p') {
        game->paused = !game->paused;
    }

    if (game->wave_cleared) {
        if (key == 'n') {
            zombie_game_next_level(game);
        }
        return; // wait until user continues
    }

    if (game->paused) {
        return;
    }

    // Movement and shooting
    switch (key) {
        case KEY_UP: case 'w': game->last_dir = DIR_UP;  move_player(&game->player, key, game->max_x, game->max_y); break;
        case KEY_DOWN: case 's': game->last_dir = DIR_DOWN; move_player(&game->player, key, game->max_x, game->max_y); break;
        case KEY_LEFT: case 'a': game->last_dir = DIR_LEFT; move_player(&game->player, key, game->max_x, game->max_y); break;
        case KEY_RIGHT: case 'd': game->last_dir = DIR_RIGHT; move_player(&game->player, key, game->max_x, game->max_y); break;
        case ' ':
            shoot_bullet(game, game->last_dir);
            break;
        default: break;
    }

    handle_collisions(game);
    update_bullets(game);
    update_zombies(game);
    game->tick++;

    // Check if all zombies are dead
    int any_alive = 0;
    for (int i = 0; i < MAX_ZOMBIES; ++i) {
        if (game->zombies[i].alive) { any_alive = 1; break; }
    }
    if (!any_alive) {
        game->wave_cleared = 1;
        game->score += 100; // wave bonus
    }
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
            char ch = '-';
            if (game->bullets[i].dir == DIR_UP || game->bullets[i].dir == DIR_DOWN) ch = '|';
            mvprintw(game->bullets[i].y, game->bullets[i].x, "%c", ch);
        }
    }

    // HUD
    int alive = 0;
    for (int i = 0; i < MAX_ZOMBIES; ++i) {
        if (game->zombies[i].alive)
            alive++;
    }
    mvprintw(0, 0, "HP:%d  Lvl:%d  Score:%d  Zombies:%d/%d  Speed:%d",
             game->player.health, game->level, game->score, alive, game->zombie_count, game->zombie_move_period);
    mvprintw(1, 0, "Move: WASD/Arrows  Shoot: Space  Pause: p  Restart: r");
    if (game->wave_cleared) {
        mvprintw(2, 0, "Wave cleared! Press 'n' for next level.");
    }
    if (game->game_over) {
        mvprintw(2, 0, "You died! Press 'r' to restart.");
    }
    if (game->paused && !game->game_over && !game->wave_cleared) {
        mvprintw(2, 0, "Paused. Press 'p' to resume.");
    }
}

void zombie_game_next_level(ZombieGame *game) {
    if (!game->wave_cleared) return;
    game->level++;
    // Increase difficulty: faster zombies and more bullets cap usage via period
    if (game->zombie_move_period > 1) game->zombie_move_period--; // speed up
    // Respawn zombies and reset positions minimally
    spawn_zombies(game);
    // Reset player to center but keep health and score
    game->player.x = game->max_x / 2;
    game->player.y = game->max_y / 2;
    // Clear bullets
    for (int i = 0; i < MAX_BULLETS; ++i) game->bullets[i].active = 0;
    game->wave_cleared = 0;
}
