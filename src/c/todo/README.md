# Todo List (C)

A simple terminal todo list application written in C, using SQLite for persistent storage.

## Features
- Add, view, edit, remove tasks
- Change task priority
- Persistent storage with SQLite
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
docker build -t todo .
docker run --rm -it todo
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
