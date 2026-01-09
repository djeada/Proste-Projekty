# Shooting Ducks

## About the Project

Python implementation of a duck shooting game with a pygame GUI.

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
cd Proste-Projekty/src/python/shooting_ducks
```

3. Start the app:

```Bash
python src/main.py
```

## Rules of the game

1. Ducks move across the screen.
2. Click on ducks to shoot them.
3. Score points for each duck hit.
4. Progress through levels with increasing difficulty.
5. Game ends when you run out of lives.

## Features

* Moving targets (ducks).
* Score tracking.
* Multiple levels.
* Lives/health system.
* Pause functionality.

## Possible improvements

Some of the ideas include:

* Add sound effects.
* Add different duck types.
* Add power-ups.
* Add animations.

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
docker build -t shooting-ducks-app .
docker run shooting-ducks-app
```

## Directory structure

```
shooting_ducks/
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
