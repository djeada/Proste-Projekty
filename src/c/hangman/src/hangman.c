/* Rules of hangman: choosing a word, applying guesses and the game state. */
#include <ctype.h>
#include <string.h>

#include "hangman.h"

const Entry word_list[] = {
    {"Animal", "elephant"}, {"Animal", "giraffe"},  {"Animal", "penguin"}, {"Animal", "kangaroo"},
    {"Animal", "dolphin"},  {"Fruit", "banana"},    {"Fruit", "cherry"},   {"Fruit", "orange"},
    {"Fruit", "mango"},     {"Fruit", "lemon"},     {"Country", "canada"}, {"Country", "norway"},
    {"Country", "brazil"},  {"Country", "japan"},   {"Country", "egypt"},  {"Coding", "python"},
    {"Coding", "compiler"}, {"Coding", "function"}, {"Coding", "keyboard"}, {"Coding", "variable"},
};
const size_t word_list_size = sizeof word_list / sizeof word_list[0];

const Entry *choose_entry(const Entry list[], size_t count, unsigned random_value)
{
    return &list[random_value % count];
}

void game_start(Game *game, const Entry *entry)
{
    memset(game, 0, sizeof *game);
    game->category = entry->category;
    game->word = entry->word;
}

GuessResult game_guess(Game *game, char letter)
{
    if (!isalpha((unsigned char)letter)) {
        return GUESS_INVALID;
    }
    letter = (char)tolower((unsigned char)letter);
    if (game->guessed[letter - 'a']) {
        return GUESS_REPEAT;
    }
    game->guessed[letter - 'a'] = true;
    if (strchr(game->word, letter) != NULL) {
        return GUESS_HIT;
    }
    game->misses++;
    return GUESS_MISS;
}

void game_masked(const Game *game, char *out)
{
    size_t length = strlen(game->word);

    for (size_t i = 0; i < length; i++) {
        char letter = game->word[i];
        out[2 * i] = game_is_guessed(game, letter) ? letter : '_';
        out[2 * i + 1] = i + 1 < length ? ' ' : '\0';
    }
}

GameStatus game_status(const Game *game)
{
    if (game->misses >= MAX_MISSES) {
        return GAME_LOST;
    }
    for (const char *c = game->word; *c != '\0'; c++) {
        if (!game_is_guessed(game, *c)) {
            return GAME_PLAYING;
        }
    }
    return GAME_WON;
}

bool game_is_guessed(const Game *game, char letter)
{
    return game->guessed[letter - 'a'];
}
