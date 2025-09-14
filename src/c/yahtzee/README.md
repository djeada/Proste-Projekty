# Yahtzee (C)

A simple terminal implementation of the classic Yahtzee dice game in C.

## Features
- Roll 5 dice, up to 3 times per turn
- Toggle holds on individual dice and reroll selectively
- Clear, readable terminal UI with indices and held markers
- Full category selection with per-category scoring and end-of-game breakdown
- Clean code structure and unit tests

## Build & Run

### Locally
```sh
mkdir -p build
cd build
cmake ..
make
./main
```

### With Docker
```sh
docker build -t yahtzee .
docker run --rm -it yahtzee
```

## Test
```sh
cd build
ctest --output-on-failure
```

## Lint
```sh
clang-tidy src/*.c
```

## Format
```sh
clang-format -i src/*.c src/*.h
```

## How to Play
- On each turn, you get up to 3 rolls.
- The first roll happens automatically; then you can:
	- h <1-5> — toggle hold on a die by index (held dice won’t change on reroll)
	- r — reroll all non-held dice (up to 2 rerolls after the first roll)
	- s — open the scoring menu, pick a category to score this roll
	- q — quit the game

Held dice are shown with an asterisk, e.g. [5]*, and indices are displayed above the dice to make selection easy.

Note: The UI uses basic ANSI escape codes to clear and redraw the screen. If your terminal doesn’t support ANSI codes or you see garbled output, try using a different terminal emulator or run with Docker.
