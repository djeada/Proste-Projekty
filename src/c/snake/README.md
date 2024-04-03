# Snake Game in C using ncurses

This is a simple implementation of the classic Snake game in C, utilizing the ncurses library for handling input and output on the terminal. The player controls a snake, guiding it to eat food and grow in length, while avoiding colliding with the walls or itself.

![snake](https://github.com/djeada/Proste-Projekty/assets/37275728/866797cf-1472-42fd-aca5-c492a684ae44)

## Prerequisites

To run this game, you need to have the ncurses library installed on your system. 

### Installing ncurses on Linux

You can install ncurses on a Debian-based system (like Ubuntu) using:

```
sudo apt-get install libncurses5-dev libncursesw5-dev
```

On Red Hat-based systems (like Fedora), use:

```
sudo yum install ncurses-devel
```

### Installing ncurses on macOS

If you are using macOS and have Homebrew installed, you can install ncurses with the following command:

```
brew install ncurses
```

## Compilation

To compile the game, use the following command:

```
gcc -o snake src/main.c -lncurses 
```

This will create an executable named `snake`.

## Running the Game

Run the game by executing the compiled binary:

```
./snake
```

## Game Controls

- Use the arrow keys to control the direction of the snake.
- The game will end if the snake collides with the wall or itself.
- Every time the snake eats the food, it grows in length.

## Features

- Simple and intuitive gameplay.
- Dynamic snake growth upon eating food.
- Collision detection with walls and self.

Enjoy the game!
