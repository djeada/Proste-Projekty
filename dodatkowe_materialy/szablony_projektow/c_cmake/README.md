## Struktura Projektu C z CMake

- projekt_c/
  - src/
    - main.c
    - hello.c
    - hello.h
  - tests/
    - test_hello.c
  - CMakeLists.txt
  - README.md
  - LICENSE
  - .gitignore

## src/main.c

Główny plik programu, na przykład z funkcją `main`:

#include "hello.h"
#include <stdio.h>

int main() {
    printf("%s\n", get_hello_message());
    return 0;
}

## src/hello.c

Implementacja funkcji zdefiniowanych w hello.h:

#include "hello.h"

const char* get_hello_message() {
    return "Hello, World!";
}

## src/hello.h

Nagłówek dla twoich funkcji:

#ifndef HELLO_H
#define HELLO_H

const char* get_hello_message();

#endif // HELLO_H

## CMakeLists.txt

Konfiguracja budowy projektu z CMake:

cmake_minimum_required(VERSION 3.10)
project(projekt_c VERSION 1.0)

add_executable(projekt_c src/main.c src/hello.c)

## Budowanie Projektu

Stwórz katalog `build` i przejdź do niego:

mkdir build && cd build

Uruchom CMake i zbuduj projekt:

cmake ..
make

## Testowanie

Możesz użyć narzędzi takich jak CTest lub Google Test do testowania swojego projektu. Testy umieszczasz w katalogu `tests`. Oto przykładowy test z użyciem Google Test:


