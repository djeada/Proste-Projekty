#include "battleship.h"
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

int main() {
    initscr(); noecho(); curs_set(0); keypad(stdscr, TRUE); timeout(100);
    srand(time(0));
    int max_y, max_x; getmaxyx(stdscr, max_y, max_x);
    BattleGame game; battleship_init(&game, max_x, max_y);
    while (1) {
        clear();
        battleship_draw(&game);
        int key = getch();
        battleship_update(&game, key);
        refresh();
    }
    endwin();
    return 0;
}
