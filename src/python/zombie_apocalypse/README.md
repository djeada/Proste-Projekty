# Zombie Apocalypse

## About the Project

This project is a zombie apocalypse survival game developed using Pygame. Players must navigate through a post-apocalyptic world filled with zombies, using the arrow keys to control their character.

## Screenshots

![zombie_apocalypse](https://user-images.githubusercontent.com/37275728/188334905-179b94fd-eec2-44b8-a64f-fecdd6c6ea01.gif)

## Requirements

To run this project locally, you will need:

* Python 3.8+
* Pygame library

No additional libraries or packagaes are needed!

## Installation

1. Download the code repository from GitHub:

```Bash
git clone https://github.com/djeada/Proste-Projekty.git
```

2. Navigate to the appropriate directory:

```Bash
cd Proste-Projekty/src/python/zombie_apocalypse
```

3. Start the app:

```Bash
python src/main.py
```

## Usage

1. Use the arrow keys to move your character around the game world.
2. Avoid zombies and gather resources to survive.
3. Explore different areas to find supplies and weapons.

## Features

* Responsive arrow key movement control for character navigation.
* Score system.
* Automatic generation of zombies.

## Possible improvements

Some of the ideas include:

* Adding more types of zombies.
* Implementing a crafting system to create weapons and tools.
* Introducing a day-night cycle that affects zombie behavior and player visibility.

## Development

For those who wish to contribute to the project or run it in a development environment, please ensure you have the following installed:

* Python >= 3.10
* pip
* pygame
* Docker (optional)

### Installation and Testing

To set up the development environment and run tests, use the following commands:

```sh
pip install .[dev]
flake8 src/ tests/
black --check src/ tests/
pytest
```

### Building the Binary

To build a standalone binary of the game, use Nuitka with the following command:

```sh
nuitka --standalone --onefile src/zombie_apocalypse/main.py -o app.bin
```

### Deployment

To build and run the Docker image for the application, use the following commands:

```sh
docker build -t zombie-apocalypse-app .
docker run zombie-apocalypse-app
```

## Best Practices

When working on this project, please adhere to the following best practices:

* Keep the source code in the `src/` directory.
* Place tests in the `tests/` directory.
* Use a linter and code formatter before committing changes.
* Automate testing and linting in your continuous integration (CI) pipeline.

## Directory Structure

The project follows this directory structure:

```
zombie_apocalypse/
├── src/
│   └── zombie_apocalypse/
│       ├── main.py
│       ├── entities/
│       ├── game/
│       ├── settings/
│       └── utils/
├── tests/
│   └── test_zombie_apocalypse.py
├── setup.py
├── Dockerfile
└── README.md
```

## Sample Code

Here is some sample code from the project:

### src/zombie_apocalypse/game/...

```python
# ...existing game code...
```

### tests/test_zombie_apocalypse.py

```python
# ...existing test code...
```
