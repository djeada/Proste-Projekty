# Text Editor in C

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
This is a simple line-based text editor in C. It allows creating, editing, and saving text files with basic operations like adding, inserting, and deleting lines.

## Requirements
- CMake >= 3.10
- C compiler (e.g., gcc, clang)
- clang-tidy, clang-format (optional, for code quality)
- Docker (optional, for containerized builds)

## Installation
Clone the repository and build the project:
```sh
git clone https://github.com/djeada/Proste-Projekty.git
cd Proste-Projekty/src/c/text_editor
cmake -S . -B build
cmake --build build
```

## Usage
Run the program:
```sh
./build/main
```
Or open an existing file:
```sh
./build/main myfile.txt
```

Type 'h' for a list of available commands.

## Features
- Load and save text files
- Add lines at the end
- Insert lines at any position
- Delete lines by line number
- Print all lines with line numbers
- Clear buffer and start new file
- Prompt to save unsaved changes on quit

## Testing
Run unit tests using CTest:
```sh
cd build
ctest
```
Or run the test binary directly:
```sh
./build/test_text_editor
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
docker build -t text_editor .
docker run -it text_editor
```

## Project Structure
```
text_editor/
├── src/
│   ├── main.c
│   ├── text_editor.c
│   └── text_editor.h
├── tests/
│   └── test_text_editor.c
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
- Add search and replace functionality
- Implement undo/redo
- Add syntax highlighting using ncurses
- Support for multiple buffers/tabs

## Contributing
Contributions are welcome! Please open issues or pull requests for improvements, bug fixes, or new features.

## License
This project is licensed under the [MIT License](https://github.com/djeada/Proste-Projekty/blob/main/LICENSE).
