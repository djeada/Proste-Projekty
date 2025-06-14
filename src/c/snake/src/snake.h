#ifndef SNAKE_H
#define SNAKE_H

#include <ncurses.h>

#define MAX_SNAKE_LENGTH 100

typedef struct {
    int x, y;
} SnakeSegment;

typedef enum { RIGHT, LEFT, UP, DOWN } Direction;

typedef struct {
    SnakeSegment snake[MAX_SNAKE_LENGTH];
    int snake_length;
    int food_x, food_y;
    int max_x, max_y;
    Direction direction;
    int get_new_food;
    int game_over;
} SnakeGame;

void snake_init(SnakeGame *game, int max_x, int max_y);
void snake_place_food(SnakeGame *game);
void snake_update_direction(SnakeGame *game, int key);
void snake_move(SnakeGame *game);

#endif // SNAKE_H
