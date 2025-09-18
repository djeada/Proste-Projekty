# 2048 (C, standard terminal)

A simple 2048 clone for the terminal. No ncurses required.

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

- Move tiles: WASD
- Restart (on Game Over): r
- Quit: q

## Notes

- Board: 4x4, spawns 2 (90%) or 4 (10%).
- Score increases by the value of merged tiles.
