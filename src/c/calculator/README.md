# Simple Calculator in C

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
This is a basic calculator program written in C that performs addition, subtraction, multiplication, and division. The program operates in a loop, continuously prompting the user to enter a mathematical expression or to quit. It can handle floating point and integer numbers, providing a simple and interactive command-line interface for basic arithmetic operations.

## Screenshots
![calculator](https://github.com/djeada/Proste-Projekty/assets/37275728/e62d057f-bb26-4409-8664-83e7323e1d86)

## Requirements
- CMake >= 3.10
- C compiler (e.g., gcc, clang)
- clang-tidy, clang-format (optional, for code quality)
- Docker (optional, for containerized builds)

## Installation
Clone the repository and build the project:
```sh
git clone https://github.com/djeada/Proste-Projekty.git
cd Proste-Projekty/src/c/calculator
cmake -S . -B build
cmake --build build
```

## Usage
Run the program:
```sh
./build/main
```
You will be prompted to enter an expression (e.g., `3.5 * 2`) or 'q' to quit.

## Features
- Supports addition, subtraction, multiplication, and division
- Handles floating point and integer calculations
- Input validation and error handling (division by zero, invalid operator)
- Continuous operation until the user decides to exit

## Testing
Run unit tests using CTest:
```sh
cd build
ctest
```
Or run the test binary directly:
```sh
./build/test_calculator
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
docker build -t calculator .
docker run calculator
```

## Project Structure
```
calculator/
├── src/
│   ├── main.c
│   ├── calculator.c
│   └── calculator.h
├── tests/
│   └── test_calculator.c
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
- Add support for more complex mathematical functions
- Add more unit tests
- Provide a library interface for external use
- Enhance the user interface

## Contributing
Contributions are welcome! Please open issues or pull requests for improvements, bug fixes, or new features.

## License
This project is licensed under the [MIT License](https://github.com/djeada/Proste-Projekty/blob/main/LICENSE).
