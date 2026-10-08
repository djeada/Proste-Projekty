# Szablon projektu w C (CMake)

Najmniejszy kompletny projekt w C: logika oddzielona od interfejsu, testy uruchamiane przez CTest, formatowanie przez clang-format i automatyczne sprawdzanie na GitHubie. Przykładem jest przelicznik temperatur — zamień go na swój program.

## Struktura

```
.
├── CMakeLists.txt          przepis na zbudowanie programu i testów
├── src/
│   ├── converter.h         deklaracje funkcji logiki (co moduł udostępnia)
│   ├── converter.c         logika: same obliczenia, bez printf i scanf
│   └── main.c              interfejs: czyta dane od użytkownika i wypisuje wynik
├── tests/
│   └── test_converter.c    testy logiki napisane za pomocą assert()
├── .github/workflows/ci.yml  GitHub Actions: budowanie, testy, formatowanie
├── .clang-format           styl kodu dla clang-format
├── .editorconfig           wcięcia i kodowanie dla edytora
└── .gitignore              pliki, których nie dodajemy do repozytorium (katalog build/)
```

Najważniejsza zasada: **logika nie wie nic o interfejsie**. Funkcje w `converter.c` dostają argumenty i zwracają wynik — nie czytają z klawiatury i nic nie wypisują. Dzięki temu testy mogą je wywołać bezpośrednio, a ten sam moduł można później podłączyć do innego interfejsu (ncurses, SDL, serwera sieciowego) bez zmieniania ani jednej linijki.

## Jak to działa

`CMakeLists.txt` buduje trzy rzeczy:

1. bibliotekę `converter_logic` z pliku `src/converter.c`,
2. program `converter` z pliku `src/main.c`, połączony z tą biblioteką,
3. program testowy `test_converter`, połączony z tą samą biblioteką i zarejestrowany w CTest poleceniem `add_test`.

Test to zwykły program: `assert()` przerywa go, gdy warunek jest fałszywy, a CTest uznaje test za zaliczony, gdy program kończy się kodem 0. Liczby zmiennoprzecinkowe porównujemy z tolerancją (`fabs(a - b) < 1e-9`), bo np. `37 * 9 / 5 + 32` nie musi dać dokładnie `98.6`.

Flagi `-Wall -Wextra` włączają ostrzeżenia kompilatora. Projekt powinien budować się bez żadnego ostrzeżenia — każde to potencjalny błąd.

## Wymagania

- kompilator C (gcc lub clang) i CMake 3.10 lub nowszy
- opcjonalnie clang-format

## Budowanie i uruchamianie

```sh
cmake -S . -B build
cmake --build build
./build/converter
```

## Testy

```sh
cd build
ctest --output-on-failure
```

## Formatowanie

```sh
clang-format -i src/*.c src/*.h tests/*.c
```

## Jak zacząć własny projekt

1. Skopiuj ten katalog i zmień nazwę `converter` na nazwę swojego projektu (pliki w `src/` i `tests/` oraz `CMakeLists.txt`).
2. Najpierw napisz logikę i testy do niej, potem interfejs w `main.c`.
3. Gdy plik logiki przekroczy kilkaset linii, podziel go na kilka modułów (`.c` + `.h`) i dopisz je do `add_library`.
4. Biblioteki zewnętrzne dołączaj przez `find_package`, np. `find_package(Curses REQUIRED)` i `target_link_libraries(nazwa ${CURSES_LIBRARIES})`.
5. Uzupełnij README: co robi program, jak go uruchomić, jak działa i jak go testować.
