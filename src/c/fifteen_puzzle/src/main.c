/* Terminal interface (ncurses) for the fifteen puzzle. */
#include <ncurses.h>
#include <stdio.h>
#include <time.h>

#include "fifteen_puzzle.h"

static void draw(const Board *board, int moves, int won) {
    char line[64];
    erase();
    mvprintw(0, 2, "Fifteen Puzzle");
    mvprintw(1, 2, "Moves: %d", moves);
    for (int row = 0; row < SIZE; row++) {
        int y = 3 + row * 2;
        mvprintw(y, 2, "+----+----+----+----+");
        for (int col = 0; col < SIZE; col++) {
            int tile = board->tiles[row * SIZE + col];
            if (tile == 0) {
                snprintf(line, sizeof(line), "|    ");
            } else {
                snprintf(line, sizeof(line), "| %2d ", tile);
            }
            mvprintw(y + 1, 2 + col * 5, "%s", line);
        }
        mvprintw(y + 1, 2 + SIZE * 5, "|");
    }
    mvprintw(3 + SIZE * 2, 2, "+----+----+----+----+");
    if (won) {
        mvprintw(12, 2, "You solved it in %d moves! Press n for a new game.", moves);
    } else {
        mvprintw(12, 2, "Arrows: slide a tile   n: new game   q: quit");
    }
    refresh();
}

int main(void) {
    Board board;
    Rng rng;
    int moves = 0;
    int won = 0;
    int key;

    rng_seed(&rng, (unsigned int)time(NULL));
    board_shuffle(&board, &rng);

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    draw(&board, moves, won);

    while ((key = getch()) != 'q') {
        int moved = 0;
        if (key == 'n') {
            board_shuffle(&board, &rng);
            moves = 0;
            won = 0;
        } else if (!won) {
            switch (key) {
                case KEY_UP: moved = board_move_gap(&board, DIR_UP); break;
                case KEY_DOWN: moved = board_move_gap(&board, DIR_DOWN); break;
                case KEY_LEFT: moved = board_move_gap(&board, DIR_LEFT); break;
                case KEY_RIGHT: moved = board_move_gap(&board, DIR_RIGHT); break;
                default: break;
            }
        }
        moves += moved;
        won = board_is_solved(&board);
        draw(&board, moves, won);
    }

    endwin();
    return 0;
}
