# Szablon projektu Python (setup.py + Nuitka)

## Opis
Ten szablon pozwala szybko rozpocząć projekt w Pythonie z automatyzacją, testami, linterami i wsparciem dla kompilacji binarnej przez Nuitka.

## Funkcje
- setup.py do budowania i instalacji
- Kompilacja binarna przez Nuitka
- Lintowanie (flake8)
- Formatowanie kodu (black)
- Testy jednostkowe (pytest)
- Automatyzacja CI (GitHub Actions)
- Deployment przez Docker
- Spójny styl kodu (EditorConfig)

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
nuitka --standalone --onefile src/example_package/example_module.py -o app.bin
```

## Deployment
Budowa i uruchomienie obrazu Docker:
```sh
docker build -t moj-python-app .
docker run moj-python-app
```

## Dobre praktyki
- Kod źródłowy w `src/`
- Testy w `tests/`
- Używaj lintera i formatowania przed commitem
- Automatyzuj testy i lint w CI

## Struktura katalogów
```
projekt_python/
├── src/
│   └── example_package/
│       └── example_module.py
├── tests/
│   └── test_example_module.py
├── setup.py
├── Dockerfile
└── README.md
```

## Przykładowy kod

### src/example_package/example_module.py

To prosty moduł Python w twoim pakiecie. Oto przykładowy kod:

```python
def say_hello(name):
    """ Funkcja wypowiadająca powitanie """
    return f"Hello, {name}!"
```

### src/example_package/__init__.py

Ten plik może być pusty, lub użyty do eksponowania niektórych funkcji lub klas na poziomie pakietu:

```python
from .example_module import say_hello
```

### tests/test_example_module.py

Oto przykład prostego testu z użyciem `unittest`:

```python
import unittest
from example_package.example_module import say_hello

class TestExampleModule(unittest.TestCase):

    def test_say_hello(self):
        self.assertEqual(say_hello("World"), "Hello, World!")

if __name__ == '__main__':
    unittest.main()
```

## Modyfikacja setup.py

Musisz nieco zmodyfikować swój setup.py, aby wskazać nowy katalog źródłowy:

```python
from setuptools import setup, find_packages

setup(
    name='example_package',
    version='0.1',
    package_dir={'': 'src'},  # Ta linia dodaje wskazanie na katalog źródłowy
    packages=find_packages(where='src'),  # Wskazuje katalog źródłowy
    license='MIT',
    description='Przykładowy pakiet Python',
    long_description=open('README.md').read(),
    install_requires=['numpy'],
    url='https://github.com/twojeusername/example_package',
    author='Twoje Imię',
    author_email='twoj.email@example.com'
)
```

## Budowanie i Dystrybucja

Aby zainstalować pakiet lokalnie dla celów deweloperskich:

```sh
pip install -e .
```

Aby zbudować pakiet do dystrybucji:

```sh
python setup.py sdist bdist_wheel
```

Aby przesłać go do PyPI:

```sh
twine upload dist/*
```

## Jak Korzystać z Tego Szablonu

Ten szablon pozwala na wyraźne oddzielenie kodu pakietu (w 'src') od innych plików projektu, co sprawia, że jest on bardziej uporządkowany i łatwiejszy w zarządzaniu. Pamiętaj, aby dostosować ścieżki w swoich testach i skryptach do nowej struktury projektu.
