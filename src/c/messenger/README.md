# Messenger in C

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
This is a simple TCP-based chat messenger in C. It supports both server and client modes, allowing multiple users to communicate over a local network.

## Requirements
- CMake >= 3.10
- C compiler (e.g., gcc, clang)
- POSIX-compatible system (Linux, macOS)
- clang-tidy, clang-format (optional, for code quality)
- Docker (optional, for containerized builds)

## Installation
Clone the repository and build the project:
```sh
git clone https://github.com/djeada/Proste-Projekty.git
cd Proste-Projekty/src/c/messenger
cmake -S . -B build
cmake --build build
```

## Usage

### Start a Server
```sh
./build/main server [port]
```
Default port is 8888.

### Connect as Client
```sh
./build/main client <host> [port] [username]
```
Example:
```sh
./build/main client 127.0.0.1 8888 Alice
```

### Chat Commands
- Type a message and press Enter to send
- Type `quit` to disconnect

## Features
- TCP socket-based communication
- Server mode with multi-client support
- Client mode with username support
- Real-time message broadcasting
- Simple and clean terminal interface

## Testing
Run unit tests using CTest:
```sh
cd build
ctest
```
Or run the test binary directly:
```sh
./build/test_messenger
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
docker build -t messenger .
docker run -it messenger
```

## Project Structure
```
messenger/
├── src/
│   ├── main.c
│   ├── messenger.c
│   └── messenger.h
├── tests/
│   └── test_messenger.c
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
- Add end-to-end encryption
- Implement user authentication
- Add file transfer support
- Create graphical user interface
- Add private messaging between users
- Support for rooms/channels

## Contributing
Contributions are welcome! Please open issues or pull requests for improvements, bug fixes, or new features.

## License
This project is licensed under the [MIT License](https://github.com/djeada/Proste-Projekty/blob/main/LICENSE).
