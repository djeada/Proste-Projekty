/* Minesweeper rules: mines, numbers, reveal with flood fill, flags. No input or output. */
#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#define MAX_ROWS 16
#define MAX_COLS 30

typedef enum { PLAYING, WON, LOST } GameState;

typedef struct {
    int rows;
    int cols;
    int mine_total;
    int mines_placed;
    int mine[MAX_ROWS][MAX_COLS];
    int neighbors[MAX_ROWS][MAX_COLS];
    int revealed[MAX_ROWS][MAX_COLS];
    int flagged[MAX_ROWS][MAX_COLS];
    int revealed_count;
    int flag_count;
    unsigned int rng;
    GameState state;
} Game;

/* Starts a game with no mines yet; the seed makes the mine layout repeatable. */
void game_start(Game *game, int rows, int cols, int mines, unsigned int seed);

/* Reveals a cell. The first reveal places the mines, never on that cell or its neighbors. */
void game_reveal(Game *game, int row, int col);

/* Puts a flag on a hidden cell or removes it. */
void game_toggle_flag(Game *game, int row, int col);

/* Number of mines minus number of flags (can be negative). */
int game_mines_left(const Game *game);

#endif /* MINESWEEPER_H */
