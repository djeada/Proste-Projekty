#ifndef ZOMBIE_APOCALYPSE_H
#define ZOMBIE_APOCALYPSE_H

#include <ncurses.h>

#define MAX_ZOMBIES 32
#define MAX_BULLETS 16
#define PLAYER_START_HEALTH 5
// Progressive spawn settings
#define BASE_ZOMBIES 6
#define ZOMBIES_PER_LEVEL 4

// Directions
typedef enum { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT, DIR_NONE } Direction;

typedef struct {
    int x, y;
    int health;
} Player;

typedef struct {
    int x, y;
    int alive;
} Zombie;

typedef struct {
    int x, y;
    Direction dir;
    int active;
} Bullet;

typedef struct {
    Player player;
    Zombie zombies[MAX_ZOMBIES];
    int zombie_count;
    Bullet bullets[MAX_BULLETS];
    int max_x, max_y;
    int game_over; // 1 when player dies
    int wave_cleared; // 1 when all zombies are dead
    int level; // starts at 1
    int score;
    int tick; // global tick for timing
    int zombie_move_period; // lower = faster zombies
    Direction last_dir; // last movement direction (for shooting)
    int paused; // 1 when paused
} ZombieGame;

void zombie_game_init(ZombieGame *game, int max_x, int max_y);
void zombie_game_update(ZombieGame *game, int key);
void zombie_game_draw(const ZombieGame *game);
void zombie_game_next_level(ZombieGame *game);

#endif // ZOMBIE_APOCALYPSE_H
