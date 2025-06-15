# Caesar Cipher

## Opis
Aplikacja desktopowa wykorzystująca szyfr Cezara do szyfrowania i deszyfrowania wiadomości tekstowych.

## Funkcje
- Szyfrowanie i deszyfrowanie tekstu
- Prosty interfejs użytkownika
- Testy jednostkowe (pytest)
- Lintowanie (flake8)
- Formatowanie kodu (black)
- Deployment przez Docker

## Wymagania
- Python >= 3.10
- pip
- Docker (opcjonalnie)

## Instalacja i testowanie
```sh
pip install .[dev]
flake8 src/ tests/
black --check src/ tests/
pytest
```

## Budowanie binarki
```sh
nuitka --standalone --onefile src/caesar_cipher/main.py -o app.bin
```

## Deployment
Budowa i uruchomienie obrazu Docker:
```sh
docker build -t caesar-cipher-app .
docker run caesar-cipher-app
```

## Dobre praktyki
- Kod źródłowy w `src/`
- Testy w `tests/`
- Używaj lintera i formatowania przed commitem
- Automatyzuj testy i lint w CI

## Struktura katalogów
```
caesar_cipher/
├── src/
│   └── caesar_cipher/
│       ├── main.py
│       ├── logic/
│       └── gui/
├── tests/
│   └── test_caesar_cipher.py
├── setup.py
├── Dockerfile
└── README.md
```

## Przykładowy kod

### src/caesar_cipher/logic/caesar_cipher.py

```python
# ...istniejący kod szyfrujący...
```

### tests/test_caesar_cipher.py

```python
# ...istniejący kod testów...
```
