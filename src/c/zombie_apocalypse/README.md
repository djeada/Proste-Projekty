# Zombie Apocalypse (C)

Terminal zombie survival with ncurses. Start simple and ramp up: each cleared wave increases zombie speed and grants a score bonus. Clear waves, press 'n' to advance, and try to survive.

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

## Lint & Format

```sh
cmake --build . --target format
cmake --build . --target main
```

## Controls

- Move: WASD or Arrow keys
- Shoot: Space (fires in your last move direction)
- Next wave: 'n' (after clearing all zombies)
- Restart: 'r'
- Pause: 'p'

## HUD

- `HP`: your health
- `Lvl`: current wave level
- `Score`: points for kills and wave clears
- `Zombies`: total zombies spawned this wave
- `Speed`: how frequently zombies step (lower = faster)

## Difficulty Curve

- Level 1: slow zombies stepping every 3 ticks; small horde.
- Each new level:
	- More zombies: BASE_ZOMBIES + (level-1)*ZOMBIES_PER_LEVEL (capped at MAX_ZOMBIES)
	- Faster step rate: zombies move more often (down to 1 tick)
- Score: +10 per kill, +100 wave clear bonus.

## Requirements
- C compiler (C99)
- CMake
- clang-tidy, clang-format (optional, for linting)

---

This project follows the template from `dodatkowe_materialy/szablony_projektow/c_cmake`.
