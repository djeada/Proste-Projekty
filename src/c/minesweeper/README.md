# Minesweeper (C)

A simple terminal implementation of the classic Minesweeper game in C.

## Features
- Configurable board size and mine count
- Uncover cells, flag mines
- Game over and win detection
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
docker build -t minesweeper .
docker run --rm -it minesweeper
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
