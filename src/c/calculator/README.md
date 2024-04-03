# Simple Calculator in C
This is a basic calculator program written in C that performs addition, subtraction, multiplication, and division. The program operates in a loop, continuously prompting the user to enter a mathematical expression or to quit. It can handle floating point and integer numbers, providing a simple and interactive command-line interface for basic arithmetic operations.

![calculator](https://github.com/djeada/Proste-Projekty/assets/37275728/e62d057f-bb26-4409-8664-83e7323e1d86)

## How to Use
- Run the program; it will prompt you to enter an expression or 'q' to quit.
- Enter an expression in the format: `number operator number` (e.g., `3.5 * 2`).
- The program will display the result of the operation.
- To exit the program, enter 'q'.

## Features
- Supports basic arithmetic operations: addition (`+`), subtraction (`-`), multiplication (`*`), and division (`/`).
- Handles floating point and integer calculations.
- Includes input validation and error handling for division by zero and invalid inputs.
- Continuous operation until the user decides to exit.

## Installation

### Compiling the Game
To compile the game, follow these steps:
1. Clone or download the repository to your local machine.
2. Navigate to the directory containing the `main.c` file.
3. Compile the program using GCC:

```
gcc src/main.c -o calculator
```

4. Run the program:
- On Linux or macOS: `./calculator`
- On Windows: `calculator.exe`

## Customization
- The program can be modified to include more complex mathematical functions.
- Error handling can be enhanced to manage more types of invalid input.
- The user interface can be further developed for a more robust interactive experience.
