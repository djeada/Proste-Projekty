# Caesar Cipher in C

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
This project is a simple implementation of the Caesar cipher in C. It allows you to encrypt and decrypt text using a shift key. The project demonstrates basic cryptography and C programming concepts, and is structured for easy testing, linting, and CI/CD.

## Screenshots
![cipher](https://github.com/djeada/Proste-Projekty/assets/37275728/6f0b1c2f-a948-44e1-96b0-1a1d4b279256)

## Requirements
- CMake >= 3.10
- C compiler (e.g., gcc, clang)
- clang-tidy, clang-format (optional, for code quality)
- Docker (optional, for containerized builds)

## Installation
Clone the repository and build the project:
```sh
git clone https://github.com/djeada/Proste-Projekty.git
cd Proste-Projekty/src/c/caesar_cipher
cmake -S . -B build
cmake --build build
```

## Usage
Run the program:
```sh
./build/main
```
You will be prompted to enter the text, the shift key, and whether to encrypt or decrypt.

## Features
- Encrypt and decrypt text using the Caesar cipher
- Handles both uppercase and lowercase letters
- Ignores non-alphabetic characters
- Command-line interface

## Testing
Run unit tests using CTest:
```sh
cd build
ctest
```
Or run the test binary directly:
```sh
./build/test_caesar
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
docker build -t caesar-cipher .
docker run caesar-cipher
```

## Project Structure
```
caesar_cipher/
├── src/
│   └── main.c
├── tests/
│   └── test_caesar.c
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
- Add support for reading input/output from files
- Add more unit tests (e.g., for full string encryption/decryption)
- Provide a library interface (separate header/source)
- Add command-line arguments for batch processing

## Contributing
Contributions are welcome! Please open issues or pull requests for improvements, bug fixes, or new features.

## License
This project is licensed under the [MIT License](https://github.com/djeada/Proste-Projekty/blob/main/LICENSE).
