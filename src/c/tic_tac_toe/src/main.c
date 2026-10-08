/* Terminal tic-tac-toe with ncurses: move with the arrows or press 1-9, Enter places a mark. */
#include <ncurses.h>
#include <stdbool.h>

#include "tic_tac_toe.h"

typedef struct {
    Board board;
    Mark turn;
    int cursor;
    bool vs_computer;
    int x_wins;
    int o_wins;
    int draws;
} Game;

static void new_round(Game *game)
{
    game->board = board_new();
    game->turn = X;
    game->cursor = 0;
}

/* Places the mark of the player whose turn it is. Counts the result when the round ends. */
static bool place(Game *game, int cell)
{
    if (board_is_over(&game->board) || !board_play(&game->board, cell, game->turn)) {
        return false;
    }
    if (!board_is_over(&game->board)) {
        game->turn = other_mark(game->turn);
    } else if (board_winner(&game->board) == X) {
        game->x_wins++;
    } else if (board_winner(&game->board) == O) {
        game->o_wins++;
    } else {
        game->draws++;
    }
    return true;
}

/* The human plays X; in vs-computer mode the computer answers at once with O. */
static void human_move(Game *game)
{
    if (place(game, game->cursor) && game->vs_computer && !board_is_over(&game->board)) {
        place(game, board_best_move(&game->board, game->turn));
    }
}

/* The board is 11 text lines: three rows of three lines each, with a separator line between
 * the rows. Every cell is exactly 5 characters wide and the '|' bars sit between them, so
 * all lines line up. Only the 5 characters of a cell are highlighted. */
static void draw_board(const Game *game)
{
    const int *line = board_winning_line(&game->board);
    int top = 2;
    int left = 4;
    int row;
    int part;
    int col;

    for (row = 0; row < 3; row++) {
        for (part = 0; part < 3; part++) {
            int y = top + row * 4 + part;
            for (col = 0; col < 3; col++) {
                int cell = row * 3 + col;
                int x = left + col * 6;
                Mark mark = game->board.cells[cell];
                bool won = line && (cell == line[0] || cell == line[1] || cell == line[2]);
                bool cursor = cell == game->cursor;
                attr_t block = won ? A_REVERSE : A_NORMAL;
                char symbol = mark == X ? 'X' : mark == O ? 'O' : (char)('1' + cell);

                attrset(block);
                mvprintw(y, x, "     ");
                if (part == 1) {
                    attron(cursor ? (A_UNDERLINE | A_BOLD) : A_NORMAL);
                    mvaddch(y, x + 2, symbol);
                }
                attrset(A_NORMAL);
                if (col < 2) {
                    mvaddch(y, x + 5, '|');
                }
            }
            if (part == 2 && row < 2) {
                mvprintw(y + 1, left, "-----+-----+-----");
            }
        }
    }
}

static void draw(const Game *game)
{
    erase();
    mvprintw(0, 4, "Tic-Tac-Toe");
    draw_board(game);

    if (board_winner(&game->board) != EMPTY) {
        mvprintw(14, 4, "%c wins! Press n for a new round.", board_winner(&game->board) == X ? 'X' : 'O');
    } else if (board_is_draw(&game->board)) {
        mvprintw(14, 4, "It is a draw! Press n for a new round.");
    } else {
        mvprintw(14, 4, "Turn: %c", game->turn == X ? 'X' : 'O');
    }
    mvprintw(15, 4, "Mode: %s", game->vs_computer ? "you (X) against the computer (O)" : "two players");
    mvprintw(16, 4, "Score  X wins: %d   O wins: %d   Draws: %d", game->x_wins, game->o_wins, game->draws);
    mvprintw(18, 4, "Arrows or 1-9: move    Enter or Space: place");
    mvprintw(19, 4, "n: new round    m: change mode    q: quit");
    refresh();
}

static void handle_key(Game *game, int key)
{
    switch (key) {
    case KEY_LEFT:
        if (game->cursor % 3 > 0) {
            game->cursor--;
        }
        break;
    case KEY_RIGHT:
        if (game->cursor % 3 < 2) {
            game->cursor++;
        }
        break;
    case KEY_UP:
        if (game->cursor >= 3) {
            game->cursor -= 3;
        }
        break;
    case KEY_DOWN:
        if (game->cursor < 6) {
            game->cursor += 3;
        }
        break;
    case '\n':
    case '\r':
    case ' ':
        human_move(game);
        break;
    case 'n':
    case 'N':
        new_round(game);
        break;
    case 'm':
    case 'M':
        game->vs_computer = !game->vs_computer;
        new_round(game);
        break;
    default:
        if (key >= '1' && key <= '9') {
            game->cursor = key - '1';
            human_move(game);
        }
        break;
    }
}

int main(void)
{
    Game game = {0};
    int key;

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    new_round(&game);

    draw(&game);
    while ((key = getch()) != 'q' && key != 'Q') {
        handle_key(&game, key);
        draw(&game);
    }

    endwin();
    return 0;
}
