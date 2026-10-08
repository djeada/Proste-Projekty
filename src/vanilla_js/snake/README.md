# Snake (JavaScript)

Snake in the browser: you steer a snake around a board on a canvas, eat the food to grow and score points, and the game ends when the snake hits a wall or its own body. There are no libraries and no build step. The rules are in one file that has no DOM code, so they are tested with Node.js.

![Screenshot](screenshot.png)

## Features

- A 20 x 15 board drawn on a canvas
- Arrow keys or W, A, S, D steer the snake
- The snake cannot reverse into itself
- Eating food makes the snake longer and adds 10 points
- Hitting the wall or the snake's own body ends the game
- The game gets faster as the snake grows
- P pauses, R or Space restarts after game over, and the New Game button starts a new game at any time

## How to play

| Key or button | Action |
|---|---|
| Arrow keys or W A S D | Turn the snake (it keeps moving on its own) |
| P | Pause or continue |
| R or Space | Start a new game (after game over) |
| New Game button | Start a new game at any time |

The head of the snake is light blue, the body is blue, and the food is red. The score is above the board.

## How it works

### Data

The board is 20 x 15 cells. A cell is an object `{ x, y }`. The snake is an array `body` of cells, where `body[0]` is the head. A direction is one of the constants `UP`, `DOWN`, `LEFT` and `RIGHT`, each an object such as `{ x: 1, y: 0 }`.

### One step of the game

`SnakeGame.step()` does the following:

1. Use the direction the player asked for (`nextDirection`) and work out the new head.
2. If the head is outside the board or on the snake's body, the game is over.
3. Otherwise put the new head at the front of the array with `unshift`. If it is the food, the score goes up by 10 and new food is placed, so the tail stays and the snake grows. If not, `pop` removes the tail.

### Turns

`turn()` stores the requested direction, but only if it is not the opposite of the last step. A turn from right to left is ignored, and two key presses between two steps are checked against the last step too, so the snake never runs into its neck.

### Timing and drawing

`scheduleStep()` sets a `setTimeout` for `game.delayMs()`, which gets shorter as the snake grows. `tick()` makes one step, redraws, and schedules the next one. `newGame()` draws the board at once, so the board is there as soon as the button is clicked, and does not wait for the first timer.

### Files

`src/snake.js` holds the rules and is loaded as a plain script, so `index.html` works when it is opened from disk (no modules, no server). At the end of the file, `module.exports` lets Node.js load the same code for the tests. `src/main.js` holds the drawing, the keys and the button.

## Project layout

```
src/index.html          the page: the canvas, the score, the button
src/style.css           the colors and the layout
src/snake.js            the rules: steps, turns, food and speed (no DOM)
src/main.js             the canvas, keyboard and button
tests/snake.test.js     tests of the rules, with node:test
package.json            the test script (npm test)
```

## Requirements

- A modern browser, such as Chrome, Firefox or Safari, to play
- Node.js 18 or newer, only to run the tests (there are no npm dependencies)

## Run

Open `src/index.html` in a browser. There is nothing to install or build.

## Test

```sh
npm test
```

The tests check the rules only: the start, moves, turns, walls, collisions with the body, eating, the random choice of food and the speed. They need no browser.

## Comparison with the other versions

- [C version](../../c/snake)
- [Python version](../../python/snake)

| | C | Python | JavaScript |
|---|---|---|---|
| Interface | ncurses terminal | pygame window | browser page |
| Lines of logic | 128 | 55 | 79 |
| Lines of interface | 121 | 75 | 74 |
| Tests | 9 | 13 | 13 |

The rules are the same in all three versions. In the browser nothing has to be installed, and the events come from the page: the timer is a `setTimeout` chain and the keys are handled by a `keydown` listener. The rules file is written as a plain script that also works as a Node.js module, so the same code is shared between the page and the tests without a bundler. Unlike C, there is no need to manage memory, and unlike Python, the random source is a function that is passed in with a default.

## Ideas for extensions

- Save the high score in `localStorage`
- Add touch buttons for phones
- Add obstacles that are placed at random on the board
- Let the walls wrap around, so the snake comes out on the other side
