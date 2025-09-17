#include "shooting_ducks.h"
#include <stdlib.h>
#include <string.h>

static void spawn_ducks(DuckGame *game) {
    int target = BASE_DUCKS + (game->level - 1) * DUCKS_PER_LEVEL;
    if (target > MAX_DUCKS) target = MAX_DUCKS;
    for (int i = 0; i < MAX_DUCKS; ++i) {
        game->ducks[i].x = rand() % game->max_x;
        game->ducks[i].y = rand() % game->max_y;
        game->ducks[i].alive = (i < target) ? 1 : 0;
        game->ducks[i].dir = DIR_RIGHT;
    }
    game->duck_count = target;
}

void duck_game_init(DuckGame *game, int max_x, int max_y) {
    memset(game, 0, sizeof(DuckGame));
    game->max_x = max_x;
    game->max_y = max_y;
    game->crosshair.x = max_x / 2;
    game->crosshair.y = max_y / 2;
    game->crosshair.health = CROSSHAIR_START_HEALTH;
    game->level = 1;
    spawn_ducks(game);
    for (int i = 0; i < MAX_SHOTS; ++i) game->shots[i].active = 0;
    game->game_over = 0;
    game->wave_cleared = 0;
    game->score = 0;
    game->tick = 0;
    game->duck_move_period = 3;
    game->paused = 0;
}

static void move_crosshair(Crosshair *c, int key, int max_x, int max_y) {
    switch (key) {
        case KEY_UP:
        case 'w': if (c->y > 0) c->y--; break;
        case KEY_DOWN:
        case 's': if (c->y < max_y - 1) c->y++; break;
        case KEY_LEFT:
        case 'a': if (c->x > 0) c->x--; break;
        case KEY_RIGHT:
        case 'd': if (c->x < max_x - 1) c->x++; break;
        default: break;
    }
}

static void fire_shot(DuckGame *game) {
    for (int i = 0; i < MAX_SHOTS; ++i) {
        if (!game->shots[i].active) {
            game->shots[i].x = game->crosshair.x;
            game->shots[i].y = game->crosshair.y;
            game->shots[i].active = 1;
            break;
        }
    }
}

static void update_shots(DuckGame *game) {
    // Shots are instant-hit at crosshair; deactivate after processing
    for (int i = 0; i < MAX_SHOTS; ++i) {
        if (!game->shots[i].active) continue;
        for (int j = 0; j < MAX_DUCKS; ++j) {
            if (game->ducks[j].alive && game->ducks[j].x == game->shots[i].x && game->ducks[j].y == game->shots[i].y) {
                game->ducks[j].alive = 0;
                game->score += 10;
            }
        }
        game->shots[i].active = 0;
    }
}

static void update_ducks(DuckGame *game) {
    if (game->duck_move_period <= 0) game->duck_move_period = 1;
    if ((game->tick % game->duck_move_period) != 0) return;
    for (int i = 0; i < MAX_DUCKS; ++i) {
        if (!game->ducks[i].alive) continue;
        if (game->ducks[i].dir == DIR_RIGHT) game->ducks[i].x++;
        else if (game->ducks[i].dir == DIR_LEFT) game->ducks[i].x--;
        if (game->ducks[i].x < 0) game->ducks[i].x = game->max_x - 1;
        if (game->ducks[i].x >= game->max_x) game->ducks[i].x = 0;
    }
}

void duck_game_update(DuckGame *game, int key) {
    if (game->game_over) return;

    if (key == 'r') { duck_game_init(game, game->max_x, game->max_y); return; }
    if (key == 'p') { game->paused = !game->paused; }

    if (game->wave_cleared) {
        if (key == 'n') duck_game_next_level(game);
        return;
    }

    if (game->paused) return;

    switch (key) {
        case KEY_UP: case 'w':
        case KEY_DOWN: case 's':
        case KEY_LEFT: case 'a':
        case KEY_RIGHT: case 'd':
            move_crosshair(&game->crosshair, key, game->max_x, game->max_y);
            break;
        case ' ': fire_shot(game); break;
        default: break;
    }

    update_shots(game);
    update_ducks(game);
    game->tick++;

    int any_alive = 0;
    for (int i = 0; i < MAX_DUCKS; ++i) if (game->ducks[i].alive) { any_alive = 1; break; }
    if (!any_alive) { game->wave_cleared = 1; game->score += 100; }
}

void duck_game_draw(const DuckGame *game) {
    // crosshair
    mvprintw(game->crosshair.y, game->crosshair.x, "+");
    // ducks
    for (int i = 0; i < MAX_DUCKS; ++i) if (game->ducks[i].alive) mvprintw(game->ducks[i].y, game->ducks[i].x, "D");
    // HUD
    int alive = 0; for (int i = 0; i < MAX_DUCKS; ++i) if (game->ducks[i].alive) alive++;
    mvprintw(0, 0, "HP:%d  Lvl:%d  Score:%d  Ducks:%d/%d  Speed:%d", game->crosshair.health, game->level, game->score, alive, game->duck_count, game->duck_move_period);
    mvprintw(1, 0, "Move: WASD/Arrows  Shoot: Space  Pause: p  Restart: r");
    if (game->wave_cleared) mvprintw(2, 0, "Wave cleared! Press 'n' for next level.");
    if (game->paused && !game->wave_cleared) mvprintw(2, 0, "Paused. Press 'p' to resume.");
}

void duck_game_next_level(DuckGame *game) {
    if (!game->wave_cleared) return;
    game->level++;
    if (game->duck_move_period > 1) game->duck_move_period--;
    spawn_ducks(game);
    game->crosshair.x = game->max_x / 2;
    game->crosshair.y = game->max_y / 2;
    for (int i = 0; i < MAX_SHOTS; ++i) game->shots[i].active = 0;
    game->wave_cleared = 0;
}
