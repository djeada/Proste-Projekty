# Timer

## About the Project

Python implementation of a simple timer/stopwatch application with a tkinter GUI.

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
cd Proste-Projekty/src/python/timer
```

3. Start the app:

```Bash
python src/main.py
```

## Features

* Start/Stop timer.
* Reset functionality.
* Display in HH:MM:SS format.

## Possible improvements

Some of the ideas include:

* Add countdown timer mode.
* Add lap time functionality.
* Add sound notification.

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
docker build -t timer-app .
docker run timer-app
```

## Directory structure

```
timer/
├── src/
│   ├── main.py
│   ├── logic/
│   │   └── timer.py
│   └── gui/
│       └── gui.py
├── tests/
│   └── test_timer.py
├── setup.py
├── Dockerfile
└── README.md
```
