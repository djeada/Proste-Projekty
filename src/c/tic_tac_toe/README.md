# Tic-Tac-Toe Game in C

![Build Status](https://github.com/djeada/Proste-Projekty/actions/workflows/ci.yml/badge.svg)
![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)

## Table of Contents
- [Project Overview](#project-overview)
- [Requirements](#requirements)
- [Installation](#installation)
- [Usage](#usage)
- [Features](#features)
- [Testing](#testing)
- [Linting and Formatting](#linting-and-formatting)
- [Deployment](#deployment)
- [Project Structure](#project-structure)
- [Possible Improvements](#possible-improvements)
- [Contributing](#contributing)
- [License](#license)

## Project Overview
This is a simple implementation of the classic Tic-Tac-Toe game in C. Players can play against each other or against a simple AI opponent. The game is played on a 3x3 board where players take turns placing their symbols (X or O).

## Requirements
- CMake >= 3.10
- C compiler (e.g., gcc, clang)
- clang-tidy, clang-format (optional, for code quality)
- Docker (optional, for containerized builds)

## Installation
Clone the repository and build the project:
```sh
git clone https://github.com/djeada/Proste-Projekty.git
cd Proste-Projekty/src/c/tic_tac_toe
cmake -S . -B build
cmake --build build
```

## Usage
Run the program:
```sh
./build/main
```
You will be asked if you want to play against the AI. Then enter row and column numbers (0-2) to make your move.

## Features
- Two-player mode (local)
- Single-player mode against simple AI
- 3x3 game board
- Win detection (rows, columns, diagonals)
- Draw detection
- Input validation

## Testing
Run unit tests using CTest:
```sh
cd build
ctest
```
Or run the test binary directly:
```sh
./build/test_tic_tac_toe
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
docker build -t tic_tac_toe .
docker run -it tic_tac_toe
```

## Project Structure
```
tic_tac_toe/
├── src/
│   ├── main.c
│   ├── tic_tac_toe.c
│   └── tic_tac_toe.h
├── tests/
│   └── test_tic_tac_toe.c
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

## Possible Improvements
- Implement minimax algorithm for unbeatable AI
- Add network multiplayer support
- Create a graphical interface using ncurses
- Add game statistics and scoreboard

## Contributing
Contributions are welcome! Please open issues or pull requests for improvements, bug fixes, or new features.

## License
This project is licensed under the [MIT License](https://github.com/djeada/Proste-Projekty/blob/main/LICENSE).
