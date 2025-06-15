# Minesweeper

## About the Project

Python implementation of the classic game of minesweeper.

## Screenshots

![minesweeper](https://user-images.githubusercontent.com/37275728/194823180-a96946b2-082e-4aac-85cd-e822b6cf58c4.gif)

## Requirements

To run this project locally you will need:

* Python 3.8+

No additional libraries or packagaes are needed!

## Installation

1. Download the code repository from GitHub: 
    
```Bash
git clone https://github.com/djeada/Proste-Projekty.git
```

2. Navigate to the appropriate directory:

```Bash
cd Proste-Projekty/src/python/minesweeper
```

3. Start the app:

```Bash
python src/main.py
```

## Rules of the game

1. A 2D grid of squares is shown.
1. The player is aware of the number of squares containing mines, but not of their location.
1. The goal of the game is to find all of the squares that do not have a mine in them.
1. If the player attempts to expose a mined square, the game is lost.
1. The player wins when the last safe square is revealed. 

## Features

* Timer.
* Mines counter.
* Option to reveal or mark a square as armed.

## Possible improvements

Some of the ideas include:

* The high scores table.
* The option of choosing the level of difficulty.
* An AI alogrithm that solves the minesweeper automatically.

## Development

For development, testing and deployment the following tools are used:

- Docker
- Docker Compose
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
nuitka --standalone --onefile src/minesweeper/main.py -o app.bin
```

### Docker deployment

To deploy the application using Docker, build and run the Docker image:

```sh
docker build -t minesweeper-app .
docker run minesweeper-app
```

## Best practices

- Keep source code in `src/`
- Keep tests in `tests/`
- Use a linter and code formatter before committing
- Automate testing and linting in CI

## Directory structure

```
minesweeper/
├── src/
│   └── minesweeper/
│       ├── main.py
│       ├── logic/
│       └── gui/
├── tests/
│   └── test_board.py
├── setup.py
├── Dockerfile
└── README.md
```

## Sample code

### src/minesweeper/logic/board.py

```python
# ...existing game board code...
```

### tests/test_board.py

```python
# ...existing test code...
```
