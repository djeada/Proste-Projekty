# Zombie Apocalypse (C)

A simple C implementation of the Zombie Apocalypse simulation/game.

## Build & Run

```sh
mkdir -p build
cd build
cmake ..
make
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

## Project Structure

- `src/` - source code
- `tests/` - unit tests
- `CMakeLists.txt` - build configuration

## Requirements
- C compiler (C99)
- CMake
- clang-tidy, clang-format (optional, for linting)

---

This project follows the template from `dodatkowe_materialy/szablony_projektow/c_cmake`.
