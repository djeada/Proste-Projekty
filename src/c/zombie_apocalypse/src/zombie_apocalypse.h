/* Rules of Zombie Apocalypse. The world is WORLD_WIDTH x WORLD_HEIGHT units, one unit per terminal cell. */
#ifndef ZOMBIE_APOCALYPSE_H
#define ZOMBIE_APOCALYPSE_H

#include <stdint.h>

#define WORLD_WIDTH 60
#define WORLD_HEIGHT 20
#define MAX_ZOMBIES 64
#define MAX_BULLETS 64
#define MAX_PICKUPS 2

#define MAX_HEALTH 100
#define PLAYER_SPEED 7.0
#define TOUCH_DISTANCE 1.0 /* player and zombies have radius 0.5 */
#define ZOMBIE_DAMAGE 10
#define KILL_SCORE 10
#define BULLET_SPEED 18.0
#define BULLET_HIT_DISTANCE 0.6
#define SHOT_COOLDOWN 0.25
#define PICKUP_HEAL 25
#define PICKUP_INTERVAL 12.0
#define WAVE_BREAK 3.0

typedef struct {
    double x, y;
} Vec2;

typedef struct {
    Vec2 pos;
    Vec2 vel;
} Bullet;

/* What the player asks for in one step. move and aim are directions; zero means "no direction". */
typedef struct {
    Vec2 move;
    Vec2 aim;
    int fire;
} Input;

typedef struct {
    Vec2 player;
    Vec2 facing; /* direction of the last movement, used when aim is zero */
    int health;
    Vec2 zombies[MAX_ZOMBIES];
    int zombie_count;
    Bullet bullets[MAX_BULLETS];
    int bullet_count;
    Vec2 pickups[MAX_PICKUPS];
    int pickup_count;
    int wave;
    int to_spawn; /* zombies of this wave that are not on the field yet */
    int score;
    int game_over;
    double spawn_timer;
    double wave_delay; /* seconds left before the next wave; 0 while a wave is running */
    double pickup_timer;
    double shot_cooldown;
    uint32_t rng;
} Game;

void game_init(Game *game, uint32_t seed);
void game_update(Game *game, const Input *input, double dt);
double game_random(Game *game); /* next number in [0, 1) */
int game_wave_size(int wave);
double game_zombie_speed(int wave);

#endif /* ZOMBIE_APOCALYPSE_H */
