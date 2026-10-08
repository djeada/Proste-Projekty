# Tic-Tac-Toe (Python)

Tic-tac-toe ("Kółko i krzyżyk") in a tkinter window. Two people can share the mouse, or you can play X against a computer that uses minimax and never loses.

![Screenshot](screenshot.png)

## Features

- Two players on one computer, or a player against the computer.
- Wins are detected, the winning line is highlighted, and draws are reported.
- A score of X wins, O wins and draws is kept across rounds.
- New round, mode switch and quit buttons.

## How to play

1. Click an empty cell to place your mark. Empty cells show their number.
2. In two-player mode, X and O take turns. In vs-computer mode you are X and the computer answers at once with O.
3. The first player with three marks in a row wins, and that row turns yellow.
4. Press **New round** to play again; the score is kept.
5. Press **Mode** to switch between two players and vs computer. This also starts a new round.

## How it works

### Data structures

The rules live in `src/tic_tac_toe.py`. A board is a list of nine strings: `"X"`, `"O"` or `" "` (`EMPTY`), indexed row by row from 0 to 8. The eight winning lines are a tuple of tuples, `LINES`. `winning_line()` returns the first line that is filled with one mark, or `None`. `play()` returns a new list with the mark added, and never changes the list it was given. That makes the search easy to follow: each possible future is simply another list.

### Turns and the round

`src/main.py` has one `App` class. It keeps the board, whose turn it is, the mode and the score in attributes. `place()` puts a mark on the board, then either switches the turn or, when the round has ended, adds one to the right score. In vs-computer mode `on_click()` calls `best_move()` after the human's move.

### The event loop

tkinter calls `App.on_click()` each time a button is pressed, and `App.draw()` then updates the text and colors of all buttons and labels. `root.mainloop()` waits for events and does nothing else, so the window stays responsive.

### Win and draw

- `winner()` returns `"X"` or `"O"` when a line is complete, otherwise `None`.
- `is_draw()` is true when the board is full and there is no winner.

### Minimax, step by step

The computer looks ahead through every possible continuation of the game and chooses the move that leads to the best result for it. Each finished game is scored from the computer's point of view:

- a win scores `10 - depth`,
- a loss scores `depth - 10`,
- a draw scores `0`,

where `depth` is the number of moves played from the position being scored. Subtracting the depth makes a quick win score higher than a slow one, and a slow loss score higher than a fast one, so the computer finishes a won game quickly and delays a lost one.

The search alternates between two players. On the computer's turn it takes the **maximum** score of its moves; on the opponent's turn it takes the **minimum**, because the opponent picks the reply that is worst for the computer. Recursion ends when a game is over.

Example. X has cells 0 and 1 and threatens to win on cell 2. O (the computer) has cells 4 and 8. It is O's turn:

```
X X 2        (empty cells are shown by their number)
3 O 5
6 7 O
```

1. The legal moves for O are 2, 3, 5, 6 and 7.
2. Move 3 (or 5, 6, 7) does not stop X. X plays 2 and wins at once. The win is found at depth 2, so the score is `2 - 10 = -8`.
3. Move 2 blocks the threat, and it also creates two threats for O: 6 (cells 2-4-6) and 5 (cells 2-5-8). X can stop only one of them, so O wins on its next move. That win is found at depth 3, so the score is `10 - 3 = 7`.
4. O compares the scores 7, -8, -8, -8, -8 and picks the maximum: cell 2.

The computer therefore never loses a game it can avoid losing. Two computers playing each other always end in a draw, which the tests check.

`best_move()` gives `max()` a key function that scores each legal move with `_score()`. `max()` returns the first best move, so ties go to the lowest cell number.

## Project layout

```
requirements.txt           pytest, for the tests
pyproject.toml             tells pytest where the logic is
.flake8                    line length for flake8
src/tic_tac_toe.py         the rules: board, legal moves, winner, draw, minimax (no GUI)
src/main.py                the tkinter window
tests/test_tic_tac_toe.py  tests of the rules
```

## Requirements

- Python 3.8 or newer
- tkinter, which comes with most Python installers. On Debian or Ubuntu install it with `sudo apt install python3-tk`.

## Run

```sh
python3 src/main.py
```

## Test

```sh
pip install -r requirements.txt
pytest
```

The tests use only the logic module. They check the win and draw rules, illegal moves, the computer taking an immediate win, the computer blocking an immediate loss, and two computers always drawing.

## Comparison with the other versions

- [C (terminal, ncurses)](../../c/tic_tac_toe)
- [Python (tkinter window)](../../python/tic_tac_toe)
- [JavaScript (browser page)](../../vanilla_js/tic_tac_toe)

| | C | Python | JavaScript |
|---|---|---|---|
| Interface | ncurses terminal | tkinter window | browser page |
| Lines of logic | 142 | 53 | 82 |
| Lines of interface | 163 | 73 | 68 |
| Tests | 10 | 17 | 9 |

The rules and the minimax search are the same algorithm in all three versions, so the tests check the same positions. C stores the board in a fixed struct of nine values and copies it by value inside the search, so no memory is allocated; the winning line is a pointer into a static table. Python and JavaScript write each new position as a new list or array, which reads more like the rules but allocates a little more. The terminal version polls keys in a loop, while the other two react to button callbacks.

## Ideas for extensions

- Add a difficulty level that sometimes chooses a random legal move.
- Let the computer start every other round, so the score is fairer.
- Save the score to a file and load it at the next start.
- Highlight the cell the computer just chose, or show the moves of the last round.
