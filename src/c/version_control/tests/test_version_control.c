#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../src/version_control.h"

static char test_dir[] = "/tmp/vcs_test_XXXXXX";

void setup_test_dir(void) {
    char *result = mkdtemp(test_dir);
    assert(result != NULL);
}

void cleanup_test_dir(void) {
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "rm -rf %s", test_dir);
    int ret = system(cmd);
    (void)ret;
}

void create_test_file(const char *name, const char *content) {
    char path[512];
    snprintf(path, sizeof(path), "%s/%s", test_dir, name);
    FILE *f = fopen(path, "w");
    assert(f != NULL);
    fprintf(f, "%s", content);
    fclose(f);
}

void test_repo_init(void) {
    Repository repo;
    int result = repo_init(&repo, test_dir);
    assert(result == 1);
    assert(repo_is_initialized(&repo) == 1);
    repo_free(&repo);
}

void test_repo_commit(void) {
    Repository repo;
    repo_init(&repo, test_dir);

    create_test_file("test.txt", "Hello World");

    int commit_id = repo_commit(&repo, "Initial commit");
    assert(commit_id == 1);
    assert(repo_get_commit_count(&repo) == 1);

    Commit c;
    int found = repo_get_commit(&repo, 1, &c);
    assert(found == 1);
    assert(strcmp(c.message, "Initial commit") == 0);
    assert(c.file_count >= 1);

    repo_free(&repo);
}

void test_repo_multiple_commits(void) {
    Repository repo;
    repo_init(&repo, test_dir);

    create_test_file("file1.txt", "Content 1");
    int id1 = repo_commit(&repo, "First commit");
    assert(id1 == 1);

    create_test_file("file2.txt", "Content 2");
    int id2 = repo_commit(&repo, "Second commit");
    assert(id2 == 2);

    assert(repo_get_commit_count(&repo) == 2);

    repo_free(&repo);
}

void test_repo_checkout(void) {
    Repository repo;
    repo_init(&repo, test_dir);

    create_test_file("data.txt", "Version 1");
    repo_commit(&repo, "Version 1");

    // Modify file
    create_test_file("data.txt", "Version 2");
    repo_commit(&repo, "Version 2");

    // Checkout version 1
    int result = repo_checkout(&repo, 1);
    assert(result == 1);

    // Read file content
    char path[512];
    snprintf(path, sizeof(path), "%s/data.txt", test_dir);
    size_t size;
    char *content = read_file_content(path, &size);
    assert(content != NULL);
    assert(strcmp(content, "Version 1") == 0);
    free(content);

    repo_free(&repo);
}

void test_format_time(void) {
    time_t now = time(NULL);
    char *formatted = format_time(now);
    assert(formatted != NULL);
    assert(strlen(formatted) > 0);
}

int main(void) {
    setup_test_dir();

    test_repo_init();
    test_repo_commit();
    test_repo_multiple_commits();
    test_repo_checkout();
    test_format_time();

    cleanup_test_dir();

    printf("All tests passed!\n");
    return 0;
}
