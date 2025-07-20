# Calculator

## About the Project

A simple calculator that can perform basic arithmetic operations.
The user can enter numbers and operators, and the result will be displayed if the expression is valid.

## Screenshots

![calculator](https://user-images.githubusercontent.com/37275728/194822287-7b84368a-2df0-4f4f-87a0-31951b91a253.gif)

## Requirements

To run this project locally you will need:

* Python 3.8+

No additional libraries or packagaes are needed!

## Installation

1. Download the code repository from GitHub: 
    
```Bash
git clone https://github.com/djeada/Proste-Projekty.git
```

2. Navigate to the appropriate directory:

```Bash
cd Proste-Projekty/src/python/calculator
```

3. Start the app:

```Bash
python src/main.py
```

## Usage

1. Enter the mathematical expression to be evaluated.
2. Press the equals sign button.
3. The result will be displayed if the provided expression is valid.

## Features

* The ability to enter mathematical expressions via buttons or the keyboard.
* Addition, subtraction, multiplication, division, exponentiation, square root are all supported. 

## Possible improvements

Some of the ideas include:

* Support for more advanced mathematical operations, like sine function evaluation etc.

## Development

For those who want to contribute or modify the project, here are some development details:

### Prerequisites

- Python >= 3.10
- pip
- Docker (optional)

### Installation and Testing

```sh
pip install .[dev]
flake8 src/ tests/
black --check src/ tests/
pytest
```

### Building the Binary

```sh
nuitka --standalone --onefile src/calculator/main.py -o app.bin
```

### Deployment

To build and run the Docker image:

```sh
docker build -t calculator-app .
docker run calculator-app
```

## Best Practices

- Keep source code in `src/`
- Keep tests in `tests/`
- Use a linter and formatter before committing code
- Automate tests and linting in CI

## Directory Structure

```
calculator/
├── src/
│   └── calculator/
│       ├── main.py
│       ├── logic/
│       └── gui/
├── tests/
│   └── test_calculator.py
├── setup.py
├── Dockerfile
└── README.md
```

