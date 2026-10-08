#ifndef CAESAR_CIPHER_H
#define CAESAR_CIPHER_H

/* Shifts one letter by key. Other characters (and non-ASCII bytes) are returned unchanged. */
char caesar_shift_char(char c, int key);

/* Encrypts text in place. The key may be negative or larger than 25. */
void caesar_encrypt(char *text, int key);

/* Decrypts text in place with the given key. */
void caesar_decrypt(char *text, int key);

/* Decrypts text in place with the most likely key and returns that key. */
int caesar_crack(char *text);

#endif
