/* Rules of hangman: choosing a word, applying guesses and the game state. */
#ifndef HANGMAN_H
#define HANGMAN_H

#include <stdbool.h>
#include <stddef.h>

#define MAX_MISSES 6
#define MAX_WORD_LEN 16

typedef struct {
    const char *category;
    const char *word;
} Entry;

typedef enum { GUESS_HIT, GUESS_MISS, GUESS_REPEAT, GUESS_INVALID } GuessResult;
typedef enum { GAME_PLAYING, GAME_WON, GAME_LOST } GameStatus;

typedef struct {
    const char *category;
    const char *word;
    bool guessed[26];
    int misses;
} Game;

extern const Entry word_list[];
extern const size_t word_list_size;

/* Picks an entry; random_value comes from the caller's random source. */
const Entry *choose_entry(const Entry list[], size_t count, unsigned random_value);

void game_start(Game *game, const Entry *entry);
GuessResult game_guess(Game *game, char letter);
/* Writes e.g. "_ a _ a _ a" into out, which needs 2 * MAX_WORD_LEN bytes. */
void game_masked(const Game *game, char *out);
GameStatus game_status(const Game *game);
bool game_is_guessed(const Game *game, char letter);

#endif
