#include "version_control.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

char *read_file_content(const char *filepath, size_t *size) {
    FILE *file = fopen(filepath, "rb");
    if (!file) {
        *size = 0;
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (file_size < 0 || file_size > MAX_FILE_SIZE) {
        fclose(file);
        *size = 0;
        return NULL;
    }

    char *content = malloc((size_t)file_size + 1);
    if (!content) {
        fclose(file);
        *size = 0;
        return NULL;
    }

    size_t bytes_read = fread(content, 1, (size_t)file_size, file);
    content[bytes_read] = '\0';
    fclose(file);

    *size = bytes_read;
    return content;
}

int write_file_content(const char *filepath, const char *content, size_t size) {
    FILE *file = fopen(filepath, "wb");
    if (!file) {
        return 0;
    }

    size_t bytes_written = fwrite(content, 1, size, file);
    fclose(file);

    return bytes_written == size;
}

char *format_time(time_t timestamp) {
    static char buffer[64];
    struct tm *tm_info = localtime(&timestamp);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_info);
    return buffer;
}

static void free_commit(Commit *commit) {
    for (int i = 0; i < commit->file_count; i++) {
        free(commit->files[i].content);
        commit->files[i].content = NULL;
    }
    commit->file_count = 0;
}

int repo_init(Repository *repo, const char *path) {
    if (!repo || !path) {
        return 0;
    }

    memset(repo, 0, sizeof(Repository));
    strncpy(repo->repo_path, path, MAX_PATH_LENGTH - 1);
    repo->repo_path[MAX_PATH_LENGTH - 1] = '\0';
    repo->commit_count = 0;
    repo->initialized = 1;

    return 1;
}

void repo_free(Repository *repo) {
    if (!repo) return;

    for (int i = 0; i < repo->commit_count; i++) {
        free_commit(&repo->commits[i]);
    }
    repo->commit_count = 0;
    repo->initialized = 0;
}

int repo_is_initialized(const Repository *repo) {
    return repo && repo->initialized;
}

int repo_add_file(Repository *repo, const char *filepath) {
    if (!repo || !filepath || !repo->initialized) {
        return 0;
    }

    // Files are tracked at commit time, this just validates the file exists
    size_t size;
    char *content = read_file_content(filepath, &size);
    if (!content && size == 0) {
        // Check if file exists but is empty
        FILE *f = fopen(filepath, "r");
        if (!f) {
            return 0;
        }
        fclose(f);
    }
    free(content);
    return 1;
}

int repo_commit(Repository *repo, const char *message) {
    if (!repo || !message || !repo->initialized) {
        return -1;
    }

    if (repo->commit_count >= MAX_COMMITS) {
        return -1;
    }

    Commit *commit = &repo->commits[repo->commit_count];
    memset(commit, 0, sizeof(Commit));

    commit->id = repo->commit_count + 1;
    strncpy(commit->message, message, MAX_MESSAGE_LENGTH - 1);
    commit->message[MAX_MESSAGE_LENGTH - 1] = '\0';
    commit->timestamp = time(NULL);
    commit->file_count = 0;

    // Scan directory for files to track
    DIR *dir = opendir(repo->repo_path);
    if (dir) {
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL && commit->file_count < MAX_FILES) {
            if (entry->d_type == DT_REG) {  // Regular file
                char fullpath[MAX_PATH_LENGTH * 2];
                snprintf(fullpath, sizeof(fullpath), "%s/%s", repo->repo_path, entry->d_name);

                size_t size;
                char *content = read_file_content(fullpath, &size);
                if (content || size == 0) {
                    FileSnapshot *fs = &commit->files[commit->file_count];
                    strncpy(fs->path, entry->d_name, MAX_PATH_LENGTH - 1);
                    fs->path[MAX_PATH_LENGTH - 1] = '\0';
                    fs->content = content;
                    fs->size = size;
                    commit->file_count++;
                }
            }
        }
        closedir(dir);
    }

    repo->commit_count++;
    return commit->id;
}

