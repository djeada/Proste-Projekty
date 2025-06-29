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
This is a calculator program written in C that provides both simple and advanced mathematical calculation capabilities. The program offers two modes: a simple mode for basic arithmetic operations and an advanced mode that supports complex expressions with parentheses, exponentiation, and operator precedence.

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

The calculator supports two modes:

### Advanced Mode (Default)
```sh
./build/calculator
# or explicitly:
./build/calculator advanced
```
Supports complex expressions like:
- `2+3*4` (follows operator precedence)
- `(2+3)*4` (parentheses)
- `2^3` (exponentiation)
- `-5+2` (unary operators)

### Simple Mode
```sh
./build/calculator simple
```
Basic mode for simple expressions like `3.5 * 2`

### Help
```sh
./build/calculator --help
```

## Features

### Simple Mode
- Basic arithmetic: addition, subtraction, multiplication, division
- Floating point and integer support
- Error handling for division by zero and invalid operators

### Advanced Mode
- All simple mode features plus:
- Complex expression parsing with proper operator precedence
- Parentheses support for grouping
- Exponentiation operator (^)
- Unary minus operator
- Comprehensive error reporting
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
│   ├── main.c          # Main entry point with mode selection
│   ├── calculator.c    # Core arithmetic functions
│   ├── calculator.h    # Calculator function declarations
│   ├── parser.c        # Advanced expression parser
│   ├── parser.h        # Parser function declarations
│   ├── repl.c          # Read-Eval-Print-Loop for advanced mode
│   └── repl.h          # REPL function declarations
├── tests/
│   └── test_calculator.c  # Unit tests for both modes
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
