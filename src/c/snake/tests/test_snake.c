#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../src/snake.h"

void test_snake_init() {
    SnakeGame game;
    snake_init(&game, 20, 10);
    assert(game.snake_length == 1);
    assert(game.snake[0].x == 10);
    assert(game.snake[0].y == 5);
    assert(game.direction == RIGHT);
    assert(game.get_new_food == 1);
    assert(game.game_over == 0);
}

int main() {
    srand(time(0));
    test_snake_init();
    printf("Snake logic tests passed.\n");
    return 0;
}
