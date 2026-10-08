/* Tests of the game rules. Returns 0 when all tests pass. */
#include "zombie_apocalypse.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

static double dist(Vec2 a, Vec2 b) {
    return hypot(a.x - b.x, a.y - b.y);
}

static Input no_input(void) {
    Input input;
    memset(&input, 0, sizeof input);
    return input;
}

/* A game with the player in the middle and no zombies, pickups or spawning. */
static void empty_field(Game *game) {
    game_init(game, 1);
    game->to_spawn = 0;
    game->zombie_count = 0;
    game->pickup_count = 0;
    game->pickup_timer = 1000.0;
}

static void test_new_game(void) {
    Game game;
    game_init(&game, 42);
    assert(game.health == MAX_HEALTH);
    assert(game.wave == 1);
    assert(game.to_spawn == game_wave_size(1));
    assert(game.score == 0);
    assert(game.game_over == 0);
}

static void test_player_moves_and_stays_in_bounds(void) {
    Game game;
    empty_field(&game);
    Input input = no_input();
    input.move.x = -1.0;
    game_update(&game, &input, 1.0);
    assert(fabs(game.player.x - (WORLD_WIDTH / 2.0 - PLAYER_SPEED)) < 1e-9);

    for (int i = 0; i < 100; i++) {
        game_update(&game, &input, 0.5);
    }
    assert(game.player.x >= 0.5 && game.player.x <= 0.5 + 1e-9);
}

static void test_diagonal_move_is_not_faster(void) {
    Game game;
    empty_field(&game);
    Input input = no_input();
    input.move.x = 1.0;
    input.move.y = 1.0;
    game_update(&game, &input, 1.0);
    double moved = hypot(game.player.x - WORLD_WIDTH / 2.0, game.player.y - WORLD_HEIGHT / 2.0);
    assert(fabs(moved - PLAYER_SPEED) < 1e-9);
}

static void test_shot_flies_in_aim_direction(void) {
    Game game;
    empty_field(&game);
    Input input = no_input();
    input.fire = 1;
    input.aim.x = 0.0;
    input.aim.y = -5.0; /* straight up on the screen */
    game_update(&game, &input, 0.01);
    assert(game.bullet_count == 1);
    assert(fabs(game.bullets[0].vel.x) < 1e-9);
    assert(fabs(game.bullets[0].vel.y + BULLET_SPEED) < 1e-9);
}

static void test_zero_aim_uses_last_move_direction(void) {
    Game game;
    empty_field(&game);
    Input input = no_input();
    input.move.y = 1.0; /* walk down */
    game_update(&game, &input, 0.01);
    input = no_input();
    input.fire = 1;
    game_update(&game, &input, 0.01);
    assert(game.bullet_count == 1);
    assert(game.bullets[0].vel.y > 0.0 && fabs(game.bullets[0].vel.x) < 1e-9);
}

static void test_shot_cooldown(void) {
    Game game;
    empty_field(&game);
    Input input = no_input();
    input.fire = 1;
    input.aim.x = 1.0;
    game_update(&game, &input, 0.01);
    game_update(&game, &input, 0.01);
    assert(game.bullet_count == 1);
    for (int i = 0; i < 30; i++) {
        game_update(&game, &input, 0.01);
    }
    assert(game.bullet_count >= 2);
}

static void test_bullet_leaving_the_world_is_removed(void) {
    Game game;
    empty_field(&game);
    game.bullets[0].pos.x = WORLD_WIDTH - 0.1;
    game.bullets[0].pos.y = 5.0;
    game.bullets[0].vel.x = BULLET_SPEED;
    game.bullet_count = 1;
    game_update(&game, &(Input){0}, 0.1);
    assert(game.bullet_count == 0);
}

static void test_zombies_spawn_on_the_edge(void) {
    Game game;
    game_init(&game, 7);
    game.spawn_timer = 0.0;
    game.player.x = 0.5;
    game.player.y = 0.5;
    game_update(&game, &(Input){0}, 0.0); /* dt 0: the zombie does not move after spawning */
    assert(game.zombie_count == 1);
    Vec2 z = game.zombies[0];
    assert(z.x == 0.0 || z.x == WORLD_WIDTH || z.y == 0.0 || z.y == WORLD_HEIGHT);
    assert(game.to_spawn == game_wave_size(1) - 1);
}

