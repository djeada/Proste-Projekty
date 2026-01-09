# Graphics Editor

## About the Project

Python implementation of a simple graphics editor with a tkinter GUI.

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
cd Proste-Projekty/src/python/graphics_editor
```

3. Start the app:

```Bash
python src/main.py
```

## Features

* Drawing tools (pencil, line, rectangle, circle).
* Color selection.
* Fill tool.
* Clear canvas.
* Save/load images (PPM format).

## Possible improvements

Some of the ideas include:

* Add more drawing tools.
* Add layers support.
* Add filters and effects.
* Add undo/redo functionality.

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
docker build -t graphics-editor-app .
docker run graphics-editor-app
```

## Directory structure

```
graphics_editor/
├── src/
│   ├── main.py
│   ├── logic/
│   │   └── image.py
│   └── gui/
│       └── gui.py
├── tests/
│   └── test_image.py
├── setup.py
├── Dockerfile
└── README.md
```
