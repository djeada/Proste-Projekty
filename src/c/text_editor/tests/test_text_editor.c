#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "../src/text_editor.h"

void test_buffer_init() {
    TextBuffer buf;
    buffer_init(&buf);
    assert(buf.num_lines == 0);
    assert(buf.modified == 0);
    assert(buf.filename[0] == '\0');
    buffer_free(&buf);
}

void test_buffer_append_line() {
    TextBuffer buf;
    buffer_init(&buf);

    assert(buffer_append_line(&buf, "Hello") == 1);
    assert(buf.num_lines == 1);
    assert(strcmp(buffer_get_line(&buf, 0), "Hello") == 0);

    assert(buffer_append_line(&buf, "World") == 1);
    assert(buf.num_lines == 2);
    assert(strcmp(buffer_get_line(&buf, 1), "World") == 0);

    buffer_free(&buf);
}

void test_buffer_insert_line() {
    TextBuffer buf;
    buffer_init(&buf);

    buffer_append_line(&buf, "Line 1");
    buffer_append_line(&buf, "Line 3");

    assert(buffer_insert_line(&buf, 1, "Line 2") == 1);
    assert(buf.num_lines == 3);
    assert(strcmp(buffer_get_line(&buf, 0), "Line 1") == 0);
    assert(strcmp(buffer_get_line(&buf, 1), "Line 2") == 0);
    assert(strcmp(buffer_get_line(&buf, 2), "Line 3") == 0);

    buffer_free(&buf);
}

void test_buffer_delete_line() {
    TextBuffer buf;
    buffer_init(&buf);

    buffer_append_line(&buf, "Line 1");
    buffer_append_line(&buf, "Line 2");
    buffer_append_line(&buf, "Line 3");

    assert(buffer_delete_line(&buf, 1) == 1);
    assert(buf.num_lines == 2);
    assert(strcmp(buffer_get_line(&buf, 0), "Line 1") == 0);
    assert(strcmp(buffer_get_line(&buf, 1), "Line 3") == 0);

    // Invalid delete
    assert(buffer_delete_line(&buf, 10) == 0);
    assert(buffer_delete_line(&buf, -1) == 0);

    buffer_free(&buf);
}

void test_buffer_get_line() {
    TextBuffer buf;
    buffer_init(&buf);

    buffer_append_line(&buf, "Test");

    assert(buffer_get_line(&buf, 0) != NULL);
    assert(buffer_get_line(&buf, 1) == NULL);
    assert(buffer_get_line(&buf, -1) == NULL);

    buffer_free(&buf);
}

void test_buffer_line_count() {
    TextBuffer buf;
    buffer_init(&buf);

    assert(buffer_line_count(&buf) == 0);
    buffer_append_line(&buf, "One");
    assert(buffer_line_count(&buf) == 1);
    buffer_append_line(&buf, "Two");
    assert(buffer_line_count(&buf) == 2);

    buffer_free(&buf);
}

int main() {
    test_buffer_init();
    test_buffer_append_line();
    test_buffer_insert_line();
    test_buffer_delete_line();
    test_buffer_get_line();
    test_buffer_line_count();
    printf("All tests passed!\n");
    return 0;
}
