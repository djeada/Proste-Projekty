# HTTP Server

## About the Project

Python implementation of a simple HTTP server.

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
cd Proste-Projekty/src/python/http_server
```

3. Start the server:

```Bash
python src/main.py
```

4. Open a browser and navigate to `http://localhost:8080`

## Features

* Simple HTTP GET request handling.
* Responds with "Hello, world!" message.
* Configurable port.

## Possible improvements

Some of the ideas include:

* Add routing support.
* Add POST request handling.
* Add static file serving.
* Add request logging.

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
docker build -t http-server-app .
docker run -p 8080:8080 http-server-app
```

## Directory structure

```
http_server/
├── src/
│   ├── main.py
│   └── logic/
│       └── server.py
├── tests/
│   └── test_server.py
├── setup.py
├── Dockerfile
└── README.md
```
