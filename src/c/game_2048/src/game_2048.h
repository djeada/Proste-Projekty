/* Rules of 2048: sliding, merging, spawning and win/lose checks. No input or output here. */
#ifndef GAME_2048_H
#define GAME_2048_H

#define G2048_SIZE 4

/* Returns a random integer in [0, n). The game uses it, tests replace it. */
typedef int (*RandomBelow)(int n);

typedef enum { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT } Direction;
typedef enum { STATUS_PLAYING, STATUS_WON, STATUS_LOST } Status;

typedef struct {
    int cells[G2048_SIZE][G2048_SIZE]; /* 0 means an empty cell */
    int score;
    int keep_playing; /* set when the player chooses to go on after 2048 */
} Game;

/* Merges one line towards its first element. Returns the points gained. */
int slide_row(int line[G2048_SIZE]);

/* Slides every line of the board in the given direction. Returns the points gained. */
int board_slide(int cells[G2048_SIZE][G2048_SIZE], Direction dir);

/* Starts a new game with two tiles on the board. */
void game_start(Game *g, RandomBelow random_below);

/* Makes a move. Returns 1 if the board changed (then a new tile appears), 0 otherwise. */
int game_move(Game *g, Direction dir, RandomBelow random_below);

/* Places a 2 (90%) or a 4 (10%) on a random empty cell. Returns 0 if the board is full. */
int game_spawn_tile(Game *g, RandomBelow random_below);

int game_can_move(const Game *g);
void game_keep_playing(Game *g);
Status game_status(const Game *g);

#endif /* GAME_2048_H */
