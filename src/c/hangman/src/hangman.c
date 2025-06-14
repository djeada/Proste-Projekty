#include "hangman.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>

void chooseWord(const char* list[], int listSize, char* word) {
    srand(time(NULL));
    int index = rand() % listSize;
    // Use safe copy, assuming word buffer is at least 20 bytes (as in main.c)
    strncpy(word, list[index], 19);
    word[19] = '\0';
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
