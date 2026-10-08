# Szablon projektu w Pythonie

Najmniejszy kompletny projekt w Pythonie: logika oddzielona od interfejsu, testy w pytest, sprawdzanie stylu przez flake8 i automatyczne testy na GitHubie. Przykładem jest przelicznik temperatur — zamień go na swój program.

## Struktura

```
.
├── src/
│   ├── converter.py        logika: same obliczenia, bez input() i print()
│   └── main.py             interfejs: czyta dane od użytkownika i wypisuje wynik
├── tests/
│   └── test_converter.py   testy logiki (pytest)
├── requirements.txt        biblioteki potrzebne do uruchomienia i testów
├── pyproject.toml          ustawienia pytest (gdzie szukać modułów)
├── .flake8                 ustawienia flake8 (maksymalna długość linii)
├── .github/workflows/ci.yml  GitHub Actions: flake8 i testy
├── .editorconfig           wcięcia i kodowanie dla edytora
└── .gitignore              pliki, których nie dodajemy do repozytorium
```

Najważniejsza zasada: **logika nie wie nic o interfejsie**. Funkcje w `converter.py` dostają argumenty i zwracają wynik albo zgłaszają wyjątek — nie pytają użytkownika i nic nie wypisują. Dzięki temu testy mogą je wywołać bezpośrednio, a interfejs tekstowy w `main.py` można później zamienić na okno w tkinterze albo grę w pygame bez zmiany logiki.

## Jak to działa

- `main.py` importuje logikę zwykłym `from converter import convert`. Python uruchomiony poleceniem `python3 src/main.py` szuka modułów w katalogu skryptu, czyli w `src/`.
- Testy też importują `converter`. Działa to dzięki ustawieniu `pythonpath = ["src"]` w `pyproject.toml`, które dodaje `src/` do ścieżki modułów podczas testów.
- Plik testowy i funkcje testowe muszą zaczynać się od `test_` — po tym pytest je znajduje. `assert` sprawdza warunek, `pytest.approx` porównuje liczby zmiennoprzecinkowe z tolerancją, a `pytest.raises` sprawdza, czy funkcja zgłasza wyjątek.
- Warunek `if __name__ == "__main__":` sprawia, że `main()` uruchamia się tylko wtedy, gdy plik jest uruchamiany, a nie gdy jest importowany.

## Wymagania

- Python 3.8 lub nowszy

## Uruchamianie

```sh
python3 src/main.py
```

## Testy i styl kodu

Najlepiej w środowisku wirtualnym:

```sh
python3 -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
pytest
flake8 src tests
```

## Jak zacząć własny projekt

1. Skopiuj ten katalog i zmień nazwę `converter` na nazwę swojego projektu.
2. Najpierw napisz logikę i testy do niej, potem interfejs w `main.py`.
3. Biblioteki zewnętrzne (np. `pygame`) dopisz do `requirements.txt`.
4. Gdy moduł logiki przekroczy kilkaset linii, podziel go na kilka plików w `src/`. Pakiety (katalogi z `__init__.py`) są potrzebne dopiero w większych projektach.
5. Uzupełnij README: co robi program, jak go uruchomić, jak działa i jak go testować.
