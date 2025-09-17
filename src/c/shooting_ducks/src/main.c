#include "shooting_ducks.h"
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>

int main() {
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    timeout(100);
    srand(time(0));

    int max_y, max_x; getmaxyx(stdscr, max_y, max_x);

    DuckGame game; duck_game_init(&game, max_x, max_y);

    while (!game.game_over) {
        clear();
        duck_game_draw(&game);
        int key = getch();
        duck_game_update(&game, key);
        refresh();
        if (game.wave_cleared) mvprintw(max_y / 2, (max_x - 24) / 2, "Wave cleared! 'n' for next");
    }

    endwin();
    return 0;
}
