#include "snake.h"

#include <string.h>

static int is_occupied(const SnakeGame *game, int x, int y) {
    for (int i = 0; i < game->length; i++) {
        if (game->body[i].x == x && game->body[i].y == y) {
            return 1;
        }
    }
    return 0;
}

static int is_outside(int x, int y) {
    return x < 0 || x >= SNAKE_WIDTH || y < 0 || y >= SNAKE_HEIGHT;
}

static Direction opposite(Direction direction) {
    switch (direction) {
        case UP:
            return DOWN;
        case DOWN:
            return UP;
        case LEFT:
            return RIGHT;
        default:
            return LEFT;
    }
}

/* Puts food on the n-th free cell, where n is chosen by random_below. */
static void place_food(SnakeGame *game, RandomFn random_below) {
    int free_cells = SNAKE_MAX_CELLS - game->length;
    if (free_cells == 0) {
        game->game_over = 1; /* the board is full */
        return;
    }
    int choice = random_below(free_cells);
    for (int y = 0; y < SNAKE_HEIGHT; y++) {
        for (int x = 0; x < SNAKE_WIDTH; x++) {
            if (is_occupied(game, x, y)) {
                continue;
            }
            if (choice == 0) {
                game->food.x = x;
                game->food.y = y;
                return;
            }
            choice--;
        }
    }
}

void game_init(SnakeGame *game, RandomFn random_below) {
    memset(game, 0, sizeof *game);
    game->body[0].x = SNAKE_WIDTH / 2;
    game->body[0].y = SNAKE_HEIGHT / 2;
    game->length = 1;
    game->direction = RIGHT;
    game->next_direction = RIGHT;
    place_food(game, random_below);
}

/* A turn is ignored if it would reverse the last step, so the snake never runs into its neck. */
void game_turn(SnakeGame *game, Direction direction) {
    if (direction != opposite(game->direction)) {
        game->next_direction = direction;
    }
}

void game_step(SnakeGame *game, RandomFn random_below) {
    if (game->game_over) {
        return;
    }
    game->direction = game->next_direction;

    Cell head = game->body[0];
    switch (game->direction) {
        case UP:
            head.y--;
            break;
        case DOWN:
            head.y++;
            break;
        case LEFT:
            head.x--;
            break;
        case RIGHT:
            head.x++;
            break;
    }
    if (is_outside(head.x, head.y) || is_occupied(game, head.x, head.y)) {
        game->game_over = 1;
        return;
    }

    int eats = head.x == game->food.x && head.y == game->food.y;
    /* Without eating, the tail leaves its cell; when eating, nothing leaves. */
    int moving = eats ? game->length : game->length - 1;
    memmove(&game->body[1], &game->body[0], moving * sizeof(Cell));
    game->body[0] = head;

    if (eats) {
        game->length++;
        game->score += 10;
        place_food(game, random_below);
    }
}

/* Starts at 150 ms per step and gets 5 ms faster per segment, but never faster than 60 ms. */
int game_delay_ms(const SnakeGame *game) {
    int delay = 150 - 5 * (game->length - 1);
    return delay < 60 ? 60 : delay;
}
