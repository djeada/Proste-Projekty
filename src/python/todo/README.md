# Todo

## About the Project

Python implementation of a simple command-line todo list application.

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
cd Proste-Projekty/src/python/todo
```

3. Start the app:

```Bash
python src/main.py
```

## Features

* Add tasks to the list.
* Remove tasks by index.
* View all tasks.
* Simple command-line interface.

## Possible improvements

Some of the ideas include:

* Add task priorities.
* Add due dates.
* Save tasks to a file.
* Add task completion status.

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
docker build -t todo-app .
docker run -it todo-app
```

## Directory structure

```
todo/
├── src/
│   ├── main.py
│   └── logic/
│       └── todo_list.py
├── tests/
│   └── test_todo.py
├── setup.py
├── Dockerfile
└── README.md
```
