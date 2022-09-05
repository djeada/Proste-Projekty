## About the Project

## Screenshots

![yahtzee](https://user-images.githubusercontent.com/37275728/188334918-95a7385c-3d10-4613-ae06-f0afafd0874e.gif)

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
cd Proste-Projekty/projekty/python/yahtzee
```

3. Start the app:

```Bash
python src/main.py
```

## Gameplay

In each turn a player may throw the dice up to three times. A player doesn't have to roll all five dice on the second and third throw of a round, he may put as many dice as he wants to the side and only throw the ones that don't have the numbers he's trying to get. For example, a player throws and gets 1,3,3,4,6. He decides he want to try for the large straight, 1,2,3,4,5. So, he puts 1,3,4 to the side and only throws 3 and 6 again, hoping to get 2 and 5.

In this game you click on the dice you want to keep. They will be moved down and will not be thrown the next time you press the 'Roll Dice' button. If you decide after the second throw in a turn that you don't want to keep the same dice before the third throw then you can click them again and they will move back to the table and be thrown in the third throw.
Upper section combinations

    Ones: Get as many ones as possible.
    Twos: Get as many twos as possible.
    Threes: Get as many threes as possible.
    Fours: Get as many fours as possible.
    Fives: Get as many fives as possible.
    Sixes: Get as many sixes as possible.

For the six combinations above the score for each of them is the sum of dice of the right kind. E.g. if you get 1,3,3,3,5 and you choose Threes you will get 3*3 = 9 points. The sum of all the above combinations is calculated and if it is 63 or more, the player will get a bonus of 35 points. On average a player needs three of each to reach 63, but it is not required to get three of each exactly, it is perfectly OK to have five sixes, and zero ones for example, as long as the sum is 63 or more the bonus will be awarded.
Lower section combinations

    Three of a kind: Get three dice with the same number. Points are the sum all dice (not just the three of a kind).
    Four of a kind: Get four dice with the same number. Points are the sum all dice (not just the four of a kind).
    Full house: Get three of a kind and a pair, e.g. 1,1,3,3,3 or 3,3,3,6,6. Scores 25 points.
    Small straight: Get four sequential dice, 1,2,3,4 or 2,3,4,5 or 3,4,5,6. Scores 30 points.
    Large straight: Get five sequential dice, 1,2,3,4,5 or 2,3,4,5,6. Scores 40 points.
    Chance: You can put anything into chance, it's basically like a garbage can when you don't have anything else you can use the dice for. The score is simply the sum of the dice.
    YAHTZEE: Five of a kind. Scores 50 points. You can optionally get multiple Yahtzees, see below for details.

Multiple Yahtzees

The rules around multiple Yahtzees are a bit complex. There are a couple of different cases:

    You already have a Yahtzee: You get a 100 bonus points in the Yahtzee box, but you also have a joker, which means that you can choose another move for the Yahtzee you just got. If the number you got yahtzees with has not been filled out in the upper section, then you must choose that. E.g. if you get an additional Yahtzee with 2's, and you haven't filled out the 2's in the upper section then you must choose that, and get 10 points for it. If the upper section box is already filled then you can choose any of the lower region boxes, and they will be scored as normal. Yahtzee is a superset of 3 of a kind, 4 of a kind, full house and chance, but you can also choose small or large straight and will get the normal 30 and 40 points for those.
    You've already put 0 in the Yahtzee box: In this case you get no 100 point bonus, but you do get a joker, and can choose your move following the rules described above for jokers. 


## UI elements


## Possible improvements

