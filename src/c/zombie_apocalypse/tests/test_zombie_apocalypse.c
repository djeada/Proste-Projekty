#include "zombie_apocalypse.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

void test_init(void) {
    ZombieGame game;
    memset(&game, 0, sizeof(ZombieGame));
    zombie_game_init(&game, 80, 24);
    
    // Player should be centered in game area
    int expected_x = game.game_area_x + game.game_area_w / 2;
    int expected_y = game.game_area_y + game.game_area_h / 2;
    assert(game.player.x == expected_x);
    assert(game.player.y == expected_y);
    assert(game.player.health == PLAYER_START_HEALTH);
    assert(game.level == 1);
    assert(game.zombie_count == BASE_ZOMBIES);
    
    int zombies_alive = 0;
    for (int i = 0; i < MAX_ZOMBIES; ++i) {
        if (game.zombies[i].alive) zombies_alive++;
    }
    assert(zombies_alive == BASE_ZOMBIES);
}

void test_player_move(void) {
    ZombieGame game;
    memset(&game, 0, sizeof(ZombieGame));
    zombie_game_init(&game, 80, 24);
    
    int old_x = game.player.x;
    int old_y = game.player.y;
    
    zombie_game_update(&game, 'w');
    assert(game.player.y == old_y - 1);
    
    zombie_game_update(&game, 's');
    assert(game.player.y == old_y);
    
    zombie_game_update(&game, 'a');
    assert(game.player.x == old_x - 1);
    
    zombie_game_update(&game, 'd');
    assert(game.player.x == old_x);
}

void test_shoot_and_zombie_hit(void) {
    ZombieGame game;
    memset(&game, 0, sizeof(ZombieGame));
    zombie_game_init(&game, 80, 24);
    
    // Set zombie to the right of player
    game.zombies[0].x = game.player.x + 1;
    game.zombies[0].y = game.player.y;
    game.zombies[0].alive = 1;
    for (int i = 1; i < MAX_ZOMBIES; ++i) game.zombies[i].alive = 0;
    
    // Set direction to right
    game.last_dir = DIR_RIGHT;
    
    // Shoot
    zombie_game_update(&game, ' ');
    
    // Update a few times for bullet to move
    for (int i = 0; i < 3; ++i) zombie_game_update(&game, 0);
    
    assert(game.zombies[0].alive == 0);
}

void test_zombie_attack(void) {
    ZombieGame game;
    memset(&game, 0, sizeof(ZombieGame));
    zombie_game_init(&game, 80, 24);
    
    // Set zombie on player position
    game.zombies[0].x = game.player.x;
    game.zombies[0].y = game.player.y;
    game.zombies[0].alive = 1;
    
    int old_health = game.player.health;
    zombie_game_update(&game, 0);
    
    assert(game.player.health == old_health - 1);
    assert(game.zombies[0].alive == 0);
}

void test_game_area_bounds(void) {
    ZombieGame game;
    memset(&game, 0, sizeof(ZombieGame));
    zombie_game_init(&game, 80, 24);
    
    // Verify game area is calculated correctly
    assert(game.game_area_x == BORDER_WIDTH);
    assert(game.game_area_y == HUD_HEIGHT + BORDER_WIDTH);
    assert(game.game_area_w == 80 - (2 * BORDER_WIDTH));
    assert(game.game_area_h == 24 - HUD_HEIGHT - (2 * BORDER_WIDTH));
}

void test_particle_system(void) {
    ZombieGame game;
    memset(&game, 0, sizeof(ZombieGame));
    zombie_game_init(&game, 80, 24);
    
    // All particles should start inactive
    for (int i = 0; i < MAX_PARTICLES; ++i) {
        assert(game.particles[i].type == PARTICLE_NONE);
    }
}

void test_visual_effects_init(void) {
    ZombieGame game;
    memset(&game, 0, sizeof(ZombieGame));
    zombie_game_init(&game, 80, 24);
    
    // Visual effects should be initialized
    assert(game.level_transition == LEVEL_TRANSITION_DURATION);
    assert(game.damage_flash == 0);
    assert(game.screen_shake == 0);
}

int main(void) {
    test_init();
    test_player_move();
    test_shoot_and_zombie_hit();
    test_zombie_attack();
    test_game_area_bounds();
    test_particle_system();
    test_visual_effects_init();
    printf("All tests passed!\n");
    return 0;
}
