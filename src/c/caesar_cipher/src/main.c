/* Command line interface: caesar_cipher encrypt|decrypt KEY TEXT, or caesar_cipher crack TEXT. */
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "caesar_cipher.h"

#define MAX_TEXT 1024

static void print_usage(void) {
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "  caesar_cipher encrypt KEY TEXT\n");
    fprintf(stderr, "  caesar_cipher decrypt KEY TEXT\n");
    fprintf(stderr, "  caesar_cipher crack TEXT\n");
}

static int parse_key(const char *s, int *key) {
    char *end;
    long value = strtol(s, &end, 10);
    if (*s == '\0' || *end != '\0' || value < INT_MIN || value > INT_MAX) {
        return 0;
    }
    *key = (int)value;
    return 1;
}

/* Copies the text argument into a buffer that the cipher functions can change. */
static int copy_text(char *buffer, const char *text) {
    if (strlen(text) >= MAX_TEXT) {
        fprintf(stderr, "The text is too long (at most %d bytes).\n", MAX_TEXT - 1);
        return 0;
    }
    strcpy(buffer, text);
    return 1;
}

int main(int argc, char *argv[]) {
    char text[MAX_TEXT];
    int key;

    if (argc == 4 && (strcmp(argv[1], "encrypt") == 0 || strcmp(argv[1], "decrypt") == 0)) {
        if (!parse_key(argv[2], &key)) {
            fprintf(stderr, "The key must be a whole number.\n");
            return 1;
        }
        if (!copy_text(text, argv[3])) {
            return 1;
        }
        if (strcmp(argv[1], "encrypt") == 0) {
            caesar_encrypt(text, key);
        } else {
            caesar_decrypt(text, key);
        }
        puts(text);
        return 0;
    }

    if (argc == 3 && strcmp(argv[1], "crack") == 0) {
        if (!copy_text(text, argv[2])) {
            return 1;
        }
        key = caesar_crack(text);
        printf("Most likely key: %d\n", key);
        puts(text);
        return 0;
    }

    print_usage();
    return 1;
}
