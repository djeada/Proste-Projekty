#ifndef ZOMBIE_APOCALYPSE_H
#define ZOMBIE_APOCALYPSE_H

#include <ncurses.h>

#define MAX_ZOMBIES 32
#define MAX_BULLETS 16
#define PLAYER_START_HEALTH 5

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
    int game_over;
} ZombieGame;

void zombie_game_init(ZombieGame *game, int max_x, int max_y);
void zombie_game_update(ZombieGame *game, int key);
void zombie_game_draw(const ZombieGame *game);

#endif // ZOMBIE_APOCALYPSE_H
