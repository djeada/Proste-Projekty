/* Rules of Shooting Ducks: spawning, movement, hits, escapes and waves. */
#ifndef SHOOTING_DUCKS_H
#define SHOOTING_DUCKS_H

#include <stdint.h>

#define FIELD_WIDTH 100.0
#define FIELD_HEIGHT 40.0
#define DUCK_HALF_WIDTH 5.0
#define DUCK_HALF_HEIGHT 3.0
#define START_LIVES 3
#define POINTS_PER_HIT 10
#define WAVE_BREAK_SECONDS 2.0
#define MAX_DUCKS 64

/* Linear congruential generator: the same numbers in every language version. */
typedef struct {
    uint32_t state;
} Rng;

typedef struct {
    double x, y;   /* centre of the duck */
    double base_y; /* height the duck bobs around */
    double speed;  /* positive flies right, negative flies left */
    double age;    /* seconds since the duck appeared */
    double phase;  /* bobbing phase */
} Duck;

typedef struct {
    Duck ducks[MAX_DUCKS];
    int duck_count;
    Rng rng;
    int wave;
    int score;
    int lives;
    int ducks_to_spawn;
    double spawn_timer;
    double break_timer; /* above zero while the "wave cleared" pause runs */
    int game_over;
} Game;

void rng_seed(Rng *rng, uint32_t seed);
double rng_next(Rng *rng); /* a number in [0, 1) */

void game_init(Game *game, uint32_t seed);
void game_update(Game *game, double dt);
int game_shoot(Game *game, double x, double y); /* returns 1 on a hit */
int ducks_in_wave(int wave);

#endif
