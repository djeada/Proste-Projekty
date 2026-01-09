# Snake

## About the Project

Python implementation of the classic Snake game with a pygame GUI.

## Requirements

To run this project locally you will need:

* Python 3.8+
* pygame

Install dependencies:

```Bash
pip install -r requirements.txt
```

## Installation

1. Download the code repository from GitHub:

```Bash
git clone https://github.com/djeada/Proste-Projekty.git
```

2. Navigate to the appropriate directory:

```Bash
cd Proste-Projekty/src/python/snake
```

3. Start the app:

```Bash
python src/main.py
```

## Rules of the game

1. Control the snake using arrow keys.
2. Eat food to grow longer.
3. Don't hit the walls or yourself.
4. The game ends when you collide with a wall or your own body.

## Features

* Arrow key controls.
* Score tracking.
* Game over detection.
* Restart functionality.

## Possible improvements

Some of the ideas include:

* Add difficulty levels with varying speeds.
* Add high score tracking.
* Add obstacles.

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
docker build -t snake-app .
docker run snake-app
```

## Directory structure

```
snake/
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
