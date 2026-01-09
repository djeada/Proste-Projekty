# Shopping Cart

## About the Project

Python implementation of a simple shopping cart system with product management.

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
cd Proste-Projekty/src/python/shopping_cart
```

3. Start the app:

```Bash
python src/main.py
```

## Features

* Add products to a store catalog.
* Add/remove items from cart.
* Update quantities.
* Apply discounts.
* Calculate totals.
* Checkout functionality.

## Possible improvements

Some of the ideas include:

* Add product categories.
* Add product search.
* Save cart to file.
* Add multiple payment methods.

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
docker build -t shopping-cart-app .
docker run -it shopping-cart-app
```

## Directory structure

```
shopping_cart/
├── src/
│   ├── main.py
│   └── logic/
│       └── cart.py
├── tests/
│   └── test_cart.py
├── setup.py
├── Dockerfile
└── README.md
```
