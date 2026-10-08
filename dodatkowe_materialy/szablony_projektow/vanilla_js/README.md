# Szablon projektu w JavaScripcie (przeglądarka)

Najmniejszy kompletny projekt strony w czystym JavaScripcie: bez frameworków, bez bundlera i bez żadnych pakietów npm. Logika jest oddzielona od obsługi strony, a testy działają na wbudowanym w Node.js module `node:test`. Przykładem jest przelicznik temperatur — zamień go na swój program.

## Struktura

```
.
├── src/
│   ├── index.html          strona: formularz i miejsce na wynik
│   ├── style.css           wygląd strony
│   ├── converter.js        logika: same obliczenia, bez dostępu do strony (DOM)
│   └── main.js             obsługa strony: zdarzenia, odczyt formularza, wyświetlanie wyniku
├── tests/
│   └── converter.test.js   testy logiki (node:test)
├── package.json            nazwa projektu i polecenie `npm test`
├── .github/workflows/ci.yml  GitHub Actions: testy
├── .editorconfig           wcięcia i kodowanie dla edytora
└── .gitignore              pliki, których nie dodajemy do repozytorium
```

Najważniejsza zasada: **logika nie wie nic o stronie**. Funkcje w `converter.js` dostają liczby i zwracają wynik — nie sięgają do `document`. Dzięki temu testy mogą je uruchomić w Node.js, gdzie nie ma przeglądarki, a ten sam plik można wykorzystać w innym interfejsie, np. w programie konsolowym.

## Jak to działa

- `index.html` wczytuje najpierw `converter.js`, potem `main.js`, zwykłymi znacznikami `<script>`. Funkcje zadeklarowane w pierwszym pliku są widoczne w drugim. Dzięki temu stronę można otworzyć bezpośrednio z dysku (dwuklik na `index.html`). Moduły ES (`type="module"`) wymagają serwera, bo przeglądarki blokują je przy adresach `file://`.
- Ostatnia linia `converter.js` eksportuje funkcje tylko wtedy, gdy istnieje obiekt `module`, czyli w Node.js. Przeglądarka ją pomija, a testy mogą zrobić `require('../src/converter.js')`.
- `main.js` nasłuchuje zdarzenia `submit` formularza, wywołuje `event.preventDefault()` (żeby strona się nie przeładowała), odczytuje pola, woła logikę i wpisuje wynik do akapitu `#result`.
- `npm test` uruchamia `node --test`, które samo znajduje pliki `*.test.js`. `assert.equal` porównuje wartości, a `assert.throws` sprawdza, czy funkcja zgłasza błąd.

## Wymagania

- dowolna nowoczesna przeglądarka
- Node.js 18 lub nowszy (tylko do testów)

## Uruchamianie

Otwórz `src/index.html` w przeglądarce.

## Testy

```sh
npm test
```

## Jak zacząć własny projekt

1. Skopiuj ten katalog i zmień nazwę `converter` na nazwę swojego projektu (pliki, `package.json`, znaczniki `<script>`).
2. Najpierw napisz logikę i testy do niej, potem obsługę strony w `main.js`.
3. Jeśli program ma działać w terminalu, a nie w przeglądarce, usuń `index.html`, `style.css` i `main.js`, a w `src/main.js` napisz program dla Node.js (`node src/main.js`), np. z modułem `readline`.
4. Gdy projekt urośnie, podziel logikę na kilka plików i wczytaj je kolejno w `index.html`.
5. Uzupełnij README: co robi program, jak go uruchomić, jak działa i jak go testować.
