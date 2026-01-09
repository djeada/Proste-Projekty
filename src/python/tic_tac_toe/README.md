# Tic-Tac-Toe

## About the Project

Python implementation of the classic Tic-Tac-Toe game with a tkinter GUI.

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
cd Proste-Projekty/src/python/tic_tac_toe
```

3. Start the app:

```Bash
python src/main.py
```

## Rules of the game

1. A 3x3 grid is shown.
2. Two players take turns marking spaces with X or O.
3. The first player to get 3 marks in a row (horizontally, vertically, or diagonally) wins.
4. If all 9 squares are filled without a winner, the game is a draw.

## Features

* Play against AI or another player.
* Visual indication of winning line.
* Game restart functionality.

## Possible improvements

Some of the ideas include:

* Implement minimax AI for unbeatable computer opponent.
* Add score tracking across multiple games.
* Add network multiplayer support.

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
docker build -t tic-tac-toe-app .
docker run tic-tac-toe-app
```

## Directory structure

```
tic_tac_toe/
├── src/
│   ├── main.py
│   ├── logic/
│   │   └── board.py
│   └── gui/
│       └── gui.py
├── tests/
│   └── test_board.py
├── setup.py
├── Dockerfile
└── README.md
```
