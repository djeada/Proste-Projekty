#include "zombie_apocalypse.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <ncurses.h>

int main() {
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    timeout(100);
    srand(time(0));

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    ZombieGame game;
    zombie_game_init(&game, max_x, max_y);

    while (!game.game_over) {
        clear();
        zombie_game_draw(&game);
        int key = getch();
        zombie_game_update(&game, key);
        refresh();
    }
    clear();
    zombie_game_draw(&game);
    if (game.player.health > 0) {
        mvprintw(max_y / 2, (max_x - 20) / 2, "You survived! Press any key.");
    } else {
        mvprintw(max_y / 2, (max_x - 20) / 2, "You died! Press any key.");
    }
    refresh();
    getch();
    endwin();
    return 0;
}
