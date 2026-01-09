# Version Control System in C

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
This is a simple version control system in C that provides basic functionality for managing project versions. It allows users to save the current state of a project, view commit history, and restore previous versions.

## Requirements
- CMake >= 3.10
- C compiler (e.g., gcc, clang)
- clang-tidy, clang-format (optional, for code quality)
- Docker (optional, for containerized builds)

## Installation
Clone the repository and build the project:
```sh
git clone https://github.com/djeada/Proste-Projekty.git
cd Proste-Projekty/src/c/version_control
cmake -S . -B build
cmake --build build
```

## Usage
Run the program:
```sh
./build/main [path_to_repository]
```

### Available Commands
- `init <path>` - Initialize a repository at the specified path
- `status` - Show repository status
- `commit <message>` - Create a new commit with all current files
- `log` - Display commit history
- `checkout <id>` - Restore files from a specific commit
- `diff <id1> <id2>` - Show differences between two commits
- `help` - Show available commands
- `quit` - Exit the program

### Example Session
```
vcs> init /path/to/project
Initialized repository at: /path/to/project

vcs> commit Initial version
Created commit #1

vcs> commit Added new feature
Created commit #2

vcs> log
=== Commit History ===

Commit #2
Date:    2024-01-15 10:30:45
Message: Added new feature
Files:   3
---
Commit #1
Date:    2024-01-15 10:25:12
Message: Initial version
Files:   2
---

vcs> checkout 1
Checked out commit #1: Initial version
```

## Features
- Initialize repositories at any directory
- Create commits with all files in the repository
- View commit history with timestamps
- Restore files from any previous commit
- Compare differences between commits
- Track file additions, modifications, and deletions

## Testing
Run unit tests using CTest:
```sh
cd build
ctest
```
Or run the test binary directly:
```sh
./build/test_version_control
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
docker build -t version_control .
docker run -it version_control
```

## Project Structure
```
version_control/
├── src/
│   ├── main.c
│   ├── version_control.c
│   └── version_control.h
├── tests/
│   └── test_version_control.c
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
- Add branching support
- Implement merge functionality
- Add remote repository support
- Compress stored file snapshots
- Add file staging area
- Implement conflict resolution

## Contributing
Contributions are welcome! Please open issues or pull requests for improvements, bug fixes, or new features.

## License
This project is licensed under the [MIT License](https://github.com/djeada/Proste-Projekty/blob/main/LICENSE).
