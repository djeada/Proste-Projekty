# Szablon projektu w C z użyciem CMake

## Opis
Ten szablon pozwala szybko rozpocząć projekt w języku C z użyciem CMake, testów, linterów i automatyzacji.

## Funkcje
- Budowanie przez CMake
- Testy jednostkowe (CTest)
- Lintowanie (clang-tidy)
- Formatowanie kodu (clang-format)
- Automatyzacja CI (GitHub Actions)
- Deployment przez Docker
- Spójny styl kodu (EditorConfig)

## Wymagania
- CMake >= 3.10
- Kompilator C (np. gcc, clang)
- clang-tidy, clang-format (opcjonalnie)
- Docker (opcjonalnie)

## Instalacja i budowanie
```sh
cmake -S . -B build
cmake --build build
```

## Testowanie
```sh
cd build
ctest
```

## Lintowanie i formatowanie
```sh
clang-tidy src/*.c
clang-format -i src/*.c
```

## Deployment
Budowa i uruchomienie obrazu Docker:
```sh
docker build -t moj-projekt-c .
docker run moj-projekt-c
```

## Dobre praktyki
- Kod źródłowy w `src/`
- Testy w `tests/`
- Używaj lintera i formatowania przed commitem
- Automatyzuj testy i lint w CI

## Struktura katalogów
```
projekt_c/
├── src/
│   ├── main.c
│   ├── hello.c
│   └── hello.h
├── tests/
│   └── test_hello.c
├── CMakeLists.txt
├── Dockerfile
├── .clang-tidy
├── .clang-format
└── README.md
```


