/* Battleship rules: boards, ship placement, shooting and the computer player. */
#ifndef BATTLESHIP_H
#define BATTLESHIP_H

#define BOARD_SIZE 10
#define SHIP_COUNT 5
#define NO_SHIP (-1)
#define MAX_TARGETS 100

typedef enum {
    SHOT_INVALID,
    SHOT_REPEAT, /* this cell was already shot */
    SHOT_MISS,
    SHOT_HIT,
    SHOT_SUNK
} ShotResult;

typedef struct {
    int x;
    int y;
} Point;

/* A small xorshift random source, so tests can use a fixed seed. */
typedef struct {
    unsigned int state;
} Rng;

typedef struct {
    int length;
    int hits;
    int placed;
} Ship;

typedef struct {
    Ship ships[SHIP_COUNT];
    int ship_at[BOARD_SIZE][BOARD_SIZE]; /* index of the ship on a cell, or NO_SHIP */
    int shot[BOARD_SIZE][BOARD_SIZE];
} Board;

typedef struct {
    Point targets[MAX_TARGETS]; /* stack of cells next to hits */
    int count;
} Computer;

void rng_seed(Rng *rng, unsigned int seed);
int rng_below(Rng *rng, int n);

void board_reset(Board *board);
int board_can_place(const Board *board, int ship, int x, int y, int horizontal);
int board_place(Board *board, int ship, int x, int y, int horizontal);
void board_place_random(Board *board, Rng *rng);
int board_fleet_placed(const Board *board);
ShotResult board_fire(Board *board, int x, int y);
int board_all_sunk(const Board *board);

void computer_reset(Computer *computer);
Point computer_choose(Computer *computer, const Board *target, Rng *rng);
void computer_report(Computer *computer, const Board *target, Point at, ShotResult result);

#endif
