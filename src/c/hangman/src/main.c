#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TRIES 10
#define WORD_LIST_SIZE 5

// List of words to choose from
const char* wordList[WORD_LIST_SIZE] = {"apple", "banana", "cherry", "date", "elderberry"};

void chooseWord(const char* list[], int listSize, char* word) {
    srand(time(NULL)); // Seed for random number generator
    int index = rand() % listSize;
    strcpy(word, list[index]);
}

int isLetterInWord(char letter, const char* word) {
    for (int i = 0; word[i] != '\0'; i++) {
        if (word[i] == letter) {
            return 1;
        }
    }
    return 0;
}

void revealLetter(char letter, const char* word, char* display) {
    for (int i = 0; word[i] != '\0'; i++) {
        if (word[i] == letter) {
            display[i] = letter;
        }
    }
}

int isWordGuessed(const char* display) {
    for (int i = 0; display[i] != '\0'; i++) {
        if (display[i] == '_') {
            return 0;
        }
    }
    return 1;
}

int main() {
    char word[20];
    char display[20];
    int tries = MAX_TRIES;
    char guess;

    chooseWord(wordList, WORD_LIST_SIZE, word);

    // Initialize display with dashes
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
