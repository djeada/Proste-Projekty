# Hangman Game in C
This is a simple implementation of the classic Hangman game in C. The player tries to guess a randomly selected word by inputting one letter at a time. The game continues until the player either guesses the word or runs out of attempts.

## How to Play
- The game will display a series of dashes, each representing a letter in the word.
- You will be prompted to guess one letter at a time.
- If your guess is correct, the letter will be revealed in its correct position(s) in the word.
- If your guess is incorrect, you lose one of your limited number of attempts.
- The game ends when you either guess the word correctly or exhaust all your attempts.

## Installation

### Compiling the Game
To compile the game, follow these steps:
1. Clone or download the repository to your local machine.
2. Navigate to the directory containing the `hangman.c` file.
3. Compile the program using GCC:

```
gcc main.c -o hangman
```

4. Run the program:
- On Linux or macOS: `./hangman`
- On Windows: `hangman.exe`

## Customization
- You can add more words to the game by modifying the `wordList` array in the `hangman.c` file.
