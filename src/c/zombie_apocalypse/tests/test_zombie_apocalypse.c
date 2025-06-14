#include "zombie_apocalypse.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

void test_init() {
    ZombieGame game;
    zombie_game_init(&game, 40, 20);
    assert(game.player.x == 20);
    assert(game.player.y == 10);
    assert(game.player.health == PLAYER_START_HEALTH);
    assert(game.zombie_count == MAX_ZOMBIES);
    int zombies_alive = 0;
    for (int i = 0; i < MAX_ZOMBIES; ++i) {
        if (game.zombies[i].alive) zombies_alive++;
    }
    assert(zombies_alive == MAX_ZOMBIES);
}

void test_player_move() {
    ZombieGame game;
    zombie_game_init(&game, 40, 20);
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

void test_shoot_and_zombie_hit() {
    ZombieGame game;
    zombie_game_init(&game, 40, 20);
    // Ustaw zombie na prawo od gracza
    game.zombies[0].x = game.player.x + 1;
    game.zombies[0].y = game.player.y;
    for (int i = 1; i < MAX_ZOMBIES; ++i) game.zombies[i].alive = 0;
    zombie_game_update(&game, ' '); // strzał w prawo
    for (int i = 0; i < 2; ++i) zombie_game_update(&game, 0); // przesuwanie pocisku
    assert(game.zombies[0].alive == 0);
}

void test_zombie_attack() {
    ZombieGame game;
    zombie_game_init(&game, 40, 20);
    // Ustaw zombie na graczu
    game.zombies[0].x = game.player.x;
    game.zombies[0].y = game.player.y;
    int old_health = game.player.health;
    zombie_game_update(&game, 0);
    assert(game.player.health == old_health - 1);
    assert(game.zombies[0].alive == 0);
}

int main() {
    test_init();
    test_player_move();
    test_shoot_and_zombie_hit();
    test_zombie_attack();
    printf("All tests passed!\n");
    return 0;
}
