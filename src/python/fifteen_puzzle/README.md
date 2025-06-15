# Fifteen Puzzle

## About the Project

A desktop application for fifteen puzzle game.

## Screenshots

![fifteen_puzzle](https://user-images.githubusercontent.com/37275728/194822577-fbfa5228-3643-4f61-ad69-bc58cd80b97a.gif)

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
cd Proste-Projekty/src/python/fifteen_puzzle
```

3. Start the app:

```Bash
python src/main.py
```

## Game rules

* Fifteen puzzle is a game where you are given a board and you need to move all the tiles to the empty space to get the board in order.
* At the start of the game, you are given a board with some tiles in random order.
* There is only one empty space on the board.
* You can move the tiles to the empty space by swapping them.
* Once all the tiles are in order, you win the game.

## Usage

1. Enter the text to be deciphered or ciphered into the message input.
2. Enter the offset using the key input.
3. Select whether the algorithm should cipher or decipher the message using the mode dropdown.
4. Press the Run button.
5. The processed message appears in the text area. 

## Features

* The ability to move the titles with the arrow keys.
* A board that displays the current state of the game.
* Tiles that are correctly placed are highlighted. 

## Possible improvements

Some of the ideas include:

* Display a special message when the user wins the game.
* Display the timer.

## Development

For development, testing, and deployment, the following tools and libraries are used:

- `pytest` for testing
- `flake8` for linting
- `black` for code formatting
- `Docker` for containerization

### Installation and Testing

To install the necessary dependencies and run tests, use the following commands:

```sh
pip install .[dev]
flake8 src/ tests/
black --check src/ tests/
pytest
```

### Building the Binary

To build a standalone binary of the application, use Nuitka with the following command:

```sh
nuitka --standalone --onefile src/fifteen_puzzle/main.py -o app.bin
```

### Deployment

To deploy the application using Docker, build and run the Docker image with these commands:

```sh
docker build -t fifteen-puzzle-app .
docker run fifteen-puzzle-app
```

## Best Practices

This project follows certain best practices for Python development:

- Source code is in the `src/` directory.
- Tests are in the `tests/` directory.
- Use a linter and code formatter before committing code.
- Automate testing and linting in CI/CD pipelines.

## Directory Structure

The project has the following directory structure:

```
fifteen_puzzle/
├── src/
│   └── fifteen_puzzle/
│       ├── main.py
│       ├── logic/
│       └── gui/
├── tests/
│   └── test_puzzle_board.py
├── setup.py
├── Dockerfile
└── README.md
```

## Sample Code

Here is some sample code from the project:

### src/fifteen_puzzle/logic/puzzle_board.py

```python
# ...existing game board code...
```

### tests/test_puzzle_board.py

```python
# ...existing test code...
```
