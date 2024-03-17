#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

void initializeDatabase(sqlite3 **db);
void addTask(sqlite3 *db);
void viewTasks(sqlite3 *db);
void editTask(sqlite3 *db);
void removeTask(sqlite3 *db);
void changePriority(sqlite3 *db);

int main() {
    sqlite3 *db;
    char choice;

    initializeDatabase(&db);

    while (1) {
        printf("\nOptions: (a)dd, (v)iew, (e)dit, (r)emove, (p)riority, (q)uit\n");
        scanf(" %c", &choice);

        switch (choice) {
            case 'a':
                addTask(db);
                break;
            case 'v':
                viewTasks(db);
                break;
            case 'e':
                editTask(db);
                break;
            case 'r':
                removeTask(db);
                break;
            case 'p':
                changePriority(db);
                break;
            case 'q':
                sqlite3_close(db);
                exit(0);
            default:
                printf("Invalid choice.\n");
                break;
        }
    }

    return 0;
}

void initializeDatabase(sqlite3 **db) {
    int rc = sqlite3_open("todo_list.db", db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(*db));
        exit(1);
    } else {
        fprintf(stderr, "Opened database successfully\n");
    }

    // Create SQL table
    char *sql = "CREATE TABLE IF NOT EXISTS TASKS("
                "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                "TASK TEXT NOT NULL,"
                "PRIORITY INTEGER NOT NULL);";
    
    char *errMsg = 0;
    rc = sqlite3_exec(*db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    } else {
        fprintf(stdout, "Table created successfully\n");
    }
}

void addTask(sqlite3 *db) {
    char task[256];
    int priority;
    printf("Enter task description: ");
    scanf("%s", task);
    printf("Enter task priority (integer): ");
    scanf("%d", &priority);

    char sql[512];
    sprintf(sql, "INSERT INTO TASKS (TASK,PRIORITY) VALUES ('%s', %d);", task, priority);
    
    char *errMsg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    } else {
        fprintf(stdout, "Task added successfully\n");
    }
}

void viewTasks(sqlite3 *db) {
    const char *sql = "SELECT ID, TASK, PRIORITY FROM TASKS ORDER BY PRIORITY DESC;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    printf("\nID | TASK | PRIORITY\n");
    printf("--------------------\n");

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char *task = (const char *)sqlite3_column_text(stmt, 1);
        int priority = sqlite3_column_int(stmt, 2);
        printf("%d | %s | %d\n", id, task, priority);
    }

    sqlite3_finalize(stmt);
}

void editTask(sqlite3 *db) {
    int id;
    char newTask[256];
    int newPriority;
    printf("Enter the ID of the task you want to edit: ");
    scanf("%d", &id);
    printf("Enter new task description: ");
    scanf("%s", newTask);
    printf("Enter new task priority: ");
    scanf("%d", &newPriority);

    char sql[512];
    sprintf(sql, "UPDATE TASKS SET TASK = '%s', PRIORITY = %d WHERE ID = %d;", newTask, newPriority, id);

    char *errMsg = 0;
    if (sqlite3_exec(db, sql, 0, 0, &errMsg) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    } else {
        printf("Task updated successfully.\n");
    }
}

void removeTask(sqlite3 *db) {
    int id;
    printf("Enter the ID of the task you want to remove: ");
    scanf("%d", &id);

    char sql[256];
    sprintf(sql, "DELETE FROM TASKS WHERE ID = %d;", id);

    char *errMsg = 0;
    if (sqlite3_exec(db, sql, 0, 0, &errMsg) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    } else {
        printf("Task removed successfully.\n");
    }
}

void changePriority(sqlite3 *db) {
    int id, newPriority;
    printf("Enter the ID of the task you want to change the priority of: ");
    scanf("%d", &id);
    printf("Enter the new priority: ");
    scanf("%d", &newPriority);

    char sql[256];
    sprintf(sql, "UPDATE TASKS SET PRIORITY = %d WHERE ID = %d;", newPriority, id);

    char *errMsg = 0;
    if (sqlite3_exec(db, sql, 0, 0, &errMsg) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    } else {
        printf("Priority changed successfully.\n");
    }
}
