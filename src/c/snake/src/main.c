#include "snake.h"
#include <stdlib.h>
#include <time.h>

int main() {
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    timeout(100);
    srand(time(0));

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    SnakeGame game;
    snake_init(&game, max_x, max_y);

    while (!game.game_over) {
        clear();
        if (game.get_new_food) {
            snake_place_food(&game);
        }
        mvprintw(game.food_y, game.food_x, "*");
        for (int i = 0; i < game.snake_length; i++) {
            mvprintw(game.snake[i].y, game.snake[i].x, "o");
        }
        int key = getch();
        snake_update_direction(&game, key);
        snake_move(&game);
        refresh();
    }
    endwin();
    return 0;
}

