#include "yahtzee.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ANSI_CLEAR "\033[2J\033[H"
#define ANSI_BOLD "\033[1m"
#define ANSI_DIM "\033[2m"
#define ANSI_GREEN "\033[32m"
#define ANSI_YELLOW "\033[33m"
#define ANSI_RESET "\033[0m"

static const char *CATEGORY_NAMES[CATEGORY_COUNT] = {
    "Ones", "Twos", "Threes", "Fours", "Fives", "Sixes",
    "Three of a Kind", "Four of a Kind", "Full House",
    "Small Straight", "Large Straight", "Yahtzee", "Chance"
};

static int compute_category_score(int catIndex, const int dice[DICE_COUNT]) {
    switch (catIndex) {
        case 0: return score_upper(dice, 1);
        case 1: return score_upper(dice, 2);
        case 2: return score_upper(dice, 3);
        case 3: return score_upper(dice, 4);
        case 4: return score_upper(dice, 5);
        case 5: return score_upper(dice, 6);
        case 6: return score_three_of_a_kind(dice);
        case 7: return score_four_of_a_kind(dice);
        case 8: return score_full_house(dice);
        case 9: return score_small_straight(dice);
        case 10:return score_large_straight(dice);
        case 11:return score_yahtzee(dice);
        case 12:return score_chance(dice);
        default:return 0;
    }
}

static void render_screen(const int dice[DICE_COUNT], const int held[DICE_COUNT], int turn, int rolls, int total) {
    printf(ANSI_CLEAR);
    printf(ANSI_BOLD "Yahtzee" ANSI_RESET "  | Turn %d/%d  | Roll %d/3  | Total: %d\n", turn + 1, CATEGORY_COUNT, rolls, total);
    printf("----------------------------------------------\n");
    // Indices row
    printf("Idx : ");
    for (int i = 0; i < DICE_COUNT; ++i) {
        printf(" %d  ", i + 1);
    }
    printf("\n");
    // Dice row
    printf("Dice: ");
    for (int i = 0; i < DICE_COUNT; ++i) {
        if (held[i]) {
            printf(ANSI_GREEN "[%d]* " ANSI_RESET, dice[i]);
        } else {
            printf("[%d]  ", dice[i]);
        }
    }
    printf("\n");
    printf(ANSI_DIM "* held (kept on reroll)" ANSI_RESET "\n\n");
    printf("Commands: h <1-5> (toggle hold), r (reroll), " ANSI_YELLOW "s (score)" ANSI_RESET ", q (quit)\n");
}

