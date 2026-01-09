#ifndef TIC_TAC_TOE_H
#define TIC_TAC_TOE_H

#define BOARD_SIZE 3
#define PLAYER_X 'X'
#define PLAYER_O 'O'
#define EMPTY ' '

typedef struct {
    char cells[BOARD_SIZE][BOARD_SIZE];
    char current_player;
    int game_over;
    char winner;
} Game;

void game_init(Game *game);
int make_move(Game *game, int row, int col);
int check_winner(const Game *game);
int is_board_full(const Game *game);
void switch_player(Game *game);
void print_board(const Game *game);
int ai_make_move(Game *game);

#endif // TIC_TAC_TOE_H
