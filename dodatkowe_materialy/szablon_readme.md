# Szablon pliku README

README to pierwsza rzecz, którą widzi osoba otwierająca projekt. Powinien w kilka minut odpowiedzieć na trzy pytania: **co to jest**, **jak to uruchomić** i **jak to działa**. Poniżej jest szablon z opisem, co wpisać w każdej sekcji.

W tym repozytorium README projektów piszemy po angielsku, żeby można je było dołączyć do projektu w dowolnym miejscu. Nazwy sekcji są podane w nawiasach. Gotowe przykłady: [Terminal Effects (C)](../src/c/terminal_effects/README.md) i README w każdym katalogu w [`src/`](../src/).

---

```markdown
# Nazwa projektu (język)

Dwa, trzy zdania: co to jest i co robi.

![Screenshot](screenshot.png)

## Funkcje (Features)

- najważniejsze możliwości programu, po jednej w punkcie

## Jak używać (How to play / How to use)

Sterowanie, polecenia, przykładowa sesja. Najlepiej konkretny przykład:
wpisujesz X, program odpowiada Y.

## Jak to działa (How it works)

Najważniejsza sekcja dla osoby, która się uczy. Opisz:
- jakie dane przechowuje program i w jakich strukturach (tablica 4×4, słownik, lista obiektów),
- zasady lub algorytm krok po kroku (np. jak przesuwają się kafelki w 2048),
- jak działa pętla programu: odczyt wejścia → zmiana stanu → rysowanie,
- wszystko, co nie jest oczywiste, z nazwami funkcji i plików.

## Struktura projektu (Project layout)

    src/
      game.c      logika gry
      main.c      interfejs w terminalu
    tests/
      test_game.c testy logiki

Każdy plik z jednym zdaniem: do czego służy.

## Wymagania (Requirements)

Kompilator / interpreter i jego wersja, biblioteki, system operacyjny.

## Uruchamianie (Run)

Dokładne polecenia, które można skopiować i wkleić.

## Testy (Test)

Polecenie uruchamiające testy i jedno zdanie o tym, co sprawdzają.

## Porównanie z innymi wersjami (Comparison with the other versions)

Linki do wersji w innych językach i tabela: rodzaj interfejsu, liczba linii
logiki i interfejsu, liczba testów. Do tego kilka zdań o najciekawszych różnicach.

## Pomysły na rozbudowę (Ideas for extensions)

3–5 pomysłów dla osoby, która chce rozwinąć projekt.
```

---

## Wskazówki

- **Zrzut ekranu** pokazuje program w trakcie działania (rozgrywka, wypełniona lista), a nie pusty ekran startowy.
- **Polecenia** testuj przed wpisaniem do README: skopiuj je do czystego terminala i sprawdź, czy działają.
- **Krótko i konkretnie.** Bez ogólników w stylu „innowacyjne rozwiązanie”. Każde zdanie ma coś wyjaśniać.
- **Aktualność.** Gdy zmieniasz kod, popraw README w tym samym commicie.
