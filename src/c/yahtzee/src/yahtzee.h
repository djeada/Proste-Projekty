/* Rules of Yahtzee: dice, scoring, upper bonus, turns and rounds. No input or output here. */
#ifndef YAHTZEE_H
#define YAHTZEE_H

#include <stdbool.h>

#define NUM_DICE 5
#define NUM_CATEGORIES 13
#define MAX_PLAYERS 4
#define MAX_ROLLS 3
#define NUM_ROUNDS 13
#define UNUSED (-1)
#define UPPER_BONUS_LIMIT 63
#define UPPER_BONUS 35

typedef enum {
    ONES,
    TWOS,
    THREES,
    FOURS,
    FIVES,
    SIXES,
    THREE_OF_A_KIND,
    FOUR_OF_A_KIND,
    FULL_HOUSE,
    SMALL_STRAIGHT,
    LARGE_STRAIGHT,
    YAHTZEE,
    CHANCE
} Category;

/* Returns one face, 1 to 6. The game never calls rand() itself, so tests can script the dice. */
typedef int (*FaceRoller)(void *context);

typedef struct {
    int scores[NUM_CATEGORIES]; /* UNUSED or the score of that category */
} Card;

typedef struct {
    int num_players;
    Card cards[MAX_PLAYERS];
    int current; /* index of the player whose turn it is */
    int round;   /* 1 to NUM_ROUNDS; NUM_ROUNDS + 1 when the game is over */
    int dice[NUM_DICE];
    bool held[NUM_DICE];
    int rolls; /* rolls made in the current turn, 0 to MAX_ROLLS */
} Game;

const char *category_name(Category c);
int score_for(const int dice[NUM_DICE], Category c);

void game_init(Game *g, int num_players);
bool game_roll(Game *g, FaceRoller roll, void *context);
bool game_toggle_hold(Game *g, int die);
bool game_choose(Game *g, Category c);
bool game_is_over(const Game *g);
int game_winner(const Game *g);

int card_upper_total(const Card *card);
int card_upper_bonus(const Card *card);
int card_total(const Card *card);

#endif
