#include <assert.h>
#include <stdio.h>

#include "snake.h"

static int first_free(int n) {
    (void)n;
    return 0;
}

static int last_free(int n) {
    return n - 1;
}

static int same_cell(Cell a, Cell b) {
    return a.x == b.x && a.y == b.y;
}

static void test_init(void) {
    SnakeGame game;
    game_init(&game, first_free);
    assert(game.length == 1);
    assert(same_cell(game.body[0], (Cell){10, 7}));
    assert(game.direction == RIGHT);
    assert(game.score == 0);
    assert(!game.game_over);
    assert(!same_cell(game.food, game.body[0]));
}

static void test_food_uses_random_choice(void) {
    SnakeGame game;
    game_init(&game, first_free);
    assert(same_cell(game.food, (Cell){0, 0}));
    game_init(&game, last_free);
    assert(same_cell(game.food, (Cell){SNAKE_WIDTH - 1, SNAKE_HEIGHT - 1}));
}

static void test_move(void) {
    SnakeGame game;
    game_init(&game, first_free);
    game_step(&game, first_free);
    assert(same_cell(game.body[0], (Cell){11, 7}));
    assert(game.length == 1);
}

static void test_cannot_reverse(void) {
    SnakeGame game;
    game_init(&game, first_free);
    game_turn(&game, LEFT);
    game_step(&game, first_free);
    assert(game.direction == RIGHT);
    assert(same_cell(game.body[0], (Cell){11, 7}));
}

static void test_two_turns_in_one_step_cannot_reverse(void) {
    SnakeGame game;
    game_init(&game, first_free);
    game_turn(&game, UP);
    game_turn(&game, LEFT); /* reverses the last step, so it is ignored */
    game_step(&game, first_free);
    assert(game.direction == UP);
    assert(same_cell(game.body[0], (Cell){10, 6}));
}

static void test_wall_ends_game(void) {
    SnakeGame game;
    game_init(&game, first_free);
    game.body[0] = (Cell){0, 5};
    game.direction = game.next_direction = LEFT;
    game_step(&game, first_free);
    assert(game.game_over);
}

static void test_self_collision_ends_game(void) {
    SnakeGame game;
    game_init(&game, first_free);
    Cell body[] = {{5, 5}, {4, 5}, {4, 6}, {5, 6}};
    for (int i = 0; i < 4; i++) {
        game.body[i] = body[i];
    }
    game.length = 4;
    game.next_direction = DOWN;
    game_step(&game, first_free);
    assert(game.game_over);
}

static void test_eating_grows_and_scores(void) {
    SnakeGame game;
    game_init(&game, first_free);
    game.food = (Cell){11, 7};
    game_step(&game, first_free);
    assert(game.length == 2);
    assert(game.score == 10);
    assert(!game.game_over);
    assert(!same_cell(game.food, game.body[0]));
    assert(!same_cell(game.food, game.body[1]));
}

static void test_speed_increases_with_length(void) {
    SnakeGame game;
    game_init(&game, first_free);
    assert(game_delay_ms(&game) == 150);
    game.length = 10;
    assert(game_delay_ms(&game) == 105);
    game.length = SNAKE_MAX_CELLS;
    assert(game_delay_ms(&game) == 60);
}

int main(void) {
    test_init();
    test_food_uses_random_choice();
    test_move();
    test_cannot_reverse();
    test_two_turns_in_one_step_cannot_reverse();
    test_wall_ends_game();
    test_self_collision_ends_game();
    test_eating_grows_and_scores();
    test_speed_increases_with_length();
    printf("Snake logic tests passed.\n");
    return 0;
}
