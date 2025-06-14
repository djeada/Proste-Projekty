#include "zombie_apocalypse.h"
#include <assert.h>
#include <stdio.h>

void test_init() {
    ZombieApocalypse game;
    zombie_apocalypse_init(&game);
    assert(game.player_health == 100);
    assert(game.zombie_count == 10);
}

void test_tick() {
    ZombieApocalypse game;
    zombie_apocalypse_init(&game);
    zombie_apocalypse_tick(&game);
    assert(game.player_health == 90);
    assert(game.zombie_count == 9);
}

void test_is_over() {
    ZombieApocalypse game;
    zombie_apocalypse_init(&game);
    for (int i = 0; i < 10; ++i) {
        zombie_apocalypse_tick(&game);
    }
    assert(zombie_apocalypse_is_over(&game));
}

int main() {
    test_init();
    test_tick();
    test_is_over();
    printf("All tests passed!\n");
    return 0;
}
