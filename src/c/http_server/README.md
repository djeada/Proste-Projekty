# Simple HTTP Server in C

![Build Status](https://github.com/djeada/Proste-Projekty/actions/workflows/ci.yml/badge.svg)
![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)

## Table of Contents
- [Project Overview](#project-overview)
- [References](#references)
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
This is a minimal HTTP server written in C. It listens on port 8080 and responds with a simple "Hello, world!" message to any HTTP request. The project demonstrates basic socket programming and HTTP response handling in C.

## References
- https://progbook.org/httpserv.html

## Requirements
- CMake >= 3.10
- C compiler (e.g., gcc, clang)
- clang-tidy, clang-format (optional, for code quality)
- Docker (optional, for containerized builds)

## Installation
Clone the repository and build the project:
```sh
git clone https://github.com/djeada/Proste-Projekty.git
cd Proste-Projekty/src/c/http_server
cmake -S . -B build
cmake --build build
```

## Usage
Run the server:
```sh
./build/main
```
The server will listen on port 8080. Open your browser and go to `http://localhost:8080` to see the response.

## Features
- Listens on port 8080
- Responds to any HTTP request with a plain text message
- Minimal, easy-to-understand code

## Testing
No unit tests yet. You can test the server manually by connecting with a browser or using `curl`:
```sh
curl http://localhost:8080
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
docker build -t http-server .
docker run -p 8080:8080 http-server
```

## Project Structure
```
http_server/
├── src/
│   └── main.c
├── CMakeLists.txt
├── Dockerfile
├── .clang-tidy
├── .clang-format
├── .editorconfig
├── .github/
│   └── workflows/
│       └── ci.yml
├── tests/
└── README.md
```

## Possible Improvements
- Add support for serving static files
- Add unit tests for request parsing
- Add logging and configuration options
- Support for multiple clients (concurrent connections)

## Contributing
Contributions are welcome! Please open issues or pull requests for improvements, bug fixes, or new features.

## License
This project is licensed under the [MIT License](https://github.com/djeada/Proste-Projekty/blob/main/LICENSE).
