/* Tests of the logic in src/yahtzee.c. Returns 0 when every test passes. */
#undef NDEBUG
#include <assert.h>
#include <stdio.h>

#include "yahtzee.h"

typedef struct {
    const int *faces;
    int next;
} Script;

static int scripted_roll(void *context) {
    Script *script = context;
    return script->faces[script->next++];
}

static int always_six(void *context) {
    (void)context;
    return 6;
}

typedef struct {
    int dice[NUM_DICE];
    Category category;
    int expected;
} ScoreCase;

static void test_scores_for_each_category(void) {
    const ScoreCase cases[] = {
        {{3, 3, 3, 5, 5}, FULL_HOUSE, 25},     {{4, 4, 4, 4, 4}, FULL_HOUSE, 0},
        {{1, 2, 3, 4, 6}, SMALL_STRAIGHT, 30}, {{2, 3, 4, 5, 6}, LARGE_STRAIGHT, 40},
        {{1, 2, 3, 4, 6}, LARGE_STRAIGHT, 0},  {{4, 4, 4, 4, 4}, YAHTZEE, 50},
        {{4, 4, 4, 4, 5}, YAHTZEE, 0},         {{2, 2, 2, 5, 6}, THREE_OF_A_KIND, 17},
        {{2, 2, 3, 5, 6}, THREE_OF_A_KIND, 0}, {{2, 2, 2, 2, 6}, FOUR_OF_A_KIND, 14},
        {{2, 2, 2, 3, 6}, FOUR_OF_A_KIND, 0},  {{5, 5, 1, 2, 5}, FIVES, 15},
        {{3, 3, 1, 3, 6}, THREES, 9},          {{6, 6, 5, 5, 4}, CHANCE, 26},
        {{1, 1, 2, 3, 3}, SMALL_STRAIGHT, 0},
    };
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        assert(score_for(cases[i].dice, cases[i].category) == cases[i].expected);
    }
}

static void test_upper_bonus(void) {
    Card card;
    for (int c = 0; c < NUM_CATEGORIES; c++) {
        card.scores[c] = UNUSED;
    }
    /* 3 ones, 6 twos, 9 threes, 12 fours, 15 fives, 18 sixes = 63 */
    for (int face = 1; face <= 6; face++) {
        card.scores[face - 1] = 3 * face;
    }
    assert(card_upper_total(&card) == 63);
    assert(card_upper_bonus(&card) == 35);

    card.scores[SIXES] = 17;
    assert(card_upper_total(&card) == 62);
    assert(card_upper_bonus(&card) == 0);
}

static void test_card_total(void) {
    Card card;
    for (int c = 0; c < NUM_CATEGORIES; c++) {
        card.scores[c] = UNUSED;
    }
    assert(card_total(&card) == 0);

    card.scores[SIXES] = 30;
    card.scores[FULL_HOUSE] = 25;
    card.scores[CHANCE] = 22;
    assert(card_total(&card) == 77);

    for (int face = 0; face < 6; face++) {
        card.scores[face] = 3 * (face + 1);
    }
    assert(card_total(&card) == 63 + 35 + 25 + 22);
}

static void test_roll_keeps_held_dice(void) {
    Game game;
    game_init(&game, 1);
    const int first[] = {1, 2, 3, 4, 5};
    Script script = {first, 0};
    assert(game_roll(&game, scripted_roll, &script));
    assert(script.next == 5);

    assert(game_toggle_hold(&game, 0));
    assert(game_toggle_hold(&game, 2));
    const int second[] = {6, 6, 6};
    script = (Script){second, 0};
    assert(game_roll(&game, scripted_roll, &script));
    assert(script.next == 3);
    assert(game.dice[0] == 1 && game.dice[1] == 6 && game.dice[2] == 3);
    assert(game.dice[3] == 6 && game.dice[4] == 6);
}

static void test_at_most_three_rolls(void) {
    Game game;
    game_init(&game, 1);
    assert(game_roll(&game, always_six, NULL));
    assert(game_roll(&game, always_six, NULL));
    assert(game_roll(&game, always_six, NULL));
    assert(!game_roll(&game, always_six, NULL));
    assert(game.rolls == MAX_ROLLS);
}

static void test_must_roll_before_holding_or_choosing(void) {
    Game game;
    game_init(&game, 1);
    assert(!game_toggle_hold(&game, 0));
    assert(!game_choose(&game, CHANCE));
    assert(game.cards[0].scores[CHANCE] == UNUSED);
}

static void test_choose_ends_turn_and_rotates_players(void) {
    Game game;
    game_init(&game, 2);
    game_roll(&game, always_six, NULL);
    assert(game_choose(&game, SIXES));
    assert(game.cards[0].scores[SIXES] == 30);
    assert(game.current == 1);
    assert(game.rolls == 0);
    for (int i = 0; i < NUM_DICE; i++) {
        assert(game.dice[i] == 0 && !game.held[i]);
    }

    game_roll(&game, always_six, NULL);
    assert(game_choose(&game, CHANCE));
    assert(game.current == 0);
    assert(game.round == 2);
}

static void test_category_only_once_per_player(void) {
    Game game;
    game_init(&game, 2);
    game_roll(&game, always_six, NULL);
    assert(game_choose(&game, YAHTZEE));
    game_roll(&game, always_six, NULL);
    assert(game_choose(&game, CHANCE));

    /* Player 0 has already used YAHTZEE, so it cannot be scored again. */
    game_roll(&game, always_six, NULL);
    assert(!game_choose(&game, YAHTZEE));
    assert(game.current == 0);
    assert(game_choose(&game, SIXES));
    assert(game.cards[0].scores[YAHTZEE] == 50);
    assert(game.cards[0].scores[SIXES] == 30);
}

static void test_game_lasts_thirteen_rounds(void) {
    Game game;
    game_init(&game, 2);
    for (int turn = 0; turn < 2 * NUM_CATEGORIES; turn++) {
        assert(!game_is_over(&game));
        game_roll(&game, always_six, NULL);
        assert(game_choose(&game, (Category)(turn / 2)));
    }
    assert(game_is_over(&game));
    assert(game.round == NUM_ROUNDS + 1);
    assert(!game_roll(&game, always_six, NULL));
    assert(!game_choose(&game, CHANCE));
}

static void test_winner(void) {
    Game game;
    game_init(&game, 3);
    game.cards[0].scores[CHANCE] = 20;
    game.cards[1].scores[YAHTZEE] = 50;
    game.cards[2].scores[CHANCE] = 30;
    assert(game_winner(&game) == 1);

    game.cards[2].scores[YAHTZEE] = 50;
    assert(game_winner(&game) == 2);
}

int main(void) {
    test_scores_for_each_category();
    test_upper_bonus();
    test_card_total();
    test_roll_keeps_held_dice();
    test_at_most_three_rolls();
    test_must_roll_before_holding_or_choosing();
    test_choose_ends_turn_and_rotates_players();
    test_category_only_once_per_player();
    test_game_lasts_thirteen_rounds();
    test_winner();
    printf("All 10 yahtzee tests passed.\n");
    return 0;
}
