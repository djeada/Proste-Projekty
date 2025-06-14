#include "yahtzee.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int dice[DICE_COUNT] = {0};
    int held[DICE_COUNT] = {0};
    srand((unsigned)time(NULL));
    printf("Welcome to Yahtzee!\nRolls: up to 3 per turn. Hold dice with h <index>.\n\n");
    for (int turn = 0; turn < 13; ++turn) {
        for (int i = 0; i < DICE_COUNT; ++i) held[i] = 0;
        int rolls = 0;
        while (rolls < 3) {
            roll_dice(dice, held);
            print_dice(dice, held);
            printf("Enter h <i> to hold/release, r to reroll, s to score: ");
            char cmd;
            int idx;
            int n = scanf(" %c", &cmd);
            if (cmd == 'h') {
                scanf(" %d", &idx);
                if (idx >= 0 && idx < DICE_COUNT) held[idx] = !held[idx];
            } else if (cmd == 'r') {
                rolls++;
            } else if (cmd == 's') {
                break;
            }
        }
        printf("Score for this roll: %d (chance)\n", score_chance(dice));
    }
    printf("Game over!\n");
    return 0;
}
