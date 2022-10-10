## About the Project

Hangman is a single-player guessing game. The computer chooses a word at random from a predefined list. The plaer makes an attempt to guess it by suggesting letters within a certain number of guesses.

## Screenshots

![hangman](https://user-images.githubusercontent.com/37275728/194822831-d1b117cb-ae01-4939-bac1-85ac4e58769a.gif)

## Requirements

To run this project locally you will need:

* Python 3.8+

No additional libraries or packagaes are needed!

## Installation

1. Download the code repository from GitHub: 
    
```Bash
git clone https://github.com/djeada/Proste-Projekty.git
```

2. Navigate to the appropriate directory:

```Bash
cd Proste-Projekty/src/python/calculator
```

3. Start the app:

```Bash
python src/main.py
```

## Rules of the game

1. The word is chosen randomly from the list of words.
2. The player has to guess the word.
3. There are n squares drawn on the board, where n is the length of the word.
4. When the player guesses a letter, the squares with the letter are revealed.
5. If the player guesses the word, he wins.
6. If the player guesses a letter that is not in the word, an image from a list of images is drawn.

## Features

* Single player mode.
* A display showing the total number of characters and letters that were successfully predicted. 
* An image representing the current state of the hangman.
* An input box in which the user can type the guessed letters. 

## Possible improvements

There are many ways in which the game could be expanded or improved in the future. Some of the ideas include:

* Add mode for two players.
* Allow the user to upload a list of possible words.
