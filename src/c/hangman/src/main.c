/* Terminal user interface of hangman: draws the figure and reads letters from stdin. */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "hangman.h"

static void draw_figure(int misses)
{
    const char *head = misses >= 1 ? "O" : " ";
    const char *body = misses >= 2 ? "|" : " ";
    const char *arm_left = misses >= 3 ? "/" : " ";
    const char *arm_right = misses >= 4 ? "\\" : " ";
    const char *leg_left = misses >= 5 ? "/" : " ";
    const char *leg_right = misses >= 6 ? "\\" : " ";

    printf("  +---+\n");
    printf("  |   |\n");
    printf("  |   %s\n", head);
    printf("  |  %s%s%s\n", arm_left, body, arm_right);
    printf("  |  %s %s\n", leg_left, leg_right);
    printf("  |\n");
    printf("=========\n");
}

/* Clears the screen with ANSI codes and redraws the whole game. */
static void show(const Game *game, const char *message)
{
    char masked[2 * MAX_WORD_LEN];

    game_masked(game, masked);
    printf("\033[H\033[2J");
    draw_figure(game->misses);
    printf("\nCategory: %s\n", game->category);
    printf("Word: %s\n", masked);
    printf("Wrong guesses left: %d\n", MAX_MISSES - game->misses);
    printf("Guessed: ");
    for (char c = 'a'; c <= 'z'; c++) {
        if (game_is_guessed(game, c)) {
            printf("%c ", c);
        }
    }
    printf("\n%s\n", message);
}

/* Returns the first non-space character of the line, '\n' for an empty line, EOF at the end of input. */
static int read_key(void)
{
    int c;
    int key = '\n';
    int found = 0;

    while ((c = getchar()) != EOF && c != '\n') {
        if (!found && !isspace(c)) {
            key = c;
            found = 1;
        }
    }
    if (c == EOF && !found) {
        return EOF;
    }
    return key;
}

static const char *describe(GuessResult result)
{
    switch (result) {
    case GUESS_HIT:
        return "Good guess!";
    case GUESS_MISS:
        return "Not in the word.";
    case GUESS_REPEAT:
        return "You already guessed that letter.";
    case GUESS_INVALID:
    default:
        return "Please enter a letter from a to z.";
    }
}

static void play(Game *game)
{
    const char *message = "";

    while (game_status(game) == GAME_PLAYING) {
        int key;

        show(game, message);
        printf("Guess a letter: ");
        fflush(stdout);
        key = read_key();
        if (key == EOF) {
            printf("\n");
            exit(EXIT_SUCCESS);
        }
        message = describe(game_guess(game, (char)key));
    }
    show(game, "");
    if (game_status(game) == GAME_WON) {
        printf("You won! The word was '%s'.\n", game->word);
    } else {
        printf("You lost. The word was '%s'.\n", game->word);
    }
}

int main(void)
{
    Game game;
    int key;

    srand((unsigned)time(NULL));
    printf("Welcome to Hangman!\n");
    do {
        game_start(&game, choose_entry(word_list, word_list_size, (unsigned)rand()));
        play(&game);
        printf("Play again? (y/n): ");
        fflush(stdout);
        key = read_key();
    } while (key == 'y' || key == 'Y');
    return EXIT_SUCCESS;
}
