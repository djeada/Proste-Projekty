#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#define BOARD_SIZE 8
#define MINE_COUNT 10

typedef struct {
    int is_mine;
    int is_revealed;
    int is_flagged;
    int adjacent_mines;
} Cell;

typedef struct {
    Cell board[BOARD_SIZE][BOARD_SIZE];
    int revealed_count;
    int flagged_count;
    int game_over;
    int win;
} MinesweeperGame;

void game_init(MinesweeperGame *game);
void game_reveal(MinesweeperGame *game, int row, int col);
void game_flag(MinesweeperGame *game, int row, int col);
void game_print(const MinesweeperGame *game);

#endif // MINESWEEPER_H
