# Simple Calculator in C
This is a basic calculator program written in C that performs addition, subtraction, multiplication, and division. The program operates in a loop, continuously prompting the user to enter a mathematical expression or to quit. It can handle floating point and integer numbers, providing a simple and interactive command-line interface for basic arithmetic operations.

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

### Compiling the Program
To compile and run the program, follow these steps:
1. Ensure you have a C compiler like GCC installed on your system.
2. Copy the source code into a file, for example `calculator.c`.
3. Compile the program using the following command:

    ```
    gcc calculator.c -o calculator
    ```

4. Run the compiled program:

    ```
    ./calculator
    ```

## Customization
- The program can be modified to include more complex mathematical functions.
- Error handling can be enhanced to manage more types of invalid input.
- The user interface can be further developed for a more robust interactive experience.