int repo_get_commit(const Repository *repo, int commit_id, Commit *commit) {
    if (!repo || !commit || commit_id < 1 || commit_id > repo->commit_count) {
        return 0;
    }

    *commit = repo->commits[commit_id - 1];
    return 1;
}

int repo_get_commit_count(const Repository *repo) {
    if (!repo) return 0;
    return repo->commit_count;
}

void repo_log(const Repository *repo) {
    if (!repo || !repo->initialized) {
        printf("Repository not initialized.\n");
        return;
    }

    if (repo->commit_count == 0) {
        printf("No commits yet.\n");
        return;
    }

    printf("\n=== Commit History ===\n\n");
    for (int i = repo->commit_count - 1; i >= 0; i--) {
        const Commit *c = &repo->commits[i];
        printf("Commit #%d\n", c->id);
        printf("Date:    %s\n", format_time(c->timestamp));
        printf("Message: %s\n", c->message);
        printf("Files:   %d\n", c->file_count);
        printf("---\n");
    }
}

int repo_checkout(Repository *repo, int commit_id) {
    if (!repo || !repo->initialized || commit_id < 1 || commit_id > repo->commit_count) {
        return 0;
    }

    const Commit *commit = &repo->commits[commit_id - 1];

    for (int i = 0; i < commit->file_count; i++) {
        const FileSnapshot *fs = &commit->files[i];
        char fullpath[MAX_PATH_LENGTH * 2];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", repo->repo_path, fs->path);

        if (fs->content) {
            if (!write_file_content(fullpath, fs->content, fs->size)) {
                return 0;
            }
        }
    }

    printf("Checked out commit #%d: %s\n", commit->id, commit->message);
    return 1;
}

int repo_diff(const Repository *repo, int commit_id1, int commit_id2) {
    if (!repo || !repo->initialized) {
        return 0;
    }

    if (commit_id1 < 1 || commit_id1 > repo->commit_count ||
        commit_id2 < 1 || commit_id2 > repo->commit_count) {
        return 0;
    }

    const Commit *c1 = &repo->commits[commit_id1 - 1];
    const Commit *c2 = &repo->commits[commit_id2 - 1];

    printf("\n=== Diff: Commit #%d vs Commit #%d ===\n\n", commit_id1, commit_id2);

    // Compare files
    for (int i = 0; i < c2->file_count; i++) {
        const FileSnapshot *f2 = &c2->files[i];
        const FileSnapshot *f1 = NULL;

        // Find matching file in c1
        for (int j = 0; j < c1->file_count; j++) {
            if (strcmp(c1->files[j].path, f2->path) == 0) {
                f1 = &c1->files[j];
                break;
            }
        }

        if (!f1) {
            printf("[+] Added: %s\n", f2->path);
        } else if (f1->size != f2->size ||
                   (f1->content != f2->content &&
                    (!f1->content || !f2->content ||
                     strcmp(f1->content, f2->content) != 0))) {
            printf("[M] Modified: %s\n", f2->path);
        }
    }

    // Check for deleted files
    for (int i = 0; i < c1->file_count; i++) {
        const FileSnapshot *f1 = &c1->files[i];
        int found = 0;

        for (int j = 0; j < c2->file_count; j++) {
            if (strcmp(c2->files[j].path, f1->path) == 0) {
                found = 1;
                break;
            }
        }

        if (!found) {
            printf("[-] Deleted: %s\n", f1->path);
        }
    }

    return 1;
}

int repo_status(const Repository *repo) {
    if (!repo || !repo->initialized) {
        printf("Repository not initialized.\n");
        return 0;
    }

    printf("\n=== Repository Status ===\n");
    printf("Path: %s\n", repo->repo_path);
    printf("Commits: %d\n", repo->commit_count);

    if (repo->commit_count > 0) {
        const Commit *latest = &repo->commits[repo->commit_count - 1];
        printf("Latest commit: #%d - %s\n", latest->id, latest->message);
    }

    // Count current files
    DIR *dir = opendir(repo->repo_path);
    int file_count = 0;
    if (dir) {
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == DT_REG) {
                file_count++;
            }
        }
        closedir(dir);
    }
    printf("Current files: %d\n", file_count);

    return 1;
}
