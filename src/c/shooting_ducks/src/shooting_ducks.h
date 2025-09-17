#ifndef SHOOTING_DUCKS_H
#define SHOOTING_DUCKS_H

#include <ncurses.h>

#define MAX_DUCKS 32
#define MAX_SHOTS 16
#define CROSSHAIR_START_HEALTH 3
#define BASE_DUCKS 6
#define DUCKS_PER_LEVEL 4

// Directions for duck drift
typedef enum { DIR_LEFT, DIR_RIGHT, DIR_UP, DIR_DOWN, DIR_NONE } Direction;

typedef struct {
    int x, y;
    int health;
} Crosshair;

typedef struct {
    int x, y;
    int alive;
    Direction dir;
} Duck;

typedef struct {
    int x, y;
    int active;
} Shot;

typedef struct {
    Crosshair crosshair;
    Duck ducks[MAX_DUCKS];
    int duck_count;
    Shot shots[MAX_SHOTS];
    int max_x, max_y;
    int game_over;
    int wave_cleared;
    int level;
    int score;
    int tick;
    int duck_move_period;
    int paused;
} DuckGame;

void duck_game_init(DuckGame *game, int max_x, int max_y);
void duck_game_update(DuckGame *game, int key);
void duck_game_draw(const DuckGame *game);
void duck_game_next_level(DuckGame *game);

#endif // SHOOTING_DUCKS_H
