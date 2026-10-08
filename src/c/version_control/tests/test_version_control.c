/* Tests of the version control logic. Each test works in its own temporary directory. */
#define _DEFAULT_SOURCE

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "../src/version_control.h"

static void write_text(const char *dir, const char *name, const char *text) {
    char path[1024];
    snprintf(path, sizeof(path), "%s/%s", dir, name);
    FILE *f = fopen(path, "wb");
    assert(f != NULL);
    fputs(text, f);
    fclose(f);
}

static void read_text(const char *dir, const char *name, char *buffer, size_t size) {
    char path[1024];
    snprintf(path, sizeof(path), "%s/%s", dir, name);
    FILE *f = fopen(path, "rb");
    assert(f != NULL);
    size_t n = fread(buffer, 1, size - 1, f);
    buffer[n] = '\0';
    fclose(f);
}

static char *make_temp_dir(char *template_path) {
    strcpy(template_path, "/tmp/vcs_test_XXXXXX");
    assert(mkdtemp(template_path) != NULL);
    return template_path;
}

static void remove_dir(const char *dir) {
    char command[1200];
    snprintf(command, sizeof(command), "rm -rf '%s'", dir);
    int ignored = system(command);
    (void)ignored;
}

static void test_init_twice_fails(void) {
    char dir[64];
    make_temp_dir(dir);
    assert(repo_exists(dir) == 0);
    assert(repo_init(dir) == 0);
    assert(repo_exists(dir) == 1);
    assert(repo_init(dir) == -1);
    assert(repo_commit_count(dir) == 0);
    remove_dir(dir);
}

static void test_commits_are_numbered(void) {
    char dir[64];
    CommitInfo info;
    make_temp_dir(dir);
    repo_init(dir);

    write_text(dir, "a.txt", "one\n");
    assert(repo_commit(dir, "first") == 1);
    write_text(dir, "a.txt", "two\n");
    assert(repo_commit(dir, "second") == 2);
    assert(repo_commit_count(dir) == 2);

    assert(repo_info(dir, 2, &info) == 0);
    assert(info.number == 2);
    assert(strcmp(info.message, "second") == 0);
    assert(info.time > 0);
    remove_dir(dir);
}

static void test_commit_ignores_subdirectories(void) {
    char dir[64];
    char path[128];
    Snapshot snap;
    make_temp_dir(dir);
    repo_init(dir);

    write_text(dir, "top.txt", "x");
    snprintf(path, sizeof(path), "%s/sub", dir);
    assert(mkdir(path, 0777) == 0);
    write_text(dir, "sub/inner.txt", "y");

    assert(repo_commit(dir, "only top") == 1);
    assert(repo_load_commit(dir, 1, &snap) == 0);
    assert(snap.count == 1);
    assert(strcmp(snap.files[0].name, "top.txt") == 0);
    snapshot_free(&snap);
    remove_dir(dir);
}

static void test_commit_rejects_bad_message(void) {
    char dir[64];
    make_temp_dir(dir);
    repo_init(dir);
    write_text(dir, "a.txt", "x");
    assert(repo_commit(dir, "two\nlines") == -1);
    assert(repo_commit_count(dir) == 0);
    remove_dir(dir);
}

static void test_changes_between_snapshots(void) {
    char dir[64];
    Snapshot base;
    Snapshot current;
    Change changes[8];
    make_temp_dir(dir);
    repo_init(dir);

    write_text(dir, "keep.txt", "same");
    write_text(dir, "edit.txt", "old");
    write_text(dir, "gone.txt", "bye");
    assert(repo_commit(dir, "base") == 1);
    assert(repo_load_commit(dir, 1, &base) == 0);

    write_text(dir, "edit.txt", "new");
    {
        char path[1024];
        snprintf(path, sizeof(path), "%s/gone.txt", dir);
        assert(remove(path) == 0);
    }
    write_text(dir, "added.txt", "hi");
    assert(snapshot_load_dir(dir, &current) == 0);

    size_t n = snapshot_changes(&base, &current, changes);
    assert(n == 3);
    int added = 0;
    int modified = 0;
    int deleted = 0;
    for (size_t i = 0; i < n; i++) {
        if (changes[i].kind == CHANGE_ADDED && strcmp(changes[i].name, "added.txt") == 0) added++;
        if (changes[i].kind == CHANGE_MODIFIED && strcmp(changes[i].name, "edit.txt") == 0) modified++;
        if (changes[i].kind == CHANGE_DELETED && strcmp(changes[i].name, "gone.txt") == 0) deleted++;
    }
    assert(added == 1 && modified == 1 && deleted == 1);

    snapshot_free(&base);
    snapshot_free(&current);
    remove_dir(dir);
}

static void test_checkout_restores_files(void) {
    char dir[64];
    char text[64];
    make_temp_dir(dir);
    repo_init(dir);

    write_text(dir, "data.txt", "version 1");
    assert(repo_commit(dir, "v1") == 1);
    write_text(dir, "data.txt", "version 2");
    assert(repo_commit(dir, "v2") == 2);

    assert(repo_checkout(dir, 1) == 0);
    read_text(dir, "data.txt", text, sizeof(text));
    assert(strcmp(text, "version 1") == 0);
    assert(repo_checkout(dir, 9) == -1);
    remove_dir(dir);
}

static void test_diff_marks_changed_lines(void) {
    File old_file = {"f", (unsigned char *)"a\nb\nc\n", 6};
    File new_file = {"f", (unsigned char *)"a\nx\nc\n", 6};
    DiffLine *lines;
    size_t count;

    assert(diff_file(&old_file, &new_file, &lines, &count) == 0);
    assert(count == 4);
    assert(lines[0].tag == ' ' && lines[0].length == 1 && lines[0].text[0] == 'a');
    assert(lines[1].tag == '-' && lines[1].text[0] == 'b');
    assert(lines[2].tag == '+' && lines[2].text[0] == 'x');
    assert(lines[3].tag == ' ' && lines[3].text[0] == 'c');
    free(lines);
}

static void test_diff_of_added_and_missing_lines(void) {
    File text = {"f", (unsigned char *)"one\ntwo", 7};
    DiffLine *lines;
    size_t count;

    assert(diff_file(NULL, &text, &lines, &count) == 0);
    assert(count == 2);
    assert(lines[0].tag == '+' && lines[0].length == 3);
    assert(lines[1].tag == '+' && lines[1].length == 3);
    free(lines);

    assert(diff_file(&text, NULL, &lines, &count) == 0);
    assert(count == 2 && lines[0].tag == '-' && lines[1].tag == '-');
    free(lines);
}

static void test_format_time(void) {
    char buffer[32];
    format_time(0, buffer, sizeof(buffer));
    assert(strlen(buffer) == 19);
    assert(buffer[4] == '-' && buffer[10] == ' ' && buffer[13] == ':');
}

int main(void) {
    test_init_twice_fails();
    test_commits_are_numbered();
    test_commit_ignores_subdirectories();
    test_commit_rejects_bad_message();
    test_changes_between_snapshots();
    test_checkout_restores_files();
    test_diff_marks_changed_lines();
    test_diff_of_added_and_missing_lines();
    test_format_time();

    printf("All tests passed!\n");
    return 0;
}
