/* Rules of Shooting Ducks: spawning, movement, hits, escapes and waves. */
#include "shooting_ducks.h"

#include <math.h>
#include <string.h>

#define BOB_AMPLITUDE 1.5
#define BOB_SPEED 2.0
#define TWO_PI 6.283185307179586

void rng_seed(Rng *rng, uint32_t seed) { rng->state = seed; }

double rng_next(Rng *rng) {
    rng->state = rng->state * 1664525u + 1013904223u;
    return rng->state / 4294967296.0;
}

int ducks_in_wave(int wave) { return 3 + 2 * wave; }

static double spawn_interval(int wave) {
    double interval = 2.0 - 0.15 * wave;
    return interval < 0.5 ? 0.5 : interval;
}

static void start_wave(Game *game, int wave) {
    game->wave = wave;
    game->ducks_to_spawn = ducks_in_wave(wave);
    game->spawn_timer = 0.0;
    game->break_timer = 0.0;
}

static void spawn_duck(Game *game) {
    Duck *duck = &game->ducks[game->duck_count++];
    int flies_right = rng_next(&game->rng) < 0.5;
    double speed = (5.0 + 1.5 * game->wave) * (0.8 + 0.4 * rng_next(&game->rng));
    duck->base_y = 5.0 + 22.0 * rng_next(&game->rng);
    duck->phase = TWO_PI * rng_next(&game->rng);
    duck->speed = flies_right ? speed : -speed;
    duck->x = flies_right ? -DUCK_HALF_WIDTH : FIELD_WIDTH + DUCK_HALF_WIDTH;
    duck->y = duck->base_y;
    duck->age = 0.0;
}

static int has_escaped(const Duck *duck) {
    return (duck->speed > 0 && duck->x - DUCK_HALF_WIDTH > FIELD_WIDTH) ||
           (duck->speed < 0 && duck->x + DUCK_HALF_WIDTH < 0);
}

void game_init(Game *game, uint32_t seed) {
    memset(game, 0, sizeof(*game));
    rng_seed(&game->rng, seed == 0 ? 1 : seed);
    game->lives = START_LIVES;
    start_wave(game, 1);
}

static void move_ducks(Game *game, double dt) {
    int kept = 0, escaped = 0;
    for (int i = 0; i < game->duck_count; ++i) {
        Duck duck = game->ducks[i];
        duck.x += duck.speed * dt;
        duck.age += dt;
        duck.y = duck.base_y + BOB_AMPLITUDE * sin(BOB_SPEED * duck.age + duck.phase);
        if (has_escaped(&duck)) {
            escaped++;
        } else {
            game->ducks[kept++] = duck;
        }
    }
    game->duck_count = kept;
    game->lives -= escaped;
    if (game->lives <= 0) {
        game->lives = 0;
        game->game_over = 1;
    }
}

void game_update(Game *game, double dt) {
    if (game->game_over) return;

    if (game->break_timer > 0) {
        game->break_timer -= dt;
        if (game->break_timer <= 0) start_wave(game, game->wave + 1);
        return;
    }

    move_ducks(game, dt);
    if (game->game_over) return;

    if (game->ducks_to_spawn > 0) {
        game->spawn_timer -= dt;
        if (game->spawn_timer <= 0 && game->duck_count < MAX_DUCKS) {
            spawn_duck(game);
            game->ducks_to_spawn--;
            game->spawn_timer = spawn_interval(game->wave);
        }
    }

    if (game->ducks_to_spawn == 0 && game->duck_count == 0) {
        game->break_timer = WAVE_BREAK_SECONDS;
    }
}

int game_shoot(Game *game, double x, double y) {
    if (game->game_over) return 0;
    for (int i = 0; i < game->duck_count; ++i) {
        Duck *duck = &game->ducks[i];
        if (fabs(x - duck->x) <= DUCK_HALF_WIDTH && fabs(y - duck->y) <= DUCK_HALF_HEIGHT) {
            memmove(duck, duck + 1, (size_t)(game->duck_count - i - 1) * sizeof(Duck));
            game->duck_count--;
            game->score += POINTS_PER_HIT;
            return 1;
        }
    }
    return 0;
}
