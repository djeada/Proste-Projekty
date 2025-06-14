#include "../src/fifteen.h"
#include <assert.h>
#include <stdio.h>

void test_puzzle_init_and_solved() {
    FifteenPuzzle puzzle;
    puzzle_init(&puzzle);
    assert(puzzle_is_solved(&puzzle));
}

void test_puzzle_move() {
    FifteenPuzzle puzzle;
    puzzle_init(&puzzle);
    // 's' (move up) is always valid from initial state (3,3)
    assert(puzzle_move(&puzzle, 's'));
    assert(!puzzle_is_solved(&puzzle));
}

int main() {
    test_puzzle_init_and_solved();
    test_puzzle_move();
    printf("Fifteen puzzle logic tests passed.\n");
    return 0;
}
