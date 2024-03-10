## Struktura Projektu

Twoja struktura katalogów projektu powinna wyglądać następująco:

- example_project/
  - src/
    - example_package/
      - __init__.py
      - example_module.py
  - tests/
    - test_example_module.py
  - setup.py
  - requirements.txt
  - README.md
  - LICENSE

## src/example_package/example_module.py

To prosty moduł Python w twoim pakiecie. Oto przykładowy kod:

def say_hello(name):
    """ Funkcja wypowiadająca powitanie """
    return f"Hello, {name}!"

## src/example_package/__init__.py

Ten plik może być pusty, lub użyty do eksponowania niektórych funkcji lub klas na poziomie pakietu:

from .example_module import say_hello

## tests/test_example_module.py

Oto przykład prostego testu z użyciem `unittest`:

import unittest
from example_package.example_module import say_hello

class TestExampleModule(unittest.TestCase):

    def test_say_hello(self):
        self.assertEqual(say_hello("World"), "Hello, World!")

if __name__ == '__main__':
    unittest.main()

## Modyfikacja setup.py

Musisz nieco zmodyfikować swój setup.py, aby wskazać nowy katalog źródłowy:

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

## Budowanie i Dystrybucja

Aby zainstalować pakiet lokalnie dla celów deweloperskich:

pip install -e .

Aby zbudować pakiet do dystrybucji:

python setup.py sdist bdist_wheel

Aby przesłać go do PyPI:

twine upload dist/*

## Jak Korzystać z Tego Szablonu

Ten szablon pozwala na wyraźne oddzielenie kodu pakietu (w 'src') od innych plików projektu, co sprawia, że jest on bardziej uporządkowany i łatwiejszy w zarządzaniu. Pamiętaj, aby dostosować ścieżki w swoich testach i skryptach do nowej struktury projektu.
