# Yahtzee (C)

A simple terminal implementation of the classic Yahtzee dice game in C.

## Features
- Roll 5 dice, up to 3 times per turn
- Score in all Yahtzee categories
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
