## About the Project

Desktop version of the popular dice game Yahtzee. The game is played by rolling five dice and scoring the roll in one of thirteen categories. The player with the highest score wins.

## Screenshots

![yahtzee](https://user-images.githubusercontent.com/37275728/194823845-3aea219e-10d3-4d09-bc36-0832e7e0a8f8.gif)

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
cd Proste-Projekty/src/python/yahtzee
```

3. Start the app:

```Bash
python src/main.py
```

## Gameplay

* There are at least 2 players in the game.
* The game is based on turns. Each turn consists of maximum 3 rolls of the dice.
* The player can choose which dice to roll again and which to keep.
* If the dice fit the category, the player can choose to score the roll in that category.
* If the dice doesn't fit any category, the player can cross out one of the categories.
* The game ends when all categories are scored or crossed out.
* The player with the highest score wins.

Categories:

* Ones: Get as many ones as possible.
* Twos: Get as many twos as possible.
* Threes: Get as many threes as possible.
* Fours: Get as many fours as possible.
* Fives: Get as many fives as possible.
* Sixes: Get as many sixes as possible.
* Three of a kind: Get three dice with the same number. Points are the sum all dice (not just the three of a kind).
* Four of a kind: Get four dice with the same number. Points are the sum all dice (not just the four of a kind).
* Full house: Get three of a kind and a pair, e.g. 1,1,3,3,3 or 3,3,3,6,6. Scores 25 points.
* Small straight: Get four sequential dice, 1,2,3,4 or 2,3,4,5 or 3,4,5,6. Scores 30 points.
* Large straight: Get five sequential dice, 1,2,3,4,5 or 2,3,4,5,6. Scores 40 points.
* Chance: You can put anything into chance, it's basically like a garbage can when you don't have anything else you can use the dice for. The score is simply the sum of the dice.
* YAHTZEE: Five of a kind. Scores 50 points. You can optionally get multiple Yahtzees, see below for details.

## UI elements


## Possible improvements

