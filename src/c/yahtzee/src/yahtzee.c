#include "yahtzee.h"

static const char *const NAMES[NUM_CATEGORIES] = {
    "Ones",       "Twos",           "Threes",          "Fours",
    "Fives",      "Sixes",          "Three of a Kind", "Four of a Kind",
    "Full House", "Small Straight", "Large Straight",  "Yahtzee",
    "Chance",
};

/* counts[face] is how many dice show that face (faces are 1 to 6). */
static void count_faces(const int dice[NUM_DICE], int counts[7]) {
    for (int face = 0; face < 7; face++) {
        counts[face] = 0;
    }
    for (int i = 0; i < NUM_DICE; i++) {
        counts[dice[i]]++;
    }
}

/* True when every face from start to start + length - 1 shows at least once. */
static bool has_run(const int counts[7], int start, int length) {
    for (int face = start; face < start + length; face++) {
        if (counts[face] == 0) {
            return false;
        }
    }
    return true;
}

static int most_of_a_kind(const int counts[7]) {
    int most = 0;
    for (int face = 1; face <= 6; face++) {
        if (counts[face] > most) {
            most = counts[face];
        }
    }
    return most;
}

static bool has_count(const int counts[7], int wanted) {
    for (int face = 1; face <= 6; face++) {
        if (counts[face] == wanted) {
            return true;
        }
    }
    return false;
}

static int sum_of_dice(const int dice[NUM_DICE]) {
    int sum = 0;
    for (int i = 0; i < NUM_DICE; i++) {
        sum += dice[i];
    }
    return sum;
}

const char *category_name(Category c) { return NAMES[c]; }

int score_for(const int dice[NUM_DICE], Category c) {
    int counts[7];
    count_faces(dice, counts);

    if (c <= SIXES) {
        return counts[c + 1] * (c + 1);
    }
    switch (c) {
        case THREE_OF_A_KIND:
            return most_of_a_kind(counts) >= 3 ? sum_of_dice(dice) : 0;
        case FOUR_OF_A_KIND:
            return most_of_a_kind(counts) >= 4 ? sum_of_dice(dice) : 0;
        case FULL_HOUSE:
            return has_count(counts, 3) && has_count(counts, 2) ? 25 : 0;
        case SMALL_STRAIGHT:
            return has_run(counts, 1, 4) || has_run(counts, 2, 4) || has_run(counts, 3, 4) ? 30 : 0;
        case LARGE_STRAIGHT:
            return has_run(counts, 1, 5) || has_run(counts, 2, 5) ? 40 : 0;
        case YAHTZEE:
            return most_of_a_kind(counts) == NUM_DICE ? 50 : 0;
        case CHANCE:
            return sum_of_dice(dice);
        default:
            return 0;
    }
}

static void start_turn(Game *g) {
    for (int i = 0; i < NUM_DICE; i++) {
        g->dice[i] = 0;
        g->held[i] = false;
    }
    g->rolls = 0;
}

void game_init(Game *g, int num_players) {
    g->num_players = num_players;
    for (int p = 0; p < MAX_PLAYERS; p++) {
        for (int c = 0; c < NUM_CATEGORIES; c++) {
            g->cards[p].scores[c] = UNUSED;
        }
    }
    g->current = 0;
    g->round = 1;
    start_turn(g);
}

bool game_is_over(const Game *g) { return g->round > NUM_ROUNDS; }

/* Rolls every die that is not held. */
bool game_roll(Game *g, FaceRoller roll, void *context) {
    if (game_is_over(g) || g->rolls >= MAX_ROLLS) {
        return false;
    }
    for (int i = 0; i < NUM_DICE; i++) {
        if (!g->held[i]) {
            g->dice[i] = roll(context);
        }
    }
    g->rolls++;
    return true;
}

/* die is an index from 0 to NUM_DICE - 1. Needs at least one roll in this turn. */
bool game_toggle_hold(Game *g, int die) {
    if (game_is_over(g) || g->rolls == 0 || die < 0 || die >= NUM_DICE) {
        return false;
    }
    g->held[die] = !g->held[die];
    return true;
}

bool game_choose(Game *g, Category c) {
    if (game_is_over(g) || g->rolls == 0 || g->cards[g->current].scores[c] != UNUSED) {
        return false;
    }
    g->cards[g->current].scores[c] = score_for(g->dice, c);

    start_turn(g);
    g->current++;
    if (g->current == g->num_players) {
        g->current = 0;
        g->round++;
    }
    return true;
}

int card_upper_total(const Card *card) {
    int total = 0;
    for (int c = ONES; c <= SIXES; c++) {
        if (card->scores[c] != UNUSED) {
            total += card->scores[c];
        }
    }
    return total;
}

int card_upper_bonus(const Card *card) {
    return card_upper_total(card) >= UPPER_BONUS_LIMIT ? UPPER_BONUS : 0;
}

int card_total(const Card *card) {
    int total = card_upper_total(card) + card_upper_bonus(card);
    for (int c = THREE_OF_A_KIND; c <= CHANCE; c++) {
        if (card->scores[c] != UNUSED) {
            total += card->scores[c];
        }
    }
    return total;
}

/* Ties go to the player who comes first. */
int game_winner(const Game *g) {
    int best = 0;
    for (int p = 1; p < g->num_players; p++) {
        if (card_total(&g->cards[p]) > card_total(&g->cards[best])) {
            best = p;
        }
    }
    return best;
}
