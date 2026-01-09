# Version Control

## About the Project

Python implementation of a simple version control system for file tracking.

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
cd Proste-Projekty/src/python/version_control
```

3. Start the app:

```Bash
python src/main.py
```

## Features

* Initialize a repository.
* Commit file snapshots.
* View commit history.
* Checkout previous versions.
* Compare different commits.

## Possible improvements

Some of the ideas include:

* Add branching support.
* Add merge functionality.
* Add diff visualization.
* Add remote repository support.

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
docker build -t version-control-app .
docker run -it version-control-app
```

## Directory structure

```
version_control/
├── src/
│   ├── main.py
│   └── logic/
│       └── repository.py
├── tests/
│   └── test_repository.py
├── setup.py
├── Dockerfile
└── README.md
```
