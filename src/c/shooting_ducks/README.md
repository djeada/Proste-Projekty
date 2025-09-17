# Shooting Ducks (C)

Terminal duck shooting with ncurses. Move the crosshair, shoot flying ducks, clear waves, and advance levels as ducks get faster and more numerous.

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

- Move crosshair: WASD or Arrow keys
- Shoot: Space
- Next wave: 'n' (after clearing)
- Restart: 'r'
- Pause: 'p'

## Difficulty Curve

- Level 1: few, slow ducks drifting right.
- Each new level:
  - More ducks per wave
  - Faster duck speed
- Score: +10 per duck, +100 wave clear bonus.

## Requirements
- C compiler (C99)
- CMake
- ncurses
