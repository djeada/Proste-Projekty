#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "caesar_cipher.h"

static void check_encrypt(const char *input, int key, const char *expected) {
    char buffer[256];
    strcpy(buffer, input);
    caesar_encrypt(buffer, key);
    assert(strcmp(buffer, expected) == 0);
}

static void check_round_trip(const char *input, int key) {
    char buffer[256];
    strcpy(buffer, input);
    caesar_encrypt(buffer, key);
    caesar_decrypt(buffer, key);
    assert(strcmp(buffer, input) == 0);
}

static void test_shift_char(void) {
    assert(caesar_shift_char('a', 3) == 'd');
    assert(caesar_shift_char('z', 1) == 'a');
    assert(caesar_shift_char('Z', 1) == 'A');
    assert(caesar_shift_char('x', -1) == 'w');
    assert(caesar_shift_char('a', -1) == 'z');
    assert(caesar_shift_char('!', 5) == '!');
}

static void test_encrypt_keeps_case_and_other_characters(void) {
    check_encrypt("Hello, World!", 3, "Khoor, Zruog!");
    check_encrypt("123 .,?", 5, "123 .,?");
    check_encrypt("\xc4\x85\xc5\xbc", 7, "\xc4\x85\xc5\xbc"); /* UTF-8 bytes of "ąż" */
}

static void test_negative_and_large_keys(void) {
    check_encrypt("abc", -1, "zab");
    check_encrypt("abc", 26, "abc");
    check_encrypt("abc", 27, "bcd");
    check_encrypt("abc", 52, "abc");
    check_encrypt("abc", -27, "zab");
    check_encrypt("abc", -2147483647 - 1, "cde");
}

static void test_decrypt(void) {
    char buffer[] = "Khoor, Zruog!";
    caesar_decrypt(buffer, 3);
    assert(strcmp(buffer, "Hello, World!") == 0);
}

static void test_round_trip(void) {
    const char *text = "The Quick Brown Fox, 2026!";
    int keys[] = {-30, -3, 0, 1, 7, 25, 52, 100};
    for (size_t i = 0; i < sizeof keys / sizeof keys[0]; i++) {
        check_round_trip(text, keys[i]);
    }
}

static void test_crack(void) {
    const char *original =
        "The quick brown fox jumps over the lazy dog and then runs away into the forest.";
    char buffer[256];
    strcpy(buffer, original);
    caesar_encrypt(buffer, 7);
    assert(caesar_crack(buffer) == 7);
    assert(strcmp(buffer, original) == 0);
}

static void test_crack_without_letters(void) {
    char buffer[] = "1234 !?";
    assert(caesar_crack(buffer) == 0);
    assert(strcmp(buffer, "1234 !?") == 0);
}

int main(void) {
    test_shift_char();
    test_encrypt_keeps_case_and_other_characters();
    test_negative_and_large_keys();
    test_decrypt();
    test_round_trip();
    test_crack();
    test_crack_without_letters();
    printf("All C tests passed.\n");
    return 0;
}
