# Fifteen Puzzle (C)

A simple terminal implementation of the classic 15-puzzle game in C.

## Features
- 4x4 sliding puzzle
- Randomized, always solvable board
- Move tiles with keyboard
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
docker build -t fifteen_puzzle .
docker run --rm -it fifteen_puzzle
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
