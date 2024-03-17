#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

int main() {
    char text[1024];
    int shift, option;

    printf("Enter text: ");
    fgets(text, sizeof(text), stdin);
    text[strcspn(text, "\n")] = 0; 

    printf("Enter shift key (integer): ");
    if(scanf("%d", &shift) != 1) {
        printf("Invalid input. Please enter an integer.\n");
        return 1;
    }

    printf("Choose (1) Encrypt or (2) Decrypt: ");
    if(scanf("%d", &option) != 1 || (option != 1 && option != 2)) {
        printf("Invalid choice. Please enter 1 or 2.\n");
        return 1;
    }

    caesarCipher(text, shift, option == 1 ? 1 : -1);
    printf("Processed text: %s\n", text);

    return 0;
}
