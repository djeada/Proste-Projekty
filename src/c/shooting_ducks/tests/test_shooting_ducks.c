#include "shooting_ducks.h"
#include <assert.h>
#include <stdio.h>

void test_init() {
    DuckGame game; duck_game_init(&game, 40, 20);
    assert(game.crosshair.x == 20);
    assert(game.crosshair.y == 10);
    assert(game.level == 1);
    assert(game.duck_count == BASE_DUCKS);
    int alive = 0; for (int i = 0; i < MAX_DUCKS; ++i) if (game.ducks[i].alive) alive++;
    assert(alive == BASE_DUCKS);
}

void test_move_and_shoot() {
    DuckGame game; duck_game_init(&game, 40, 20);
    int ox = game.crosshair.x, oy = game.crosshair.y;
    duck_game_update(&game, 'a');
    assert(game.crosshair.x == ox - 1);
    duck_game_update(&game, 'd');
    assert(game.crosshair.x == ox);
    // place a duck at crosshair and shoot
    for (int i = 0; i < MAX_DUCKS; ++i) game.ducks[i].alive = 0;
    game.ducks[0].x = game.crosshair.x;
    game.ducks[0].y = game.crosshair.y;
    game.ducks[0].alive = 1;
    duck_game_update(&game, ' ');
    duck_game_update(&game, 0);
    assert(game.ducks[0].alive == 0);
}

int main() {
    test_init();
    test_move_and_shoot();
    printf("All tests passed!\n");
    return 0;
}
