# Graphics Editor in C

![Build Status](https://github.com/djeada/Proste-Projekty/actions/workflows/ci.yml/badge.svg)
![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)

## Table of Contents
- [Project Overview](#project-overview)
- [Requirements](#requirements)
- [Installation](#installation)
- [Usage](#usage)
- [Features](#features)
- [Controls](#controls)
- [Testing](#testing)
- [Linting and Formatting](#linting-and-formatting)
- [Deployment](#deployment)
- [Project Structure](#project-structure)
- [Possible Improvements](#possible-improvements)
- [Contributing](#contributing)
- [License](#license)

## Project Overview
This is a graphical paint application built with SDL2 in C. It provides a modern, mouse-driven interface for creating and editing images with various drawing tools, color selection, and image manipulation features. Images can be saved and loaded in PPM format.

## Requirements
- CMake >= 3.10
- C compiler (e.g., gcc, clang)
- SDL2 development libraries
- pkg-config
- clang-tidy, clang-format (optional, for code quality)
- Docker (optional, for containerized builds)

### Installing SDL2

**Ubuntu/Debian:**
```sh
sudo apt-get install libsdl2-dev
```

**macOS (Homebrew):**
```sh
brew install sdl2
```

**Fedora:**
```sh
sudo dnf install SDL2-devel
```

**Arch Linux:**
```sh
sudo pacman -S sdl2
```

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

The application opens a graphical window with:
- A toolbar at the top with drawing tools and color palette
- A large white canvas area for drawing
- Real-time preview while drawing shapes

## Features
- **Modern GUI**: SDL2-based graphical interface with toolbar, color palette, and canvas
- **Drawing Tools**:
  - Pencil: Freehand drawing with mouse drag
  - Line: Click and drag to draw straight lines
  - Rectangle: Click and drag to draw rectangles
  - Circle: Click and drag to draw circles (radius from center)
  - Fill (Bucket): Click to flood-fill an area with current color
- **Color Palette**: 16 preset colors for quick selection
- **Current Color Display**: Shows the currently selected color
- **Image Manipulation**:
  - Grayscale conversion
  - Color inversion
  - 90° rotation
  - Horizontal/vertical flip
- **File Operations**:
  - New image (blank canvas)
  - Save to PPM format
  - Load from PPM format
- **Real-time Shape Preview**: See shapes as you draw them
- **Keyboard Shortcuts**: Quick access to all features

## Controls

### Keyboard Shortcuts
| Key | Action |
|-----|--------|
| 1 | Select Pencil tool |
| 2 | Select Line tool |
| 3 | Select Rectangle tool |
| 4 | Select Circle tool |
| 5 | Select Fill tool |
| N | New image (clear canvas) |
| S | Save image to output.ppm |
| L | Load image from output.ppm |
| G | Convert to grayscale |
| I | Invert colors |
| R | Rotate 90 degrees |
| H | Flip horizontal |
| V | Flip vertical |
| Delete/Backspace | Clear canvas |
| Escape | Exit application |

### Mouse Controls
- **Click** on tool buttons to select tools
- **Click** on color palette to select colors
- **Click and drag** on canvas to draw
- For Pencil: continuous drawing while dragging
- For Line/Rectangle/Circle: preview shown while dragging, shape drawn on release

## Testing
Run unit tests using CTest (tests the image logic, no SDL2 required):
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

Note: Running GUI applications in Docker requires X11 forwarding or similar setup.

## Project Structure
```
graphics_editor/
├── src/
│   ├── main.c              # SDL2 GUI application
│   ├── graphics_editor.c   # Image manipulation logic
│   └── graphics_editor.h   # Header file
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
- Add PNG/JPG support using SDL_image
- Implement undo/redo functionality with history stack
- Add more drawing tools (bezier curves, polygons, text)
- Implement layers support
- Add blur, sharpen, and other filters
- Add brush size selection
- Add file dialog for save/load
- Add zoom and pan functionality
- Add selection tool for copy/paste
- Add SDL_ttf for text rendering in UI

## Contributing
Contributions are welcome! Please open issues or pull requests for improvements, bug fixes, or new features.

## License
This project is licensed under the [MIT License](https://github.com/djeada/Proste-Projekty/blob/main/LICENSE).
