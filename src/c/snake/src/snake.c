#include "snake.h"
#include <stdlib.h>
#include <string.h>

void snake_init(SnakeGame *game, int max_x, int max_y) {
    game->max_x = max_x;
    game->max_y = max_y;
    game->snake_length = 1;
    game->snake[0].x = max_x / 2;
    game->snake[0].y = max_y / 2;
    game->direction = RIGHT;
    game->get_new_food = 1;
    game->game_over = 0;
}

void snake_place_food(SnakeGame *game) {
    game->food_x = (rand() % (game->max_x - 2)) + 1;
    game->food_y = (rand() % (game->max_y - 2)) + 1;
    game->get_new_food = 0;
}

void snake_update_direction(SnakeGame *game, int key) {
    switch (key) {
        case KEY_RIGHT:
            if (game->direction != LEFT) game->direction = RIGHT;
            break;
        case KEY_LEFT:
            if (game->direction != RIGHT) game->direction = LEFT;
            break;
        case KEY_UP:
            if (game->direction != DOWN) game->direction = UP;
            break;
        case KEY_DOWN:
            if (game->direction != UP) game->direction = DOWN;
            break;
        default:
            break;
    }
}

void snake_move(SnakeGame *game) {
    SnakeSegment new_head = game->snake[0];
    switch (game->direction) {
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
    for (int i = 1; i < game->snake_length; i++) {
        if (game->snake[i].x == new_head.x && game->snake[i].y == new_head.y) {
            game->game_over = 1;
        }
    }

    // Check for wall collision
    if (new_head.x >= game->max_x || new_head.x < 0 || new_head.y >= game->max_y || new_head.y < 0) {
        game->game_over = 1;
    } else {
        memmove(&game->snake[1], &game->snake[0], sizeof(SnakeSegment) * (game->snake_length - 1));
        game->snake[0] = new_head;
    }

    if (game->snake[0].x == game->food_x && game->snake[0].y == game->food_y) {
        game->get_new_food = 1;
        if (game->snake_length < MAX_SNAKE_LENGTH) {
            game->snake[game->snake_length] = game->snake[game->snake_length - 1];
            game->snake_length++;
        }
    }
}
