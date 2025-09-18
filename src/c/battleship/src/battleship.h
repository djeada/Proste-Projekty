#ifndef BATTLESHIP_H
#define BATTLESHIP_H

#include <stdio.h>

#define BOARD_SIZE 10
#define MAX_SHIPS 5

typedef enum { PHASE_PLACEMENT, PHASE_BATTLE, PHASE_GAMEOVER, PHASE_QUIT } Phase;

typedef struct { int x, y; } Point;

typedef struct {
    Point pos; // top-left of ship
    int length;
    int horizontal; // 1 = horizontal, 0 = vertical
    int hits;
    int placed; // 1 when placed
} Ship;

typedef struct {
    int has_ship; // 1 if a ship occupies this tile
    int hit;      // 1 if fired upon
} Cell;

typedef struct {
    Cell grid[BOARD_SIZE][BOARD_SIZE];
    Ship ships[MAX_SHIPS];
} Board;

typedef struct {
    Board player;
    Board enemy;
    Phase phase;
    int cursor_x, cursor_y;
    int current_ship; // index during placement
    int enemy_ships_remaining;
    int player_ships_remaining;
    int max_x, max_y;
    char status[128];
} BattleGame;

void battleship_init(BattleGame *game, int max_x, int max_y);
int place_ship(Board *board, int ship_index, int x, int y, int horizontal);
int fire_at(Board *board, int x, int y);
int all_ships_placed(const Board *board);
int is_defeated(const Board *board);
void battleship_update(BattleGame *game, int key);
void battleship_draw_text(const BattleGame *game, FILE *out);

#endif // BATTLESHIP_H
