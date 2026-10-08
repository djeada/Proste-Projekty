/* Logic of the version control tool: snapshots, commits, checkout and diffs. */
#define _DEFAULT_SOURCE

#include "version_control.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define PATH_SIZE 4096

typedef struct {
    const unsigned char *text;
    size_t length;
} Line;

static int file_exists(const char *path) {
    struct stat st;
    return stat(path, &st) == 0;
}

static int read_file(const char *path, File *file) {
    FILE *f = fopen(path, "rb");
    if (!f) {
        return -1;
    }
    if (fseek(f, 0, SEEK_END) != 0) {
        fclose(f);
        return -1;
    }
    long size = ftell(f);
    if (size < 0 || fseek(f, 0, SEEK_SET) != 0) {
        fclose(f);
        return -1;
    }
    file->data = malloc((size_t)size + 1);
    if (!file->data) {
        fclose(f);
        return -1;
    }
    file->size = fread(file->data, 1, (size_t)size, f);
    fclose(f);
    return 0;
}

static int write_file(const char *path, const unsigned char *data, size_t size) {
    FILE *f = fopen(path, "wb");
    if (!f) {
        return -1;
    }
    size_t written = fwrite(data, 1, size, f);
    return (fclose(f) == 0 && written == size) ? 0 : -1;
}

static int compare_files(const void *a, const void *b) {
    const File *fa = a;
    const File *fb = b;
    return strcmp(fa->name, fb->name);
}

static int same_contents(const File *a, const File *b) {
    return a->size == b->size && memcmp(a->data, b->data, a->size) == 0;
}

/* Path of "item" inside commit number N, e.g. "dir/.vcs/commits/2/meta". */
static void commit_path(char *buf, size_t size, const char *dir, int number, const char *item) {
    snprintf(buf, size, "%s/%s/commits/%d/%s", dir, VCS_DIR, number, item);
}

int repo_exists(const char *dir) {
    char path[PATH_SIZE];
    snprintf(path, sizeof(path), "%s/%s", dir, VCS_DIR);
    return file_exists(path);
}

int repo_init(const char *dir) {
    char path[PATH_SIZE];
    if (repo_exists(dir)) {
        return -1;
    }
    snprintf(path, sizeof(path), "%s/%s", dir, VCS_DIR);
    if (mkdir(path, 0777) != 0) {
        return -1;
    }
    snprintf(path, sizeof(path), "%s/%s/commits", dir, VCS_DIR);
    return mkdir(path, 0777) == 0 ? 0 : -1;
}

int repo_commit_count(const char *dir) {
    char path[PATH_SIZE];
    int count = 0;

    if (!repo_exists(dir)) {
        return -1;
    }
    for (;;) {
        commit_path(path, sizeof(path), dir, count + 1, "meta");
        if (!file_exists(path)) {
            return count;
        }
        count++;
    }
}

int repo_commit(const char *dir, const char *message) {
    char path[PATH_SIZE];
    char item[PATH_SIZE];
    Snapshot snap;
    int count = repo_commit_count(dir);

    if (count < 0 || !message || strchr(message, '\n') || strlen(message) >= MESSAGE_SIZE - 1) {
        return -1;
    }
    if (snapshot_load_dir(dir, &snap) != 0) {
        return -1;
    }

    int number = count + 1;
    snprintf(path, sizeof(path), "%s/%s/commits/%d", dir, VCS_DIR, number);
    commit_path(item, sizeof(item), dir, number, "files");
    int ok = mkdir(path, 0777) == 0 && mkdir(item, 0777) == 0;

    for (size_t i = 0; ok && i < snap.count; i++) {
        snprintf(item, sizeof(item), "%s/%s/commits/%d/files/%s", dir, VCS_DIR, number,
                 snap.files[i].name);
        ok = write_file(item, snap.files[i].data, snap.files[i].size) == 0;
    }

    if (ok) {
        commit_path(path, sizeof(path), dir, number, "meta");
        FILE *meta = fopen(path, "w");
        ok = meta != NULL;
        if (meta) {
            fprintf(meta, "%ld\n%s\n", (long)time(NULL), message);
            ok = fclose(meta) == 0;
        }
    }

    snapshot_free(&snap);
    return ok ? number : -1;
}

int repo_info(const char *dir, int number, CommitInfo *info) {
    char path[PATH_SIZE];
    long stamp;
    commit_path(path, sizeof(path), dir, number, "meta");

    FILE *meta = fopen(path, "r");
    if (!meta) {
        return -1;
    }
    int ok = fscanf(meta, "%ld\n", &stamp) == 1 && fgets(info->message, MESSAGE_SIZE, meta) != NULL;
    fclose(meta);
    if (!ok) {
        return -1;
    }
    info->message[strcspn(info->message, "\n")] = '\0';
    info->number = number;
    info->time = (time_t)stamp;
    return 0;
}

int repo_load_commit(const char *dir, int number, Snapshot *snap) {
    char path[PATH_SIZE];
    commit_path(path, sizeof(path), dir, number, "files");
    return snapshot_load_dir(path, snap);
}

int repo_checkout(const char *dir, int number) {
    Snapshot snap;
    if (repo_load_commit(dir, number, &snap) != 0) {
        return -1;
    }

    int ok = 1;
    for (size_t i = 0; ok && i < snap.count; i++) {
        char path[PATH_SIZE];
        snprintf(path, sizeof(path), "%s/%s", dir, snap.files[i].name);
        ok = write_file(path, snap.files[i].data, snap.files[i].size) == 0;
    }

    snapshot_free(&snap);
    return ok ? 0 : -1;
}

