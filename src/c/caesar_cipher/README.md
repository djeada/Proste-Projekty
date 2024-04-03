# Caesar Cipher in C
This program is a simple implementation of the Caesar Cipher encryption technique in C. It allows the user to either encrypt or decrypt text using a specified shift key. The Caesar Cipher is one of the oldest known encryption techniques, and it's a great example to understand the basics of cryptography.

![cipher](https://github.com/djeada/Proste-Projekty/assets/37275728/6f0b1c2f-a948-44e1-96b0-1a1d4b279256)

## How to Use
- Compile and run the program.
- Enter the text you want to encrypt or decrypt.
- Enter the shift key (an integer value).
- Choose whether to encrypt (1) or decrypt (2) the text.
- The program will display the processed text based on your choices.

## Features
- Encrypts or decrypts text using the Caesar Cipher method.
- Handles both uppercase and lowercase characters.
- Ignores non-alphabetical characters (such as numbers or symbols).
- User can specify the magnitude and direction of the shift.

## Installation

### Compiling the Game
To compile the game, follow these steps:
1. Clone or download the repository to your local machine.
2. Navigate to the directory containing the `main.c` file.
3. Compile the program using GCC:

```
gcc src/main.c -o caesar_cipher
```

4. Run the program:
- On Linux or macOS: `./caesar_cipher`
- On Windows: `caesar_cipher.exe`

## Customization
- The shift logic can be modified to include or exclude certain characters or to handle different character sets.
- The user interface can be expanded to include file input/output for processing larger texts.
- Additional features like handling different languages or adding a graphical user interface can also be explored.
