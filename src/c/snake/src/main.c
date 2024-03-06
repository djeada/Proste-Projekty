#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int x, y;
} SnakeSegment;

#define MAX_SNAKE_LENGTH 100

int main() {
    initscr();
    noecho();
    curs_set(0);
    timeout(100);
    srand(time(0));

    int max_y = 0, max_x = 0, food_x = 0, food_y = 0, snake_length = 1;
    getmaxyx(stdscr, max_y, max_x);

    SnakeSegment snake[MAX_SNAKE_LENGTH];
    snake[0].x = max_x / 2;
    snake[0].y = max_y / 2;
    char key = 'l';
    bool get_new_food = true, game_over = false;

    while (!game_over) {
        if (get_new_food) {
            food_x = rand() % max_x + 1;
            food_y = rand() % max_y + 1;
            get_new_food = false;
        }
        mvprintw(food_y, food_x, "*");

        for (int i = 0; i < snake_length; i++) mvprintw(snake[i].y, snake[i].x, "o");

        SnakeSegment new_head = snake[0];
        if (key == 'l') new_head.x++;
        else if (key == 'h') new_head.x--;
        else if (key == 'j') new_head.y++;
        else if (key == 'k') new_head.y--;

        memmove(&snake[1], &snake[0], sizeof(SnakeSegment) * snake_length);
        snake[0] = new_head;
        
        if (snake[0].x >= max_x || snake[0].x <= 0 || snake[0].y >= max_y || snake[0].y <= 0) game_over = true;
        for (int i = 1; i < snake_length; i++)
            if (snake[i].x == snake[0].x && snake[i].y == snake[0].y) game_over = true;

        if (snake[0].x == food_x && snake[0].y == food_y) {
            get_new_food = true;
            snake_length++;
        }

        key = getch();
        refresh();
        clear();
    }

    endwin();
    return 0;
}
