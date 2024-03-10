
## Struktura Projektu Frontend JavaScript

- projekt_frontend_js/
  - src/
    - assets/
      - style.css
    - index.html
    - index.js
  - dist/ (katalog wyjściowy po budowaniu)
  - tests/
    - index.test.js
  - package.json
  - webpack.config.js (lub odpowiednik dla Parcel, Rollup itd.)
  - README.md
  - LICENSE
  - .gitignore

## src/index.html

Twoja strona główna HTML. Tutaj załączasz swój wyjściowy plik JavaScript:

<!DOCTYPE html>
<html lang="pl">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Przykładowa Strona</title>
</head>
<body>
    <h1>Witaj Świecie!</h1>
    <script src="main.js"></script>
</body>
</html>

## src/index.js

Główny plik JavaScript Twojej aplikacji:

import './assets/style.css';

function sayHello() {
    console.log('Hello, World!');
}

sayHello();

## src/assets/style.css

Twoje style CSS:

body {
    font-family: Arial, sans-serif;
    background-color: #f4f4f4;
    color: #333;
}

## webpack.config.js

Konfiguracja Webpacka:

const path = require('path');

module.exports = {
    entry: './src/index.js',
    output: {
        filename: 'main.js',
        path: path.resolve(__dirname, 'dist'),
    },
    module: {
        rules: [
            {
                test: /\.css$/,
                use: ['style-loader', 'css-loader'],
            },
        ],
    },
};

## package.json

Konfiguracja zależności projektu i skryptów:

{
  "name": "projekt_frontend_js",
  "version": "1.0.0",
  "main": "index.js",
  "scripts": {
    "build": "webpack --mode production",
    "dev": "webpack --mode development",
    "test": "echo \"Error: no test specified\" && exit 1"
  },
  "devDependencies": {
    "css-loader": "^5.2.6",
    "style-loader": "^2.0.0",
    "webpack": "^5.36.2",
    "webpack-cli": "^4.7.0"
  }
}

## Budowanie i Uruchamianie

Aby zbudować projekt:

npm run build

Aby uruchomić w trybie deweloperskim:

npm run dev

## Testowanie

Dla testowania możesz dodać narzędzia takie jak Jest. Konfiguracja zależy od potrzeb Twojego projektu.
