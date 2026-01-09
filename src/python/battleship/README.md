# Battleship

## About the Project

Python implementation of the classic Battleship game with a tkinter GUI.

## Requirements

To run this project locally you will need:

* Python 3.8+

No additional libraries or packages are needed!

## Installation

1. Download the code repository from GitHub:

```Bash
git clone https://github.com/djeada/Proste-Projekty.git
```

2. Navigate to the appropriate directory:

```Bash
cd Proste-Projekty/src/python/battleship
```

3. Start the app:

```Bash
python src/main.py
```

## Rules of the game

1. Each player has a 10x10 grid with ships placed on it.
2. Players take turns guessing coordinates to fire at.
3. A hit is marked on the opponent's grid.
4. The first player to sink all opponent ships wins.

## Features

* Play against AI.
* Ship placement phase.
* Visual hit/miss indicators.
* Ship sinking detection.

## Possible improvements

Some of the ideas include:

* Add smarter AI.
* Add network multiplayer.
* Add different ship configurations.
* Add sound effects.

## Development

For development, testing and deployment the following tools are used:

- Docker
- Python 3.10+
- pip

### Local development

1. Install dependencies:

```sh
pip install .[dev]
```

2. Run linters and tests:

```sh
flake8 src/ tests/
black --check src/ tests/
pytest
```

### Build binary

To build a standalone binary of the application, use Nuitka:

```sh
nuitka --standalone --onefile src/main.py -o app.bin
```

### Docker deployment

To deploy the application using Docker, build and run the Docker image:

```sh
docker build -t battleship-app .
docker run battleship-app
```

## Directory structure

```
battleship/
├── src/
│   ├── main.py
│   ├── logic/
│   │   └── game.py
│   └── gui/
│       └── gui.py
├── tests/
│   └── test_game.py
├── setup.py
├── Dockerfile
└── README.md
```
