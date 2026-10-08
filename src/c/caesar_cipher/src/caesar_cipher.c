#include "caesar_cipher.h"

/* Relative frequency of a..z in English text, in percent. */
static const double english_frequency[26] = {
    8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015, 6.094, 6.966, 0.153, 0.772, 4.025, 2.406,
    6.749, 7.507, 1.929, 0.095, 5.987, 6.327, 9.056, 2.758, 0.978, 2.360, 0.150, 1.974, 0.074};

/* In C, % keeps the sign of the left operand (-1 % 26 == -1), so add 26 before taking the rest. */
static int normalize_key(int key) {
    return ((key % 26) + 26) % 26;
}

/* Index 0..25 of an ASCII letter (case ignored), or -1 for any other character. */
static int letter_index(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c - 'A';
    }
    if (c >= 'a' && c <= 'z') {
        return c - 'a';
    }
    return -1;
}

char caesar_shift_char(char c, int key) {
    int index = letter_index(c);
    if (index < 0) {
        return c;
    }
    char base = (c >= 'a') ? 'a' : 'A';
    return (char)(base + (index + normalize_key(key)) % 26);
}

void caesar_encrypt(char *text, int key) {
    for (char *p = text; *p != '\0'; p++) {
        *p = caesar_shift_char(*p, key);
    }
}

void caesar_decrypt(char *text, int key) {
    caesar_encrypt(text, -normalize_key(key));
}

/* Sum of English frequencies of the letters that appear when text is decrypted with key. */
static double score(const char *text, int key) {
    double total = 0.0;
    for (const char *p = text; *p != '\0'; p++) {
        int index = letter_index(*p);
        if (index >= 0) {
            total += english_frequency[(index + 26 - key) % 26];
        }
    }
    return total;
}

int caesar_crack(char *text) {
    int best_key = 0;
    double best_score = -1.0;
    for (int key = 0; key < 26; key++) {
        double s = score(text, key);
        if (s > best_score) {
            best_score = s;
            best_key = key;
        }
    }
    caesar_decrypt(text, best_key);
    return best_key;
}
