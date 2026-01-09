# 2048

## About the Project

Python implementation of the popular 2048 puzzle game with a tkinter GUI.

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
cd Proste-Projekty/src/python/2048
```

3. Start the app:

```Bash
python src/main.py
```

## Rules of the game

1. Use arrow keys to slide tiles in that direction.
2. When two tiles with the same number touch, they merge into one.
3. The goal is to create a tile with the number 2048.
4. The game ends when no more moves are possible.

## Features

* Arrow key controls.
* Score tracking.
* Colored tiles based on value.
* Game over detection.
* Restart functionality.

## Possible improvements

Some of the ideas include:

* Add undo functionality.
* Add high score tracking.
* Add animations for tile movements.

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
docker build -t 2048-app .
docker run 2048-app
```

## Directory structure

```
2048/
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
