#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

#define MAX_TASK_LEN 256
#define MAX_SQL_LEN 512

void initializeDatabase(sqlite3 **database);
void addTask(sqlite3 *database);
void viewTasks(sqlite3 *database);
void editTask(sqlite3 *database);
void removeTask(sqlite3 *database);
void changePriority(sqlite3 *database);

int main() {
    sqlite3 *database;
    char userChoice;

    initializeDatabase(&database);

    while (1) {
        printf("\nOptions: (a)dd, (v)iew, (e)dit, (r)emove, (p)riority, (q)uit\n");
        scanf(" %c", &userChoice);

        switch (userChoice) {
            case 'a':
                addTask(database);
                break;
            case 'v':
                viewTasks(database);
                break;
            case 'e':
                editTask(database);
                break;
            case 'r':
                removeTask(database);
                break;
            case 'p':
                changePriority(database);
                break;
            case 'q':
                sqlite3_close(database);
                exit(0);
            default:
                printf("Invalid choice.\n");
                break;
        }
    }

    return 0;
}

void initializeDatabase(sqlite3 **database) {
    int resultCode = sqlite3_open("todo_list.db", database);
    if (resultCode) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(*database));
        exit(1);
    } else {
        fprintf(stderr, "Opened database successfully\n");
    }

    // Create SQL table
    char *sql = "CREATE TABLE IF NOT EXISTS TASKS("
                "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                "TASK TEXT NOT NULL,"
                "PRIORITY INTEGER NOT NULL);";

    char *errorMessage = 0;
    resultCode = sqlite3_exec(*database, sql, 0, 0, &errorMessage);
    if (resultCode != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errorMessage);
        sqlite3_free(errorMessage);
    } else {
        fprintf(stdout, "Table created successfully\n");
    }
}

void addTask(sqlite3 *database) {
    char taskDescription[MAX_TASK_LEN];
    int priority;
    printf("Enter task description: ");
    scanf("%s", taskDescription);
    printf("Enter task priority (integer): ");
    scanf("%d", &priority);

    char sql[MAX_SQL_LEN];
    sprintf(sql, "INSERT INTO TASKS (TASK,PRIORITY) VALUES ('%s', %d);", taskDescription, priority);

    char *errorMessage = 0;
    int resultCode = sqlite3_exec(database, sql, 0, 0, &errorMessage);
    if (resultCode != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errorMessage);
        sqlite3_free(errorMessage);
    } else {
        fprintf(stdout, "Task added successfully\n");
    }
}

void viewTasks(sqlite3 *database) {
    const char *sql = "SELECT ID, TASK, PRIORITY FROM TASKS ORDER BY PRIORITY DESC;";
    sqlite3_stmt *statement;

    if (sqlite3_prepare_v2(database, sql, -1, &statement, NULL) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(database));
        return;
    }

    printf("\nID | TASK | PRIORITY\n");
    printf("--------------------\n");

    while (sqlite3_step(statement) == SQLITE_ROW) {
        int taskId = sqlite3_column_int(statement, 0);
        const char *taskDescription = (const char *)sqlite3_column_text(statement, 1);
        int priority = sqlite3_column_int(statement, 2);
        printf("%d | %s | %d\n", taskId, taskDescription, priority);
    }

    sqlite3_finalize(statement);
}

void editTask(sqlite3 *database) {
    int taskId;
    char newTaskDescription[MAX_TASK_LEN];
    int newPriority;
    printf("Enter the ID of the task you want to edit: ");
    scanf("%d", &taskId);
    printf("Enter new task description: ");
    scanf("%s", newTaskDescription);
    printf("Enter new task priority: ");
    scanf("%d", &newPriority);

    char sql[MAX_SQL_LEN];
    sprintf(sql, "UPDATE TASKS SET TASK = '%s', PRIORITY = %d WHERE ID = %d;", newTaskDescription, newPriority, taskId);

    char *errorMessage = 0;
    if (sqlite3_exec(database, sql, 0, 0, &errorMessage) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errorMessage);
        sqlite3_free(errorMessage);
    } else {
        printf("Task updated successfully.\n");
    }
}

void removeTask(sqlite3 *database) {
    int taskId;
    printf("Enter the ID of the task you want to remove: ");
    scanf("%d", &taskId);

    char sql[MAX_TASK_LEN];
    sprintf(sql, "DELETE FROM TASKS WHERE ID = %d;", taskId);

    char *errorMessage = 0;
    if (sqlite3_exec(database, sql, 0, 0, &errorMessage) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errorMessage);
        sqlite3_free(errorMessage);
    } else {
        printf("Task removed successfully.\n");
    }
}

void changePriority(sqlite3 *database) {
    int taskId;
    int newPriority;
    printf("Enter the ID of the task you want to change the priority of: ");
    scanf("%d", &taskId);
    printf("Enter the new priority: ");
    scanf("%d", &newPriority);

    char sql[MAX_TASK_LEN];
    sprintf(sql, "UPDATE TASKS SET PRIORITY = %d WHERE ID = %d;", newPriority, taskId);

    char *errorMessage = 0;
    if (sqlite3_exec(database, sql, 0, 0, &errorMessage) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errorMessage);
        sqlite3_free(errorMessage);
    } else {
        printf("Priority changed successfully.\n");
    }
}
