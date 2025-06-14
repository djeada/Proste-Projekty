#include "yahtzee.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void roll_dice(int dice[DICE_COUNT], int held[DICE_COUNT]) {
    for (int i = 0; i < DICE_COUNT; ++i) {
        if (!held[i]) dice[i] = (rand() % 6) + 1;
    }
}

void print_dice(const int dice[DICE_COUNT], const int held[DICE_COUNT]) {
    printf("Dice: ");
    for (int i = 0; i < DICE_COUNT; ++i) {
        printf("%d%s ", dice[i], held[i] ? "*" : "");
    }
    printf("\n");
}

int score_upper(int dice[DICE_COUNT], int face) {
    int sum = 0;
    for (int i = 0; i < DICE_COUNT; ++i) if (dice[i] == face) sum += face;
    return sum;
}

int score_three_of_a_kind(int dice[DICE_COUNT]) {
    int counts[7] = {0};
    for (int i = 0; i < DICE_COUNT; ++i) counts[dice[i]]++;
    for (int i = 1; i <= 6; ++i) if (counts[i] >= 3) {
        int sum = 0;
        for (int j = 0; j < DICE_COUNT; ++j) sum += dice[j];
        return sum;
    }
    return 0;
}

int score_four_of_a_kind(int dice[DICE_COUNT]) {
    int counts[7] = {0};
    for (int i = 0; i < DICE_COUNT; ++i) counts[dice[i]]++;
    for (int i = 1; i <= 6; ++i) if (counts[i] >= 4) {
        int sum = 0;
        for (int j = 0; j < DICE_COUNT; ++j) sum += dice[j];
        return sum;
    }
    return 0;
}

int score_full_house(int dice[DICE_COUNT]) {
    int counts[7] = {0};
    int has3 = 0, has2 = 0;
    for (int i = 0; i < DICE_COUNT; ++i) counts[dice[i]]++;
    for (int i = 1; i <= 6; ++i) {
        if (counts[i] == 3) has3 = 1;
        if (counts[i] == 2) has2 = 1;
    }
    return (has3 && has2) ? 25 : 0;
}

int score_small_straight(int dice[DICE_COUNT]) {
    int counts[7] = {0};
    for (int i = 0; i < DICE_COUNT; ++i) counts[dice[i]] = 1;
    if ((counts[1] && counts[2] && counts[3] && counts[4]) ||
        (counts[2] && counts[3] && counts[4] && counts[5]) ||
        (counts[3] && counts[4] && counts[5] && counts[6])) return 30;
    return 0;
}

int score_large_straight(int dice[DICE_COUNT]) {
    int counts[7] = {0};
    for (int i = 0; i < DICE_COUNT; ++i) counts[dice[i]] = 1;
    if ((counts[1] && counts[2] && counts[3] && counts[4] && counts[5]) ||
        (counts[2] && counts[3] && counts[4] && counts[5] && counts[6])) return 40;
    return 0;
}

int score_yahtzee(int dice[DICE_COUNT]) {
    for (int i = 1; i < DICE_COUNT; ++i) if (dice[i] != dice[0]) return 0;
    return 50;
}

int score_chance(int dice[DICE_COUNT]) {
    int sum = 0;
    for (int i = 0; i < DICE_COUNT; ++i) sum += dice[i];
    return sum;
}
