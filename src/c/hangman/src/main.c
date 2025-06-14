#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hangman.h"

#define MAX_TRIES 10
#define WORD_LIST_SIZE 5

const char* wordList[WORD_LIST_SIZE] = {"apple", "banana", "cherry", "date", "elderberry"};

int main() {
    char word[20];
    char display[20];
    int tries = MAX_TRIES;
    char guess;

    chooseWord(wordList, WORD_LIST_SIZE, word);

    for (int i = 0; word[i] != '\0'; i++) {
        display[i] = '_';
    }
    display[strlen(word)] = '\0';

    printf("Welcome to Hangman!\n");

    while (tries > 0) {
        printf("\n%s\n", display);
        printf("Guess a letter: ");
        scanf(" %c", &guess);

        if (isLetterInWord(guess, word)) {
            revealLetter(guess, word, display);
            if (isWordGuessed(display)) {
                printf("\nCongratulations! You guessed the word: %s\n", word);
                return 0;
            }
        } else {
            tries--;
            printf("Wrong guess. Tries left: %d\n", tries);
        }
    }

    printf("\nGame Over! The word was: %s\n", word);
    return 0;
}
