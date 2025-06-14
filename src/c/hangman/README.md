# Hangman Game in C

![Build Status](https://github.com/djeada/Proste-Projekty/actions/workflows/ci.yml/badge.svg)
![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)

## Table of Contents
- [Project Overview](#project-overview)
- [Screenshots](#screenshots)
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
This is a simple implementation of the classic Hangman game in C. The player tries to guess a randomly selected word by inputting one letter at a time. The game continues until the player either guesses the word or runs out of attempts.

## Screenshots
![hangman](https://github.com/djeada/Proste-Projekty/assets/37275728/1ce340f4-9efc-423e-a65d-7ee9ac905a9f)

## Requirements
- CMake >= 3.10
- C compiler (e.g., gcc, clang)
- clang-tidy, clang-format (optional, for code quality)
- Docker (optional, for containerized builds)

## Installation
Clone the repository and build the project:
```sh
git clone https://github.com/djeada/Proste-Projekty.git
cd Proste-Projekty/src/c/hangman
cmake -S . -B build
cmake --build build
```

## Usage
Run the program:
```sh
./build/main
```
You will be prompted to guess letters to find the hidden word.

## Features
- Randomly selects a word from a predefined list
- User guesses one letter at a time
- Tracks number of attempts left
- Displays current progress after each guess
- Ends when the word is guessed or attempts run out

## Testing
Run unit tests using CTest:
```sh
cd build
ctest
```
Or run the test binary directly:
```sh
./build/test_hangman
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
docker build -t hangman .
docker run hangman
```

## Project Structure
```
hangman/
├── src/
│   ├── main.c
│   ├── hangman.c
│   └── hangman.h
├── tests/
│   └── test_hangman.c
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
- Add support for custom word lists (from file)
- Add more unit tests
- Provide a library interface for external use
- Enhance the user interface

## Contributing
Contributions are welcome! Please open issues or pull requests for improvements, bug fixes, or new features.

## License
This project is licensed under the [MIT License](https://github.com/djeada/Proste-Projekty/blob/main/LICENSE).
