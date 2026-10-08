/* Tests of the hangman rules. Returns 0 when all tests pass. */
#include <assert.h>
#include <string.h>

#include "hangman.h"

static void test_choose_entry(void)
{
    Entry list[] = {{"A", "one"}, {"B", "two"}, {"C", "three"}};

    assert(choose_entry(list, 3, 0) == &list[0]);
    assert(choose_entry(list, 3, 4) == &list[1]);
}

static void test_word_list_is_valid(void)
{
    for (size_t i = 0; i < word_list_size; i++) {
        size_t length = strlen(word_list[i].word);

        assert(length > 0 && length < MAX_WORD_LEN);
        for (size_t j = 0; j < length; j++) {
            assert(word_list[i].word[j] >= 'a' && word_list[i].word[j] <= 'z');
        }
    }
}

static void test_new_game_is_masked(void)
{
    Entry entry = {"Fruit", "lemon"};
    Game game;
    char masked[2 * MAX_WORD_LEN];

    game_start(&game, &entry);
    game_masked(&game, masked);
    assert(strcmp(masked, "_ _ _ _ _") == 0);
    assert(game_status(&game) == GAME_PLAYING);
}

static void test_hit_reveals_all_occurrences(void)
{
    Entry entry = {"Fruit", "banana"};
    Game game;
    char masked[2 * MAX_WORD_LEN];

    game_start(&game, &entry);
    assert(game_guess(&game, 'a') == GUESS_HIT);
    game_masked(&game, masked);
    assert(strcmp(masked, "_ a _ a _ a") == 0);
    assert(game.misses == 0);
}

static void test_miss_costs_one_attempt(void)
{
    Entry entry = {"Fruit", "banana"};
    Game game;

    game_start(&game, &entry);
    assert(game_guess(&game, 'z') == GUESS_MISS);
    assert(game.misses == 1);
}

static void test_repeated_guess_is_ignored(void)
{
    Entry entry = {"Fruit", "banana"};
    Game game;

    game_start(&game, &entry);
    assert(game_guess(&game, 'b') == GUESS_HIT);
    assert(game_guess(&game, 'q') == GUESS_MISS);
    assert(game_guess(&game, 'b') == GUESS_REPEAT);
    assert(game_guess(&game, 'q') == GUESS_REPEAT);
    assert(game.misses == 1);
}

static void test_invalid_and_uppercase_guesses(void)
{
    Entry entry = {"Fruit", "banana"};
    Game game;

    game_start(&game, &entry);
    assert(game_guess(&game, '1') == GUESS_INVALID);
    assert(game_guess(&game, ' ') == GUESS_INVALID);
    assert(game_guess(&game, 'B') == GUESS_HIT);
    assert(game_is_guessed(&game, 'b'));
    assert(game.misses == 0);
}

static void test_win(void)
{
    Entry entry = {"Fruit", "banana"};
    Game game;

    game_start(&game, &entry);
    game_guess(&game, 'b');
    game_guess(&game, 'a');
    assert(game_status(&game) == GAME_PLAYING);
    game_guess(&game, 'n');
    assert(game_status(&game) == GAME_WON);
}

static void test_lose_after_max_misses(void)
{
    Entry entry = {"Fruit", "banana"};
    Game game;
    const char *wrong = "cdefgh";

    game_start(&game, &entry);
    for (int i = 0; i < MAX_MISSES - 1; i++) {
        game_guess(&game, wrong[i]);
    }
    assert(game_status(&game) == GAME_PLAYING);
    game_guess(&game, wrong[MAX_MISSES - 1]);
    assert(game.misses == MAX_MISSES);
    assert(game_status(&game) == GAME_LOST);
}

int main(void)
{
    test_choose_entry();
    test_word_list_is_valid();
    test_new_game_is_masked();
    test_hit_reveals_all_occurrences();
    test_miss_costs_one_attempt();
    test_repeated_guess_is_ignored();
    test_invalid_and_uppercase_guesses();
    test_win();
    test_lose_after_max_misses();
    return 0;
}
