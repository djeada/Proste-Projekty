#ifndef VERSION_CONTROL_H
#define VERSION_CONTROL_H

#include <time.h>

#define MAX_PATH_LENGTH 256
#define MAX_MESSAGE_LENGTH 256
#define MAX_COMMITS 100
#define MAX_FILES 50
#define MAX_FILE_SIZE 65536

typedef struct {
    char path[MAX_PATH_LENGTH];
    char *content;
    size_t size;
} FileSnapshot;

typedef struct {
    int id;
    char message[MAX_MESSAGE_LENGTH];
    time_t timestamp;
    FileSnapshot files[MAX_FILES];
    int file_count;
} Commit;

typedef struct {
    char repo_path[MAX_PATH_LENGTH];
    Commit commits[MAX_COMMITS];
    int commit_count;
    int initialized;
} Repository;

// Repository management
int repo_init(Repository *repo, const char *path);
void repo_free(Repository *repo);
int repo_is_initialized(const Repository *repo);

// Commit operations
int repo_commit(Repository *repo, const char *message);
int repo_get_commit(const Repository *repo, int commit_id, Commit *commit);
int repo_get_commit_count(const Repository *repo);

// History operations
void repo_log(const Repository *repo);
int repo_checkout(Repository *repo, int commit_id);
int repo_diff(const Repository *repo, int commit_id1, int commit_id2);

// File tracking
int repo_add_file(Repository *repo, const char *filepath);
int repo_status(const Repository *repo);

// Utility functions
char *read_file_content(const char *filepath, size_t *size);
int write_file_content(const char *filepath, const char *content, size_t size);
char *format_time(time_t timestamp);

#endif // VERSION_CONTROL_H
