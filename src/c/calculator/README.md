# Calculator in C

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
This is a calculator program written in C that evaluates mathematical expressions. It supports basic arithmetic, parentheses, exponentiation, and follows proper operator precedence. The calculator can handle both simple expressions like `3.5 * 2` and complex ones like `(2+3)*4^2`.

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

Run the calculator:
```sh
./build/calculator
```

The calculator accepts any mathematical expression:
- Simple: `3.5 * 2`, `10 + 5`
- Complex: `(2+3)*4`, `2^3`, `-5+2`
- Advanced: `2*3+4/2`, `(1+2)*(3+4)`

For help:
```sh
./build/calculator --help
```

## Features

- Mathematical expression parsing with proper operator precedence
- Support for basic arithmetic: `+`, `-`, `*`, `/`
- Exponentiation: `^`
- Parentheses for grouping expressions
- Unary minus operator
- Floating point calculations
- Comprehensive error reporting for invalid expressions and division by zero
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
│   ├── main.c          # Main program entry point
│   ├── parser.c        # Mathematical expression parser
│   ├── parser.h        # Parser function declarations
│   ├── repl.c          # Interactive calculator shell
│   └── repl.h          # REPL function declarations
├── tests/
│   └── test_calculator.c  # Unit tests
├── CMakeLists.txt      # Build configuration
├── Dockerfile          # Container build configuration
├── .clang-tidy         # Static analysis configuration
├── .clang-format       # Code formatting configuration
├── .editorconfig       # Editor configuration
├── .github/
│   └── workflows/
│       └── ci.yml      # CI/CD pipeline
└── README.md
```
│   └── workflows/
│       └── ci.yml      # CI/CD pipeline
└── README.md
```

### Architecture Overview

The calculator is designed with a modular architecture:

- **calculator.c/h**: Core arithmetic functions for basic operations (+, -, *, /)
- **parser.c/h**: Recursive descent parser for complex expressions with operator precedence
- **repl.c/h**: Interactive shell for advanced calculator mode
- **main.c**: Command-line interface that orchestrates both simple and advanced modes

## Possible Improvements
- Add support for mathematical functions (sin, cos, log, etc.)
- Add variables and memory functions
- Add history and recall functionality
- Provide a library interface for external use
- Add configuration file support
- Enhance error messages with position indicators

## Contributing
Contributions are welcome! Please open issues or pull requests for improvements, bug fixes, or new features.

## License
This project is licensed under the [MIT License](https://github.com/djeada/Proste-Projekty/blob/main/LICENSE).
