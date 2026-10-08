/* Tests of the game rules; they need no terminal. */
#include "shooting_ducks.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>

#define NEAR(a, b) (fabs((a) - (b)) < 1e-9)

static void spawn_first_duck(Game *game) { game_update(game, 0.001); }

static void test_new_game(void) {
    Game game;
    game_init(&game, 42);
    assert(game.wave == 1);
    assert(game.score == 0);
    assert(game.lives == START_LIVES);
    assert(game.duck_count == 0);
    assert(game.ducks_to_spawn == ducks_in_wave(1));
    assert(!game.game_over);
}

static void test_random_source(void) {
    Rng a, b;
    rng_seed(&a, 7);
    rng_seed(&b, 7);
    for (int i = 0; i < 1000; ++i) {
        double value = rng_next(&a);
        assert(value == rng_next(&b));
        assert(value >= 0.0 && value < 1.0);
    }
}

static void test_wave_spawns_all_ducks(void) {
    Game game;
    game_init(&game, 3);
    for (int i = 0; i < 100 && game.ducks_to_spawn > 0; ++i) game_update(&game, 0.1);
    assert(game.ducks_to_spawn == 0);
    assert(game.duck_count > 0);
}

static void test_movement_uses_elapsed_time(void) {
    Game game;
    game_init(&game, 5);
    spawn_first_duck(&game);
    Duck before = game.ducks[0];
    game_update(&game, 0.5);
    assert(NEAR(game.ducks[0].x, before.x + before.speed * 0.5));
}

static void test_shooting_hits_and_misses(void) {
    Game game;
    game_init(&game, 5);
    spawn_first_duck(&game);
    Duck duck = game.ducks[0];

    assert(game_shoot(&game, duck.x + 50.0, duck.y) == 0);
    assert(game.score == 0);
    assert(game.duck_count == 1);

    assert(game_shoot(&game, duck.x, duck.y) == 1);
    assert(game.score == POINTS_PER_HIT);
    assert(game.duck_count == 0);
}

static void test_escape_costs_a_life(void) {
    Game game;
    game_init(&game, 9);
    spawn_first_duck(&game);
    game.ducks[0].speed = 1.0;
    game.ducks[0].x = FIELD_WIDTH + DUCK_HALF_WIDTH + 1.0;
    game_update(&game, 0.001);
    assert(game.lives == START_LIVES - 1);
    assert(game.duck_count == 0);
    assert(!game.game_over);
}

static void test_three_escapes_end_the_game(void) {
    Game game;
    game_init(&game, 9);
    game.ducks_to_spawn = 0;
    for (int i = 0; i < START_LIVES; ++i) {
        game.break_timer = 0.0;
        game.ducks[0].x = -DUCK_HALF_WIDTH - 1.0;
        game.ducks[0].y = 10.0;
        game.ducks[0].base_y = 10.0;
        game.ducks[0].speed = -1.0;
        game.ducks[0].age = 0.0;
        game.ducks[0].phase = 0.0;
        game.duck_count = 1;
        game_update(&game, 0.001);
    }
    assert(game.game_over);
    assert(game.lives == 0);

    int score = game.score;
    game_update(&game, 1.0);
    assert(game.wave == 1);
    assert(game_shoot(&game, 0.0, 0.0) == 0);
    assert(game.score == score);
}

static void test_wave_progression(void) {
    Game game;
    game_init(&game, 11);
    game.ducks_to_spawn = 0;
    game.duck_count = 0;
    game_update(&game, 0.01);
    assert(game.break_timer > 0);

    game_update(&game, WAVE_BREAK_SECONDS);
    assert(game.wave == 2);
    assert(game.ducks_to_spawn == ducks_in_wave(2));
    assert(game.break_timer == 0);
}

static void test_waves_grow_and_speed_up(void) {
    assert(ducks_in_wave(2) > ducks_in_wave(1));

    Game first, later;
    game_init(&first, 21);
    game_init(&later, 21);
    later.wave = 5;
    spawn_first_duck(&first);
    spawn_first_duck(&later);
    assert(fabs(later.ducks[0].speed) > fabs(first.ducks[0].speed));
}

static void test_ducks_fly_both_ways(void) {
    Game game;
    game_init(&game, 1234);
    int right = 0, left = 0;
    for (int i = 0; i < 40; ++i) {
        game.duck_count = 0;
        game.ducks_to_spawn = 1;
        game.spawn_timer = 0.0;
        game_update(&game, 0.001);
        if (game.duck_count) {
            if (game.ducks[0].speed > 0) right++;
            else left++;
        }
    }
    assert(right > 0 && left > 0);
}

int main(void) {
    test_new_game();
    test_random_source();
    test_wave_spawns_all_ducks();
    test_movement_uses_elapsed_time();
    test_shooting_hits_and_misses();
    test_escape_costs_a_life();
    test_three_escapes_end_the_game();
    test_wave_progression();
    test_waves_grow_and_speed_up();
    test_ducks_fly_both_ways();
    printf("All tests passed!\n");
    return 0;
}
