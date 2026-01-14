# Zombie Apocalypse (C)

A visually enhanced terminal zombie survival game with ncurses. Features colorful graphics, particle effects, screen shake, and smooth animations. Start simple and ramp up: each cleared wave increases zombie speed and grants a score bonus.

## Features

### Visual Enhancements
- **Color-coded elements**: Player (cyan), Zombies (green), Bullets (yellow), Explosions (red)
- **Visual health bar**: Diamond indicators with color coding (green/yellow/red based on health)
- **Decorative border**: Clean frame around the game area
- **Directional player**: Character shows facing direction (^v<>)
- **Animated zombies**: Z/z alternating for movement effect
- **Explosion particles**: *+x. debris when zombies die
- **Screen effects**: Damage flash, screen shake, invincibility blink
- **High score tracking**: Persistent across game sessions
- **Level transition animation**: Brief display at start of each wave

### Scalability
- Adapts to different terminal sizes (minimum 60x20)
- Proper game area boundaries with HUD panel
- Terminal resize handling during gameplay

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
- Quit: 'q' or ESC

## HUD

- `HP`: Visual health bar with diamond indicators
- `Lvl`: Current wave level
- `Score`: Points for kills and wave clears
- `Hi`: High score (displayed if > 0)
- `Zombies`: Total zombies remaining / spawned this wave
- `Speed`: Visual indicator showing zombie movement rate

## Difficulty Curve

- Level 1: Slow zombies stepping every 3 ticks; small horde (6 zombies)
- Each new level:
  - More zombies: BASE_ZOMBIES + (level-1)*ZOMBIES_PER_LEVEL (capped at MAX_ZOMBIES)
  - Faster step rate: zombies move more often (down to 1 tick)
- Score: +10 per kill, +100 wave clear bonus

## Requirements
- C compiler (C99)
- CMake 3.10+
- ncurses library
- Terminal with color support (recommended)
- Minimum terminal size: 60x20

---

This project follows the template from `dodatkowe_materialy/szablony_projektow/c_cmake`.