int snapshot_load_dir(const char *dir, Snapshot *snap) {
    struct dirent *entry;
    size_t capacity = 0;

    snap->files = NULL;
    snap->count = 0;

    DIR *d = opendir(dir);
    if (!d) {
        return -1;
    }
    while ((entry = readdir(d)) != NULL) {
        if (entry->d_type != DT_REG) {
            continue;
        }
        if (snap->count == capacity) {
            size_t new_capacity = capacity ? capacity * 2 : 16;
            File *grown = realloc(snap->files, new_capacity * sizeof(File));
            if (!grown) {
                break;
            }
            snap->files = grown;
            capacity = new_capacity;
        }

        File *file = &snap->files[snap->count];
        char path[PATH_SIZE];
        snprintf(path, sizeof(path), "%s/%s", dir, entry->d_name);
        file->name = malloc(strlen(entry->d_name) + 1);
        if (!file->name) {
            break;
        }
        strcpy(file->name, entry->d_name);
        if (read_file(path, file) != 0) {
            free(file->name);
            break;
        }
        snap->count++;
    }
    closedir(d);

    if (entry != NULL) {
        snapshot_free(snap);
        return -1;
    }
    qsort(snap->files, snap->count, sizeof(File), compare_files);
    return 0;
}

void snapshot_free(Snapshot *snap) {
    for (size_t i = 0; i < snap->count; i++) {
        free(snap->files[i].name);
        free(snap->files[i].data);
    }
    free(snap->files);
    snap->files = NULL;
    snap->count = 0;
}

const File *snapshot_find(const Snapshot *snap, const char *name) {
    for (size_t i = 0; i < snap->count; i++) {
        if (strcmp(snap->files[i].name, name) == 0) {
            return &snap->files[i];
        }
    }
    return NULL;
}

static size_t add_change(Change *out, size_t n, ChangeKind kind, const char *name) {
    out[n].kind = kind;
    out[n].name = name;
    return n + 1;
}

size_t snapshot_changes(const Snapshot *base, const Snapshot *current, Change *out) {
    size_t n = 0;

    for (size_t i = 0; i < current->count; i++) {
        const File *now = &current->files[i];
        const File *before = snapshot_find(base, now->name);
        if (!before) {
            n = add_change(out, n, CHANGE_ADDED, now->name);
        } else if (!same_contents(before, now)) {
            n = add_change(out, n, CHANGE_MODIFIED, now->name);
        }
    }
    for (size_t i = 0; i < base->count; i++) {
        if (!snapshot_find(current, base->files[i].name)) {
            n = add_change(out, n, CHANGE_DELETED, base->files[i].name);
        }
    }
    return n;
}

/* Splits file contents into lines, without the '\n' characters. */
static Line *split_lines(const File *file, size_t *count) {
    size_t size = file ? file->size : 0;
    const unsigned char *data = file ? file->data : NULL;
    size_t total = 0;

    for (size_t i = 0; i < size; i++) {
        if (data[i] == '\n') {
            total++;
        }
    }
    if (size > 0 && data[size - 1] != '\n') {
        total++;
    }

    Line *lines = malloc((total + 1) * sizeof(Line));
    *count = 0;
    if (!lines) {
        return NULL;
    }
    size_t start = 0;
    for (size_t i = 0; i < size; i++) {
        if (data[i] == '\n') {
            lines[*count].text = data + start;
            lines[*count].length = i - start;
            (*count)++;
            start = i + 1;
        }
    }
    if (start < size) {
        lines[*count].text = data + start;
        lines[*count].length = size - start;
        (*count)++;
    }
    return lines;
}

static int same_line(Line a, Line b) {
    return a.length == b.length && memcmp(a.text, b.text, a.length) == 0;
}

int diff_file(const File *old_file, const File *new_file, DiffLine **lines, size_t *count) {
    size_t n = 0;
    size_t m = 0;
    Line *a = split_lines(old_file, &n);
    Line *b = split_lines(new_file, &m);
    size_t *lcs = malloc((n + 1) * (m + 1) * sizeof(size_t));
    DiffLine *result = malloc((n + m + 1) * sizeof(DiffLine));

    *lines = NULL;
    *count = 0;
    if (!a || !b || !lcs || !result) {
        free(a);
        free(b);
        free(lcs);
        free(result);
        return -1;
    }

    /* lcs[i][j] = length of the longest common subsequence of a[i..] and b[j..] */
    for (size_t i = n + 1; i-- > 0;) {
        for (size_t j = m + 1; j-- > 0;) {
            size_t cell = i * (m + 1) + j;
            if (i == n || j == m) {
                lcs[cell] = 0;
            } else if (same_line(a[i], b[j])) {
                lcs[cell] = lcs[cell + m + 2] + 1;
            } else {
                size_t down = lcs[cell + m + 1];
                size_t right = lcs[cell + 1];
                lcs[cell] = down > right ? down : right;
            }
        }
    }

    size_t i = 0;
    size_t j = 0;
    while (i < n || j < m) {
        DiffLine *line = &result[(*count)++];
        if (i < n && j < m && same_line(a[i], b[j])) {
            line->tag = ' ';
            line->text = a[i].text;
            line->length = a[i].length;
            i++;
            j++;
        } else if (i < n && (j == m || lcs[i * (m + 1) + j + 1] <= lcs[(i + 1) * (m + 1) + j])) {
            line->tag = '-';
            line->text = a[i].text;
            line->length = a[i].length;
            i++;
        } else {
            line->tag = '+';
            line->text = b[j].text;
            line->length = b[j].length;
            j++;
        }
    }

    free(a);
    free(b);
    free(lcs);
    *lines = result;
    return 0;
}

void format_time(time_t when, char *buffer, size_t size) {
    struct tm *local = localtime(&when);
    if (!local || strftime(buffer, size, "%Y-%m-%d %H:%M:%S", local) == 0) {
        snprintf(buffer, size, "unknown time");
    }
}
