#include "yahtzee.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DICE_SIDES 6
#define COUNTS_SIZE (DICE_SIDES + 1)
#define FULL_HOUSE_SCORE 25
#define SMALL_STRAIGHT_SCORE 30
#define LARGE_STRAIGHT_SCORE 40
#define YAHTZEE_SCORE 50

void roll_dice(int dice[DICE_COUNT], const int held[DICE_COUNT]) {
    for (int i = 0; i < DICE_COUNT; ++i) {
        if (!held[i]) {
            dice[i] = (rand() % DICE_SIDES) + 1;
        }
    }
}

void print_dice(const int dice[DICE_COUNT], const int held[DICE_COUNT]) {
    printf("Dice: ");
    for (int i = 0; i < DICE_COUNT; ++i) {
        printf("%d%s ", dice[i], held[i] ? "*" : "");
    }
    printf("\n");
}

int score_upper(const int dice[DICE_COUNT], int face) {
    int sum = 0;
    for (int i = 0; i < DICE_COUNT; ++i) {
        if (dice[i] == face) {
            sum += face;
        }
    }
    return sum;
}

int score_three_of_a_kind(const int dice[DICE_COUNT]) {
    int counts[COUNTS_SIZE] = {0};
    for (int i = 0; i < DICE_COUNT; ++i) {
        counts[dice[i]]++;
    }
    for (int i = 1; i <= DICE_SIDES; ++i) {
        if (counts[i] >= 3) {
            int sum = 0;
            for (int j = 0; j < DICE_COUNT; ++j) {
                sum += dice[j];
            }
            return sum;
        }
    }
    return 0;
}

int score_four_of_a_kind(const int dice[DICE_COUNT]) {
    int counts[COUNTS_SIZE] = {0};
    for (int i = 0; i < DICE_COUNT; ++i) {
        counts[dice[i]]++;
    }
    for (int i = 1; i <= DICE_SIDES; ++i) {
        if (counts[i] >= 4) {
            int sum = 0;
            for (int j = 0; j < DICE_COUNT; ++j) {
                sum += dice[j];
            }
            return sum;
        }
    }
    return 0;
}

int score_full_house(const int dice[DICE_COUNT]) {
    int counts[COUNTS_SIZE] = {0};
    int has3 = 0;
    int has2 = 0;
    for (int i = 0; i < DICE_COUNT; ++i) {
        counts[dice[i]]++;
    }
    for (int i = 1; i <= DICE_SIDES; ++i) {
        if (counts[i] == 3) {
            has3 = 1;
        }
        if (counts[i] == 2) {
            has2 = 1;
        }
    }
    return (has3 && has2) ? FULL_HOUSE_SCORE : 0;
}

int score_small_straight(const int dice[DICE_COUNT]) {
    int counts[COUNTS_SIZE] = {0};
    for (int i = 0; i < DICE_COUNT; ++i) {
        counts[dice[i]] = 1;
    }
    if ((counts[1] && counts[2] && counts[3] && counts[4]) ||
        (counts[2] && counts[3] && counts[4] && counts[DICE_COUNT]) ||
        (counts[3] && counts[4] && counts[DICE_COUNT] && counts[DICE_SIDES])) {
        return SMALL_STRAIGHT_SCORE;
    }
    return 0;
}

int score_large_straight(const int dice[DICE_COUNT]) {
    int counts[COUNTS_SIZE] = {0};
    for (int i = 0; i < DICE_COUNT; ++i) {
        counts[dice[i]] = 1;
    }
    if ((counts[1] && counts[2] && counts[3] && counts[4] && counts[DICE_COUNT]) ||
        (counts[2] && counts[3] && counts[4] && counts[DICE_COUNT] && counts[DICE_SIDES])) {
        return LARGE_STRAIGHT_SCORE;
    }
    return 0;
}

int score_yahtzee(const int dice[DICE_COUNT]) {
    for (int i = 1; i < DICE_COUNT; ++i) {
        if (dice[i] != dice[0]) {
            return 0;
        }
    }
    return YAHTZEE_SCORE;
}

int score_chance(const int dice[DICE_COUNT]) {
    int sum = 0;
    for (int i = 0; i < DICE_COUNT; ++i) {
        sum += dice[i];
    }
    return sum;
}
