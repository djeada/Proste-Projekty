# Food Ordering

## About the Project

Python implementation of a food ordering system with menu management and order tracking.

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
cd Proste-Projekty/src/python/food_ordering
```

3. Start the app:

```Bash
python src/main.py
```

## Features

* Browse menu items by category.
* Add items to order.
* Set delivery address.
* Choose payment method.
* Track order status.

## Possible improvements

Some of the ideas include:

* Add user accounts.
* Add order history.
* Add restaurant selection.
* Add real payment integration.

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
docker build -t food-ordering-app .
docker run -it food-ordering-app
```

## Directory structure

```
food_ordering/
├── src/
│   ├── main.py
│   └── logic/
│       └── ordering.py
├── tests/
│   └── test_ordering.py
├── setup.py
├── Dockerfile
└── README.md
```
