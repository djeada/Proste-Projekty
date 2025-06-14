# Timer (C)

A simple terminal timer application written in C.

## Features
- Counts up in HH:MM:SS format
- Graceful exit on Ctrl+C
- Clean code structure and tests

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
docker build -t timer .
docker run --rm -it timer
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
