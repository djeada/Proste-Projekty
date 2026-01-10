# 2048 (C, standard terminal)

A feature-rich 2048 clone for the terminal with colorful tiles and Unicode box-drawing.

## Features

- **Colorful Tiles**: Each tile value (2, 4, 8, 16, ..., 2048+) has a unique color
- **Unicode Box Drawing**: Beautiful grid with Unicode borders
- **Arrow Key Support**: Use arrow keys or WASD to move tiles
- **Score Tracking**: Live score display during gameplay
- **Clean UI**: Attractive header with instructions, game over screen

## Build & Run

```sh
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . -j
./main
```

## Test

```sh
cd build
ctest --output-on-failure
```

## Controls

- **Move tiles**: W/A/S/D or Arrow Keys (↑↓←→)
- **Restart** (on Game Over): R
- **Quit**: Q

## Notes

- Board: 4x4 grid
- New tiles: 2 (90% chance) or 4 (10% chance)
- Score increases by the value of merged tiles
- Requires a terminal with ANSI color and Unicode support
