# To-Do List Manager in C with SQLite
This is a simple C program that allows users to manage their to-do tasks using a SQLite database. It supports various operations like adding, viewing, editing, and removing tasks, along with the ability to change their priority.

## How to Use
- Run the program, and it will prompt you with options for different operations.
- Choose the appropriate option to add, view, edit, remove, or change the priority of tasks.
- The tasks are stored in a SQLite database file named `todo_list.db`.

## Installation

### Compiling the Program
To compile the program with SQLite support, follow these steps:
1. Clone or download the repository to your local machine.
2. Ensure SQLite3 is installed on your system. You can download it from [SQLite's official site](https://www.sqlite.org/download.html).
3. Navigate to the directory containing the `todo_list.c` file.
4. Compile the program using GCC and link the SQLite library:

```
gcc todo_list.c -o todo_list -lsqlite3
```

5. Run the program:

```
./todo_list
```

## Features
- **Add Task**: Add new tasks with a description and priority.
- **View Tasks**: Display all the tasks sorted by their priority.
- **Edit Task**: Modify the description or priority of an existing task.
- **Remove Task**: Delete a task from the list.
- **Change Priority**: Update the priority of a specific task.

## Customization
- The database schema can be modified to include additional fields like due dates, categories, or notes.
- The user interface can be enhanced for a more interactive experience.
- The application logic can be expanded to include more complex features such as task dependencies or recurring tasks.
