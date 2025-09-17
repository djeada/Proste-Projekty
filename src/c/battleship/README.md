# Battleship (C + ncurses)

Classic Battleship in the terminal using ncurses. Place ships on your board, then take turns firing at the opponent grid.

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

- Move cursor: WASD or Arrow keys
- Rotate ship (placement): r
- Place ship: Enter
- Fire (battle): Enter
- Toggle phase (after placement in single-player demo): n
- Restart: q

## Notes

- This version includes a simple single-player demo vs random AI for testing.
- Board: 10x10, ships of standard lengths: 5, 4, 3, 3, 2.
