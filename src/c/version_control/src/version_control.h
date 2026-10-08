/* Logic of a small git-like tool: snapshots, commits and line diffs. */
#ifndef VERSION_CONTROL_H
#define VERSION_CONTROL_H

#include <stddef.h>
#include <time.h>

#define VCS_DIR ".vcs"
#define MESSAGE_SIZE 256

/* A file name and its contents. */
typedef struct {
    char *name;
    unsigned char *data;
    size_t size;
} File;

/* A set of files, sorted by name. */
typedef struct {
    File *files;
    size_t count;
} Snapshot;

typedef enum { CHANGE_ADDED, CHANGE_MODIFIED, CHANGE_DELETED } ChangeKind;

typedef struct {
    ChangeKind kind;
    const char *name;
} Change;

/* tag is ' ' (same line), '-' (only in the old file) or '+' (only in the new file). */
typedef struct {
    char tag;
    const unsigned char *text;
    size_t length;
} DiffLine;

typedef struct {
    int number;
    time_t time;
    char message[MESSAGE_SIZE];
} CommitInfo;

/* Repository. All functions take the directory that holds the .vcs folder. */
int repo_exists(const char *dir);
int repo_init(const char *dir);
int repo_commit_count(const char *dir);
int repo_commit(const char *dir, const char *message);
int repo_info(const char *dir, int number, CommitInfo *info);
int repo_load_commit(const char *dir, int number, Snapshot *snap);
int repo_checkout(const char *dir, int number);

/* Snapshots */
int snapshot_load_dir(const char *dir, Snapshot *snap);
void snapshot_free(Snapshot *snap);
const File *snapshot_find(const Snapshot *snap, const char *name);
size_t snapshot_changes(const Snapshot *base, const Snapshot *current, Change *out);

/* Line diff (longest common subsequence). Free *lines with free(). */
int diff_file(const File *old_file, const File *new_file, DiffLine **lines, size_t *count);

void format_time(time_t when, char *buffer, size_t size);

#endif
