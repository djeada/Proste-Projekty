#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "../src/caesar.h"

void test_caesar_shift() {
    assert(caesar_shift('A', 3) == 'D');
    assert(caesar_shift('Z', 1) == 'A');
    assert(caesar_shift('a', 2) == 'c');
    assert(caesar_shift('z', 1) == 'a');
    assert(caesar_shift('!', 5) == '!');
}

void test_caesarCipher_encrypt() {
    char text[] = "abc XYZ";
    caesarCipher(text, 3, 1);
    assert(strcmp(text, "def ABC") == 0);
}

void test_caesarCipher_decrypt() {
    char text[] = "def ABC";
    caesarCipher(text, 3, -1);
    assert(strcmp(text, "abc XYZ") == 0);
}

int main() {
    test_caesar_shift();
    test_caesarCipher_encrypt();
    test_caesarCipher_decrypt();
    printf("All tests passed!\n");
    return 0;
}
