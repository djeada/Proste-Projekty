#include "battleship.h"
#include <assert.h>
#include <stdio.h>

void test_init_place_fire() {
    BattleGame g; battleship_init(&g, 80, 24);
    assert(g.phase == PHASE_PLACEMENT);
    // Place first ship at (0,0) horiz
    assert(place_ship(&g.player, 0, 0, 0, 1) == 1);
    // Overlap should fail
    assert(place_ship(&g.player, 1, 0, 0, 1) == 0);
    // Out of bounds should fail
    assert(place_ship(&g.player, 1, 8, 0, 1) == 0);
    // Valid placement
    assert(place_ship(&g.player, 1, 0, 2, 0) == 1);

    // Fire at empty cell
    assert(fire_at(&g.enemy, 0, 0) == 1);
    // Mark enemy ship and hit
    place_ship(&g.enemy, 0, 0, 0, 1);
    assert(fire_at(&g.enemy, 0, 0) == 1);
}

int main() {
    test_init_place_fire();
    printf("All tests passed!\n");
    return 0;
}
