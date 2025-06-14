# Weather (C)

A simple terminal weather application written in C. Fetches weather data for a given city using wttr.in.

## Features
- Fetches and displays weather for a city
- Colorful terminal output
- Clean code structure and tests

## Build & Run

### Locally
```sh
mkdir -p build
cd build
cmake ..
make
./main London
```

### With Docker
```sh
docker build -t weather .
docker run --rm -it weather
```

## Test
```sh
cd build
ctest --output-on-failure
```

## Lint
```sh
clang-tidy src/*.c
```

## Format
```sh
clang-format -i src/*.c src/*.h
```
