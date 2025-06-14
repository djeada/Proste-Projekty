#include "../src/yahtzee.h"
#include <assert.h>
#include <stdio.h>

void test_score_upper() {
    int dice[DICE_COUNT] = {1, 2, 3, 4, 5};
    assert(score_upper(dice, 3) == 3);
}

void test_score_yahtzee() {
    int dice[DICE_COUNT] = {6, 6, 6, 6, 6};
    assert(score_yahtzee(dice) == 50);
}

void test_score_full_house() {
    int dice[DICE_COUNT] = {2, 2, 3, 3, 3};
    assert(score_full_house(dice) == 25);
}

int main() {
    test_score_upper();
    test_score_yahtzee();
    test_score_full_house();
    printf("Yahtzee logic tests passed.\n");
    return 0;
}
