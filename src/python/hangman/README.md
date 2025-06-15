# Hangman

## About the Project

Hangman is a single-player guessing game. The computer chooses a word at random from a predefined list. The plaer makes an attempt to guess it by suggesting letters within a certain number of guesses.

## Screenshots

![hangman](https://user-images.githubusercontent.com/37275728/194822831-d1b117cb-ae01-4939-bac1-85ac4e58769a.gif)

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

## Rules of the game

1. The word is chosen randomly from the list of words.
2. The player has to guess the word.
3. There are n squares drawn on the board, where n is the length of the word.
4. When the player guesses a letter, the squares with the letter are revealed.
5. If the player guesses the word, he wins.
6. If the player guesses a letter that is not in the word, an image from a list of images is drawn.

## Features

* Single player mode.
* A display showing the total number of characters and letters that were successfully predicted. 
* An image representing the current state of the hangman.
* An input box in which the user can type the guessed letters. 

## Possible improvements

There are many ways in which the game could be expanded or improved in the future. Some of the ideas include:

* Add mode for two players.
* Allow the user to upload a list of possible words.

## Development

For those who wish to contribute to the development of Hangman, or to modify it for personal use, the following tools and practices are recommended:

- **Code Linting**: Use `flake8` to check the code for potential errors and enforce a consistent style.
- **Code Formatting**: Use `black` to automatically format the code to adhere to Python's PEP 8 style guide.
- **Testing**: Use `pytest` to run the unit tests and ensure that the code is working as expected.
- **Docker**: Optionally, you can use Docker to build and run the application in a containerized environment.

### Installation and Testing

To install the necessary development dependencies and run the tests, use the following commands:

```sh
pip install .[dev]
flake8 src/ tests/
black --check src/ tests/
pytest
```

### Building a Standalone Executable

To build a standalone executable binary of the Hangman game, you can use Nuitka, a Python-to-C++ compiler. Run the following command:

```sh
nuitka --standalone --onefile src/hangman/main.py -o app.bin
```

### Docker Deployment

To deploy the Hangman game as a Docker container, you can build and run the Docker image using the following commands:

```sh
docker build -t hangman-app .
docker run hangman-app
```

## Best Practices

When working with the Hangman codebase, or any Python project, it's important to follow best practices for code quality and project structure:

- Keep the source code in the `src/` directory.
- Place tests in the `tests/` directory.
- Use a linter and code formatter before committing changes.
- Automate testing and linting in your continuous integration (CI) pipeline.

## Directory Structure

The recommended directory structure for the Hangman project is as follows:

```
hangman/
├── src/
│   └── hangman/
│       ├── main.py
│       ├── logic/
│       └── gui/
├── tests/
│   └── test_hangman.py
├── setup.py
├── Dockerfile
└── README.md
```

## Sample Code

Here are some snippets of the existing code in the Hangman project:

### src/hangman/logic/config.py

```python
# ...existing configuration code...
```

### tests/test_hangman.py

```python
# ...existing test code...
```
