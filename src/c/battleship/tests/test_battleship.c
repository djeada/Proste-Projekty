/* Tests of the battleship rules. Returns 0 when every check passes. */
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "battleship.h"

static void test_placement_inside_board(void) {
    Board b;
    board_reset(&b);
    assert(board_place(&b, 0, 0, 0, 1));
    assert(!board_place(&b, 1, 8, 0, 1));  /* 4 cells would leave the right edge */
    assert(!board_place(&b, 1, 0, 9, 0));  /* 4 cells would leave the bottom edge */
    assert(board_place(&b, 1, 9, 0, 0));   /* exactly fits in the last column */
}

static void test_touching_allowed_overlap_not(void) {
    Board b;
    board_reset(&b);
    assert(board_place(&b, 0, 0, 0, 1));   /* cells (0..4, 0) */
    assert(!board_place(&b, 1, 4, 0, 0));  /* overlaps at (4, 0) */
    assert(board_place(&b, 1, 5, 0, 1));   /* touches the first ship: allowed */
    assert(!board_place(&b, 1, 0, 2, 1));  /* the same ship cannot be placed twice */
}

static void test_fire_miss_hit_sunk(void) {
    Board b;
    board_reset(&b);
    assert(board_place(&b, 4, 0, 0, 1));   /* ship of length 2 at (0,0)-(1,0) */
    assert(board_fire(&b, 5, 5) == SHOT_MISS);
    assert(board_fire(&b, 0, 0) == SHOT_HIT);
    assert(board_fire(&b, 1, 0) == SHOT_SUNK);
    assert(!board_all_sunk(&b));           /* the other ships are not placed */
}

static void test_repeat_and_invalid_shots(void) {
    Board b;
    board_reset(&b);
    board_place(&b, 4, 0, 0, 1);
    assert(board_fire(&b, 0, 0) == SHOT_HIT);
    assert(board_fire(&b, 0, 0) == SHOT_REPEAT);
    assert(b.ships[4].hits == 1);
    assert(board_fire(&b, 10, 0) == SHOT_INVALID);
    assert(board_fire(&b, -1, 3) == SHOT_INVALID);
}

static void test_fleet_destroyed(void) {
    Board b;
    Rng rng;
    rng_seed(&rng, 1);
    board_place_random(&b, &rng);
    assert(board_fleet_placed(&b));
    assert(!board_all_sunk(&b));
    for (int y = 0; y < BOARD_SIZE; ++y) {
        for (int x = 0; x < BOARD_SIZE; ++x) {
            board_fire(&b, x, y);
        }
    }
    assert(board_all_sunk(&b));
}

static void test_random_fleet_is_valid_and_repeatable(void) {
    for (unsigned int seed = 1; seed <= 20; ++seed) {
        Board b;
        Rng rng;
        rng_seed(&rng, seed);
        board_place_random(&b, &rng);
        assert(board_fleet_placed(&b));
        int cells = 0;
        for (int y = 0; y < BOARD_SIZE; ++y) {
            for (int x = 0; x < BOARD_SIZE; ++x) {
                if (b.ship_at[y][x] != NO_SHIP) cells++;
            }
        }
        assert(cells == 5 + 4 + 3 + 3 + 2);
    }
    Board first, again;
    Rng r1, r2;
    rng_seed(&r1, 7);
    rng_seed(&r2, 7);
    board_place_random(&first, &r1);
    board_place_random(&again, &r2);
    assert(memcmp(first.ship_at, again.ship_at, sizeof first.ship_at) == 0);
}

static void test_computer_follows_hit_to_neighbours(void) {
    Board b;
    Rng rng;
    Computer c;
    rng_seed(&rng, 3);
    board_reset(&b);
    board_place(&b, 0, 0, 0, 1);
    computer_reset(&c);
    Point hit = {5, 5};
    board_fire(&b, 5, 5);
    computer_report(&c, &b, hit, SHOT_HIT);
    assert(c.count == 4);
    Point next = computer_choose(&c, &b, &rng);
    int neighbour = (next.x == 4 && next.y == 5) || (next.x == 6 && next.y == 5) ||
                    (next.x == 5 && next.y == 4) || (next.x == 5 && next.y == 6);
    assert(neighbour);
}

static void test_computer_forgets_targets_after_sinking(void) {
    Board b;
    Computer c;
    board_reset(&b);
    computer_reset(&c);
    Point p = {5, 5};
    computer_report(&c, &b, p, SHOT_HIT);
    assert(c.count == 4);
    computer_report(&c, &b, p, SHOT_SUNK);
    assert(c.count == 0);
}

static void test_computer_finishes_game_without_repeats(void) {
    Board mine, theirs;
    Rng rng;
    Computer c;
    rng_seed(&rng, 42);
    board_reset(&theirs);
    board_place_random(&theirs, &rng);
    board_reset(&mine);
    board_place_random(&mine, &rng);
    computer_reset(&c);
    int shots = 0;
    while (!board_all_sunk(&theirs)) {
        Point p = computer_choose(&c, &theirs, &rng);
        ShotResult r = board_fire(&theirs, p.x, p.y);
        assert(r == SHOT_MISS || r == SHOT_HIT || r == SHOT_SUNK);
        computer_report(&c, &theirs, p, r);
        shots++;
        assert(shots <= BOARD_SIZE * BOARD_SIZE);
    }
}

int main(void) {
    test_placement_inside_board();
    test_touching_allowed_overlap_not();
    test_fire_miss_hit_sunk();
    test_repeat_and_invalid_shots();
    test_fleet_destroyed();
    test_random_fleet_is_valid_and_repeatable();
    test_computer_follows_hit_to_neighbours();
    test_computer_forgets_targets_after_sinking();
    test_computer_finishes_game_without_repeats();
    printf("All battleship tests passed.\n");
    return 0;
}