int main() {
    int dice[DICE_COUNT] = {0};
    int held[DICE_COUNT] = {0};
    int total = 0;
    int used[CATEGORY_COUNT] = {0};
    int scores[CATEGORY_COUNT];
    for (int i = 0; i < CATEGORY_COUNT; ++i) scores[i] = -1;
    srand((unsigned)time(NULL));

    printf(ANSI_BOLD "Welcome to Yahtzee!" ANSI_RESET "\n");
    printf("- Up to 3 rolls per turn.\n- Toggle holds with 'h <1-5>'.\n- 'r' to reroll, 's' to choose a scoring category, 'q' to quit.\n\n");

    char line[128];
    for (int turn = 0; turn < CATEGORY_COUNT; ++turn) {
        // reset holds
        for (int i = 0; i < DICE_COUNT; ++i) {
            held[i] = 0;
        }

        // First roll happens automatically
        roll_dice(dice, held);
        int rolls = 1;

        for (;;) {
            render_screen(dice, held, turn, rolls, total);
            printf("> ");
            if (!fgets(line, sizeof line, stdin)) {
                printf("\nInput error. Exiting.\n");
                return 1;
            }

            // trim leading spaces
            char *p = line;
            while (*p == ' ' || *p == '\t') ++p;
            if (*p == '\0' || *p == '\n') {
                continue; // empty input
            }

            if (p[0] == 'q' || p[0] == 'Q') {
                printf("\nQuitting...\n");
                return 0;
            }

            if (p[0] == 'h' || p[0] == 'H') {
                int idx = -1;
                // try to parse an integer after h
                if (sscanf(p + 1, "%d", &idx) == 1) {
                    if (idx >= 1 && idx <= DICE_COUNT) {
                        held[idx - 1] = !held[idx - 1];
                    } else {
                        printf("Invalid index. Use 1-%d. Press Enter...", DICE_COUNT);
                        char *tmp = fgets(line, sizeof line, stdin);
                        (void)tmp;
                    }
                }
                // stay in the loop without rolling yet
                continue;
            }

            if (p[0] == 'r' || p[0] == 'R') {
                if (rolls < 3) {
                    roll_dice(dice, held);
                    ++rolls;
                } else {
                    printf("No rerolls left. Press Enter...");
                    char *tmp = fgets(line, sizeof line, stdin);
                    (void)tmp;
                }
                continue;
            }

            if (p[0] == 's' || p[0] == 'S') {
                // Show category menu with candidate scores
                printf(ANSI_CLEAR);
                printf(ANSI_BOLD "Choose a category:" ANSI_RESET "\n\n");
                for (int ci = 0; ci < CATEGORY_COUNT; ++ci) {
                    printf("%2d) %-17s ", ci + 1, CATEGORY_NAMES[ci]);
                    if (used[ci]) {
                        printf(ANSI_DIM "(USED: %d)" ANSI_RESET, scores[ci]);
                    } else {
                        int cand = compute_category_score(ci, dice);
                        printf("candidate: %d", cand);
                    }
                    printf("\n");
                }
                printf("\nEnter category number (1-%d), or 'b' to go back: ", CATEGORY_COUNT);
                if (!fgets(line, sizeof line, stdin)) {
                    printf("\nInput error. Exiting.\n");
                    return 1;
                }
                // Back
                if (line[0] == 'b' || line[0] == 'B') {
                    continue; // back to turn UI without scoring
                }
                int pick = 0;
                if (sscanf(line, "%d", &pick) == 1) {
                    if (pick >= 1 && pick <= CATEGORY_COUNT) {
                        int idx = pick - 1;
                        if (used[idx]) {
                            printf("\nCategory already used. Press Enter...");
                            char *tmp2 = fgets(line, sizeof line, stdin);
                            (void)tmp2;
                            continue;
                        }
                        int scored = compute_category_score(idx, dice);
                        used[idx] = 1;
                        scores[idx] = scored;
                        total += scored;
                        printf("\nScored %d points in %s. Total = %d. Press Enter to continue...", scored, CATEGORY_NAMES[idx], total);
                        char *tmp = fgets(line, sizeof line, stdin);
                        (void)tmp;
                        break; // end of turn
                    } else {
                        printf("\nInvalid category. Press Enter...");
                        char *tmp = fgets(line, sizeof line, stdin);
                        (void)tmp;
                        continue;
                    }
                } else {
                    printf("\nInvalid input. Press Enter...");
                    char *tmp = fgets(line, sizeof line, stdin);
                    (void)tmp;
                    continue;
                }
            }

            // Unknown command
            printf("Unknown command. Use h <1-5>, r, s, or q. Press Enter...");
            char *tmp = fgets(line, sizeof line, stdin);
            (void)tmp;
        }
    }

    // End of game summary
    printf(ANSI_CLEAR);
    printf(ANSI_BOLD "Game over!" ANSI_RESET " Final total: %d\n\n", total);
    printf(ANSI_BOLD "Score breakdown:" ANSI_RESET "\n");
    for (int ci = 0; ci < CATEGORY_COUNT; ++ci) {
        printf("- %-17s : %d\n", CATEGORY_NAMES[ci], scores[ci] < 0 ? 0 : scores[ci]);
    }
    printf("\nThanks for playing!\n");
    return 0;
}
