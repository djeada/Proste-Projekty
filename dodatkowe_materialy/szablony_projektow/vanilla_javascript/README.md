# Szablon projektu Vanilla JavaScript

## Opis
Ten szablon pozwala szybko rozpocząć projekt frontendowy w czystym JavaScript z automatyzacją, testami i dobrymi praktykami.

## Funkcje
- Modularna struktura (`src/js`, `src/css`, `tests`)
- Lintowanie (ESLint)
- Formatowanie kodu (Prettier)
- Testy jednostkowe (Jest)
- Automatyzacja CI (GitHub Actions)
- Deployment przez Docker
- Spójny styl kodu (EditorConfig)

## Wymagania
- Node.js >= 18
- npm
- Docker (opcjonalnie)

## Instalacja i uruchomienie
```sh
npm install
npm run lint
npm test
```

## Budowanie (jeśli dotyczy)
```sh
npm run build
```

## Deployment
Budowa i uruchomienie obrazu Docker:
```sh
docker build -t moj-js-app .
docker run -p 8080:8080 moj-js-app
```

## Dobre praktyki
- Kod JavaScript w `src/js/`
- Style w `src/css/`
- Testy w `tests/`
- Używaj lintera i formatowania przed commitem
- Automatyzuj testy i lint w CI

## Struktura katalogów
```
projekt_frontend_js/
├── src/
│   ├── js/
│   │   └── script.js
│   ├── css/
│   │   └── style.css
│   └── index.html
├── tests/
│   └── example.test.js
├── package.json
├── Dockerfile
└── README.md
```
