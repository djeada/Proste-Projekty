#include "caesar.h"
#include <ctype.h>

char caesar_shift(char c, int shift) {
    if (isupper(c)) {
        return (c - 'A' + shift + 26) % 26 + 'A';
    } else if (islower(c)) {
        return (c - 'a' + shift + 26) % 26 + 'a';
    } else {
        return c;
    }
}

void caesarCipher(char* text, int shift, int direction) {
    int i = 0;
    shift = shift % 26;
    while (text[i] != '\0') {
        if (isalpha(text[i])) {
            char base = islower(text[i]) ? 'a' : 'A';
            text[i] = (text[i] - base + direction * shift + 26) % 26 + base;
        }
        i++;
    }
}
