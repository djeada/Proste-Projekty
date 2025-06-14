#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "../src/hangman.h"

void test_isLetterInWord() {
    assert(isLetterInWord('a', "apple") == 1);
    assert(isLetterInWord('z', "apple") == 0);
}

void test_revealLetter() {
    char display[] = "_____";
    revealLetter('p', "apple", display);
    assert(strcmp(display, "_pp__") == 0);
}

void test_isWordGuessed() {
    assert(isWordGuessed("apple") == 1);
    assert(isWordGuessed("_pp__") == 0);
}

int main() {
    test_isLetterInWord();
    test_revealLetter();
    test_isWordGuessed();
    printf("All tests passed!\n");
    return 0;
}
