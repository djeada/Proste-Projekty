# Snake Game in C

![Build Status](https://github.com/djeada/Proste-Projekty/actions/workflows/ci.yml/badge.svg)
![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)

## Overview
A simple implementation of the classic Snake game in C, using the ncurses library for terminal input/output. The player controls a snake, eating food to grow and avoiding collisions with walls or itself.

## Requirements
- CMake >= 3.10
- C compiler (e.g., gcc, clang)
- ncurses library
- clang-tidy, clang-format (optional)
- Docker (optional)

## Installation
Clone the repository and build the project:
```sh
git clone https://github.com/djeada/Proste-Projekty.git
cd Proste-Projekty/src/c/snake
cmake -S . -B build
cmake --build build
```

## Usage
Run the program:
```sh
./build/main
```

## Features
- Classic snake gameplay
- Terminal-based interface using ncurses
- Random food placement
- Game over on collision with self or wall

## Testing
Run unit tests using CTest:
```sh
cd build
ctest
```
Or run the test binary directly:
```sh
./build/test_snake
```

## Linting and Formatting
Check code quality and formatting:
```sh
clang-tidy src/*.c
clang-format -i src/*.c
```

## Deployment
Build and run the project in Docker:
```sh
docker build -t snake .
docker run snake
```

## Project Structure
```
snake/
├── src/
│   └── main.c
├── tests/
│   └── test_snake.c
├── CMakeLists.txt
├── Dockerfile
├── .clang-tidy
├── .clang-format
├── .editorconfig
├── .github/
│   └── workflows/
│       └── ci.yml
└── README.md
```

## License
This project is licensed under the [MIT License](https://github.com/djeada/Proste-Projekty/blob/main/LICENSE).
