# Messenger

## About the Project

Python implementation of a simple chat messenger with client/server architecture.

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
cd Proste-Projekty/src/python/messenger
```

3. Start the server:

```Bash
python src/main.py server
```

4. In another terminal, start the client:

```Bash
python src/main.py client
```

## Features

* Server/client architecture.
* Send and receive messages.
* Multiple client support.
* Username identification.

## Possible improvements

Some of the ideas include:

* Add encryption.
* Add user authentication.
* Add message history.
* Add file sharing.

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
docker build -t messenger-app .
docker run -p 8888:8888 messenger-app server
```

## Directory structure

```
messenger/
├── src/
│   ├── main.py
│   └── logic/
│       └── messenger.py
├── tests/
│   └── test_messenger.py
├── setup.py
├── Dockerfile
└── README.md
```