static void test_same_seed_gives_same_numbers(void) {
    Game a;
    Game b;
    game_init(&a, 123);
    game_init(&b, 123);
    for (int i = 0; i < 100; i++) {
        double x = game_random(&a);
        assert(x >= 0.0 && x < 1.0);
        assert(x == game_random(&b));
    }
}

static void test_zombie_chases_player(void) {
    Game game;
    empty_field(&game);
    game.zombies[0].x = 5.0;
    game.zombies[0].y = 10.0;
    game.zombie_count = 1;
    double before = dist(game.zombies[0], game.player);
    game_update(&game, &(Input){0}, 0.5);
    double after = dist(game.zombies[0], game.player);
    assert(after < before);
    assert(fabs((before - after) - game_zombie_speed(1) * 0.5) < 1e-9);
}

static void test_bullet_kills_zombie(void) {
    Game game;
    empty_field(&game);
    game.zombies[0].x = 20.0;
    game.zombies[0].y = 10.0;
    game.zombie_count = 1;
    game.bullets[0].pos.x = 20.0;
    game.bullets[0].pos.y = 10.0;
    game.bullet_count = 1;
    game_update(&game, &(Input){0}, 0.01);
    assert(game.zombie_count == 0);
    assert(game.bullet_count == 0);
    assert(game.score == KILL_SCORE);
}

static void test_zombie_touch_costs_health(void) {
    Game game;
    empty_field(&game);
    game.zombies[0].x = game.player.x + 0.5;
    game.zombies[0].y = game.player.y;
    game.zombie_count = 1;
    game_update(&game, &(Input){0}, 0.01);
    assert(game.zombie_count == 0);
    assert(game.health == MAX_HEALTH - ZOMBIE_DAMAGE);
    assert(game.game_over == 0);
}

static void test_game_over_freezes_the_game(void) {
    Game game;
    empty_field(&game);
    game.health = ZOMBIE_DAMAGE;
    game.zombies[0].x = game.player.x + 0.5;
    game.zombies[0].y = game.player.y;
    game.zombie_count = 1;
    game_update(&game, &(Input){0}, 0.01);
    assert(game.game_over == 1);
    assert(game.health == 0);

    Input input = no_input();
    input.move.x = 1.0;
    double x = game.player.x;
    game_update(&game, &input, 1.0);
    assert(game.player.x == x);
}

static void test_pickup_heals_up_to_max(void) {
    Game game;
    empty_field(&game);
    game.health = MAX_HEALTH - 5;
    game.pickups[0] = game.player;
    game.pickup_count = 1;
    game_update(&game, &(Input){0}, 0.01);
    assert(game.pickup_count == 0);
    assert(game.health == MAX_HEALTH);
}

static void test_wave_clears_and_next_wave_is_harder(void) {
    Game game;
    empty_field(&game);
    game_update(&game, &(Input){0}, 0.01);
    assert(game.wave_delay > 0.0);
    game_update(&game, &(Input){0}, WAVE_BREAK + 0.1);
    assert(game.wave == 2);
    assert(game.to_spawn == game_wave_size(2));
    assert(game.to_spawn > game_wave_size(1));
    assert(game_zombie_speed(2) > game_zombie_speed(1));
}

int main(void) {
    test_new_game();
    test_player_moves_and_stays_in_bounds();
    test_diagonal_move_is_not_faster();
    test_shot_flies_in_aim_direction();
    test_zero_aim_uses_last_move_direction();
    test_shot_cooldown();
    test_bullet_leaving_the_world_is_removed();
    test_zombies_spawn_on_the_edge();
    test_same_seed_gives_same_numbers();
    test_zombie_chases_player();
    test_bullet_kills_zombie();
    test_zombie_touch_costs_health();
    test_game_over_freezes_the_game();
    test_pickup_heals_up_to_max();
    test_wave_clears_and_next_wave_is_harder();
    printf("All zombie_apocalypse tests passed.\n");
    return 0;
}
