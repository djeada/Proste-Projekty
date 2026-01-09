# Graphics Editor in C

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
This is a simple console-based graphics editor (Paint-like) in C. It supports creating, loading, and saving images in PPM format, with various drawing tools and image manipulation features. Images can be viewed as ASCII art in the terminal.

## Requirements
- CMake >= 3.10
- C compiler (e.g., gcc, clang)
- clang-tidy, clang-format (optional, for code quality)
- Docker (optional, for containerized builds)

## Installation
Clone the repository and build the project:
```sh
git clone https://github.com/djeada/Proste-Projekty.git
cd Proste-Projekty/src/c/graphics_editor
cmake -S . -B build
cmake --build build
```

## Usage
Run the program:
```sh
./build/main
```

### Drawing Tools
- **Draw pixel** - Single pixel at specified coordinates
- **Draw line** - Line between two points (Bresenham's algorithm)
- **Draw/Fill rectangle** - Outline or filled rectangle
- **Draw/Fill circle** - Outline or filled circle (Midpoint algorithm)
- **Bucket fill** - Flood fill from a point

### Transform Operations
- **Rotate** - 90° or 180° rotation
- **Flip** - Horizontal or vertical flip
- **Resize** - Scale to new dimensions
- **Crop** - Extract region

### Filters & Effects
- **Grayscale** - Convert to grayscale
- **Invert** - Invert colors
- **Brightness** - Adjust brightness (-255 to +255)
- **Contrast** - Adjust contrast (factor)

### File Operations
- **New image** - Create blank canvas
- **Load PPM** - Load from PPM file
- **Save PPM** - Save to PPM file
- **View ASCII** - Display as ASCII art

## Features
- Create images of any size (up to 1024x1024)
- PPM format support (P6 binary)
- Drawing tools: pixel, line, rectangle, circle
- Bucket fill (flood fill)
- Image transformations: rotate, flip, resize, crop
- Filters: grayscale, invert, brightness, contrast
- ASCII art preview in terminal
- Color selection (RGB)

## Testing
Run unit tests using CTest:
```sh
cd build
ctest
```
Or run the test binary directly:
```sh
./build/test_graphics_editor
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
docker build -t graphics_editor .
docker run -it graphics_editor
```

## Project Structure
```
graphics_editor/
├── src/
│   ├── main.c
│   ├── graphics_editor.c
│   └── graphics_editor.h
├── tests/
│   └── test_graphics_editor.c
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
- Add PNG/JPG support using external libraries
- Implement undo/redo functionality
- Add more drawing tools (bezier curves, polygons)
- Implement layers support
- Add blur, sharpen, and other filters
- Create graphical UI using ncurses or SDL

## Contributing
Contributions are welcome! Please open issues or pull requests for improvements, bug fixes, or new features.

## License
This project is licensed under the [MIT License](https://github.com/djeada/Proste-Projekty/blob/main/LICENSE).
