/* The rules of the game: no input, output or drawing here. */
#include "zombie_apocalypse.h"

#include <math.h>
#include <string.h>

#define PLAYER_RADIUS 0.5

static Vec2 vec_add(Vec2 a, Vec2 b) {
    Vec2 r = {a.x + b.x, a.y + b.y};
    return r;
}

static Vec2 vec_sub(Vec2 a, Vec2 b) {
    Vec2 r = {a.x - b.x, a.y - b.y};
    return r;
}

static Vec2 vec_scale(Vec2 v, double k) {
    Vec2 r = {v.x * k, v.y * k};
    return r;
}

static double vec_length(Vec2 v) {
    return sqrt(v.x * v.x + v.y * v.y);
}

static Vec2 vec_normalize(Vec2 v) {
    double len = vec_length(v);
    Vec2 zero = {0.0, 0.0};
    return len < 1e-9 ? zero : vec_scale(v, 1.0 / len);
}

static int vec_is_zero(Vec2 v) {
    return v.x == 0.0 && v.y == 0.0;
}

static double distance(Vec2 a, Vec2 b) {
    return vec_length(vec_sub(a, b));
}

static double clamp(double v, double lo, double hi) {
    return v < lo ? lo : (v > hi ? hi : v);
}

int game_wave_size(int wave) {
    return 5 + 3 * (wave - 1);
}

double game_zombie_speed(int wave) {
    double speed = 2.0 + 0.4 * (wave - 1);
    return speed > 5.0 ? 5.0 : speed;
}

static double spawn_interval(int wave) {
    double interval = 1.5 - 0.15 * (wave - 1);
    return interval < 0.5 ? 0.5 : interval;
}

/* A 32-bit linear congruential generator: the same seed gives the same game in every version. */
double game_random(Game *game) {
    game->rng = game->rng * 1664525u + 1013904223u;
    return game->rng / 4294967296.0;
}

void game_init(Game *game, uint32_t seed) {
    memset(game, 0, sizeof *game);
    game->player.x = WORLD_WIDTH / 2.0;
    game->player.y = WORLD_HEIGHT / 2.0;
    game->facing.x = 1.0;
    game->health = MAX_HEALTH;
    game->wave = 1;
    game->to_spawn = game_wave_size(1);
    game->pickup_timer = PICKUP_INTERVAL;
    game->rng = seed;
}

static void remove_zombie(Game *game, int i) {
    game->zombies[i] = game->zombies[--game->zombie_count];
}

static void remove_bullet(Game *game, int i) {
    game->bullets[i] = game->bullets[--game->bullet_count];
}

static void remove_pickup(Game *game, int i) {
    game->pickups[i] = game->pickups[--game->pickup_count];
}

static void move_player(Game *game, const Input *input, double dt) {
    Vec2 dir = vec_normalize(input->move);
    if (vec_is_zero(dir)) {
        return;
    }
    game->facing = dir;
    game->player = vec_add(game->player, vec_scale(dir, PLAYER_SPEED * dt));
    game->player.x = clamp(game->player.x, PLAYER_RADIUS, WORLD_WIDTH - PLAYER_RADIUS);
    game->player.y = clamp(game->player.y, PLAYER_RADIUS, WORLD_HEIGHT - PLAYER_RADIUS);
}

static void try_fire(Game *game, const Input *input, double dt) {
    game->shot_cooldown = fmax(0.0, game->shot_cooldown - dt);
    if (!input->fire || game->shot_cooldown > 0.0 || game->bullet_count >= MAX_BULLETS) {
        return;
    }
    Vec2 dir = vec_normalize(input->aim);
    if (vec_is_zero(dir)) {
        dir = game->facing;
    }
    Bullet bullet = {game->player, vec_scale(dir, BULLET_SPEED)};
    game->bullets[game->bullet_count++] = bullet;
    game->shot_cooldown = SHOT_COOLDOWN;
}

static void move_bullets(Game *game, double dt) {
    for (int i = 0; i < game->bullet_count;) {
        Bullet *b = &game->bullets[i];
        b->pos = vec_add(b->pos, vec_scale(b->vel, dt));
        if (b->pos.x < 0 || b->pos.x > WORLD_WIDTH || b->pos.y < 0 || b->pos.y > WORLD_HEIGHT) {
            remove_bullet(game, i);
        } else {
            i++;
        }
    }
}

