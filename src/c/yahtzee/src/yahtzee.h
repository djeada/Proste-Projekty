#ifndef YAHTZEE_H
#define YAHTZEE_H

#define DICE_COUNT 5
#define CATEGORY_COUNT 13

void roll_dice(int dice[DICE_COUNT], int held[DICE_COUNT]);
int score_upper(int dice[DICE_COUNT], int face);
int score_three_of_a_kind(int dice[DICE_COUNT]);
int score_four_of_a_kind(int dice[DICE_COUNT]);
int score_full_house(int dice[DICE_COUNT]);
int score_small_straight(int dice[DICE_COUNT]);
int score_large_straight(int dice[DICE_COUNT]);
int score_yahtzee(int dice[DICE_COUNT]);
int score_chance(int dice[DICE_COUNT]);
void print_dice(const int dice[DICE_COUNT], const int held[DICE_COUNT]);

#endif // YAHTZEE_H
