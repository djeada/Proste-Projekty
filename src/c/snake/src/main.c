#include <string.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int x, y;
} SnakeSegment;

#define MAX_SNAKE_LENGTH 100

typedef enum { RIGHT, LEFT, UP, DOWN } Direction;

int main() {
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE); // Enable keyboard mapping
    timeout(100);         // Non-blocking delay for getch()
    srand(time(0));

    int max_y, max_x, food_x, food_y, snake_length = 1;
    getmaxyx(stdscr, max_y, max_x);

    SnakeSegment snake[MAX_SNAKE_LENGTH];
    snake[0].x = max_x / 2;
    snake[0].y = max_y / 2;
    Direction direction = RIGHT;
    bool get_new_food = true, game_over = false;

    while (!game_over) {
        clear();
        if (get_new_food) {
            food_x = (rand() % (max_x - 2)) + 1;
            food_y = (rand() % (max_y - 2)) + 1;
            get_new_food = false;
        }
        mvprintw(food_y, food_x, "*");

        for (int i = 0; i < snake_length; i++) {
            mvprintw(snake[i].y, snake[i].x, "o");
        }

        int key = getch();
        switch (key) {
            case KEY_RIGHT:
                if (direction != LEFT) direction = RIGHT;
                break;
            case KEY_LEFT:
                if (direction != RIGHT) direction = LEFT;
                break;
            case KEY_UP:
                if (direction != DOWN) direction = UP;
                break;
            case KEY_DOWN:
                if (direction != UP) direction = DOWN;
                break;
        }

        SnakeSegment new_head = snake[0];
        switch (direction) {
            case RIGHT:
                new_head.x++;
                break;
            case LEFT:
                new_head.x--;
                break;
            case UP:
                new_head.y--;
                break;
            case DOWN:
                new_head.y++;
                break;
        }

        // Check for self collision
        for (int i = 1; i < snake_length; i++) {
            if (snake[i].x == new_head.x && snake[i].y == new_head.y) {
                game_over = true;
            }
        }

        // Check for wall collision
        if (new_head.x >= max_x || new_head.x < 0 || new_head.y >= max_y || new_head.y < 0) {
            game_over = true;
        } else {
            memmove(&snake[1], &snake[0], sizeof(SnakeSegment) * (snake_length - 1));
            snake[0] = new_head;
        }

        if (snake[0].x == food_x && snake[0].y == food_y) {
            get_new_food = true;
            if (snake_length < MAX_SNAKE_LENGTH) {
                snake[snake_length] = snake[snake_length - 1];
                snake_length++;
            }
        }

        refresh();
    }

    endwin();
    return 0;
}