static void spawn_zombie(Game *game) {
    double side = game_random(game) * 4.0;
    double t = game_random(game);
    Vec2 pos;
    if (side < 1.0) {
        pos.x = 0.0;
        pos.y = t * WORLD_HEIGHT;
    } else if (side < 2.0) {
        pos.x = WORLD_WIDTH;
        pos.y = t * WORLD_HEIGHT;
    } else if (side < 3.0) {
        pos.x = t * WORLD_WIDTH;
        pos.y = 0.0;
    } else {
        pos.x = t * WORLD_WIDTH;
        pos.y = WORLD_HEIGHT;
    }
    if (game->zombie_count < MAX_ZOMBIES) {
        game->zombies[game->zombie_count++] = pos;
    }
}

static void spawn_zombies(Game *game, double dt) {
    if (game->to_spawn == 0 || game->wave_delay > 0.0) {
        return;
    }
    game->spawn_timer -= dt;
    if (game->spawn_timer > 0.0) {
        return;
    }
    spawn_zombie(game);
    game->to_spawn--;
    game->spawn_timer = spawn_interval(game->wave);
}

static void move_zombies(Game *game, double dt) {
    double step = game_zombie_speed(game->wave) * dt;
    for (int i = 0; i < game->zombie_count; i++) {
        Vec2 toward = vec_normalize(vec_sub(game->player, game->zombies[i]));
        game->zombies[i] = vec_add(game->zombies[i], vec_scale(toward, step));
    }
}

static void resolve_bullet_hits(Game *game) {
    for (int b = 0; b < game->bullet_count;) {
        int hit = -1;
        for (int z = 0; z < game->zombie_count && hit < 0; z++) {
            if (distance(game->zombies[z], game->bullets[b].pos) < BULLET_HIT_DISTANCE) {
                hit = z;
            }
        }
        if (hit < 0) {
            b++;
        } else {
            remove_zombie(game, hit);
            remove_bullet(game, b);
            game->score += KILL_SCORE;
        }
    }
}

static void resolve_zombie_touches(Game *game) {
    for (int z = 0; z < game->zombie_count;) {
        if (distance(game->zombies[z], game->player) < TOUCH_DISTANCE) {
            game->health -= ZOMBIE_DAMAGE;
            remove_zombie(game, z);
        } else {
            z++;
        }
    }
    if (game->health <= 0) {
        game->health = 0;
        game->game_over = 1;
    }
}

static void resolve_pickups(Game *game) {
    for (int i = 0; i < game->pickup_count;) {
        if (distance(game->pickups[i], game->player) < TOUCH_DISTANCE) {
            game->health += PICKUP_HEAL;
            if (game->health > MAX_HEALTH) {
                game->health = MAX_HEALTH;
            }
            remove_pickup(game, i);
        } else {
            i++;
        }
    }
}

static void spawn_pickups(Game *game, double dt) {
    game->pickup_timer -= dt;
    if (game->pickup_timer > 0.0) {
        return;
    }
    game->pickup_timer = PICKUP_INTERVAL;
    if (game->pickup_count < MAX_PICKUPS) {
        Vec2 pos;
        pos.x = game_random(game) * WORLD_WIDTH;
        pos.y = game_random(game) * WORLD_HEIGHT;
        game->pickups[game->pickup_count++] = pos;
    }
}

static void update_waves(Game *game, double dt) {
    if (game->wave_delay > 0.0) {
        game->wave_delay -= dt;
        if (game->wave_delay <= 0.0) {
            game->wave_delay = 0.0;
            game->wave++;
            game->to_spawn = game_wave_size(game->wave);
            game->spawn_timer = 0.0;
        }
    } else if (game->to_spawn == 0 && game->zombie_count == 0) {
        game->wave_delay = WAVE_BREAK;
    }
}

void game_update(Game *game, const Input *input, double dt) {
    if (game->game_over) {
        return;
    }
    move_player(game, input, dt);
    try_fire(game, input, dt);
    move_bullets(game, dt);
    spawn_zombies(game, dt);
    move_zombies(game, dt);
    resolve_bullet_hits(game);
    resolve_zombie_touches(game);
    if (game->game_over) {
        return;
    }
    resolve_pickups(game);
    spawn_pickups(game, dt);
    update_waves(game, dt);
}
