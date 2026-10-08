/* Tests of the text buffer. Returns 0 when every check passes. */
#undef NDEBUG
#include <assert.h>
#include <string.h>

#include "text_editor.h"

static void load(Buffer *b, const char *text) { assert(buffer_load(b, text, strlen(text)) == 0); }

static void insert(Buffer *b, const char *text) {
    assert(buffer_insert(b, text, strlen(text)) == 0);
}

static void expect_line(const Buffer *b, size_t row, const char *expected) {
    assert(row < b->count);
    assert(strcmp(buffer_line(b, row), expected) == 0);
}

static void test_load_splits_lines(void) {
    Buffer b;
    buffer_init(&b);
    assert(b.count == 1);
    expect_line(&b, 0, "");

    load(&b, "a\nb\n");
    assert(b.count == 2);
    expect_line(&b, 1, "b");

    load(&b, "a\n\nb");
    assert(b.count == 3);
    expect_line(&b, 1, "");
    assert(b.modified == 0);
    buffer_free(&b);
}

static void test_insert_and_newline(void) {
    Buffer b;
    buffer_init(&b);
    insert(&b, "abcd");
    assert(b.modified == 1);
    assert(b.col == 4);

    b.col = 2;
    assert(buffer_insert(&b, "\n", 1) == 0);
    assert(b.count == 2);
    expect_line(&b, 0, "ab");
    expect_line(&b, 1, "cd");
    assert(b.row == 1 && b.col == 0);

    insert(&b, "x\ny");
    assert(b.count == 3);
    expect_line(&b, 1, "x");
    expect_line(&b, 2, "ycd");
    assert(b.row == 2 && b.col == 1);
    buffer_free(&b);
}

static void test_backspace_and_delete_join_lines(void) {
    Buffer b;
    buffer_init(&b);
    load(&b, "ab\ncd");
    b.row = 1;
    b.col = 0;
    assert(buffer_backspace(&b) == 0);
    assert(b.count == 1);
    expect_line(&b, 0, "abcd");
    assert(b.row == 0 && b.col == 2);

    load(&b, "ab\ncd");
    b.row = 0;
    b.col = 2;
    assert(buffer_delete(&b) == 0);
    assert(b.count == 1);
    expect_line(&b, 0, "abcd");
    assert(b.col == 2);

    buffer_home(&b);
    assert(buffer_backspace(&b) == 0);
    expect_line(&b, 0, "abcd");
    buffer_free(&b);
}

static void test_utf8_characters_are_whole(void) {
    Buffer b;
    buffer_init(&b);
    insert(&b, "z\xc4\x85"); /* "zą" */
    assert(b.col == 3);
    buffer_backspace(&b);
    expect_line(&b, 0, "z");
    assert(b.col == 1);

    load(&b, "\xc4\x85\xc4\x99"); /* "ąę" */
    buffer_home(&b);
    buffer_right(&b);
    assert(b.col == 2);
    buffer_delete(&b);
    expect_line(&b, 0, "\xc4\x85");
    buffer_free(&b);
}

static void test_cursor_moves_across_lines(void) {
    Buffer b;
    buffer_init(&b);
    load(&b, "abc\nde");
    buffer_right(&b);
    buffer_right(&b);
    buffer_right(&b);
    assert(b.row == 0 && b.col == 3);
    buffer_right(&b);
    assert(b.row == 1 && b.col == 0);
    buffer_left(&b);
    assert(b.row == 0 && b.col == 3);
    buffer_end(&b);
    assert(b.col == 3);
    buffer_free(&b);
}

static void test_vertical_moves_clamp_column(void) {
    Buffer b;
    buffer_init(&b);
    load(&b, "abcdef\nab\nabcdef");
    b.col = 5;
    buffer_move_rows(&b, 1);
    assert(b.row == 1 && b.col == 2);
    buffer_move_rows(&b, 10);
    assert(b.row == 2);
    buffer_move_rows(&b, -10);
    assert(b.row == 0);
    buffer_goto_line(&b, 99);
    assert(b.row == 2 && b.col == 0);
    buffer_free(&b);
}

static void test_find_wraps_around(void) {
    Buffer b;
    buffer_init(&b);
    load(&b, "foo bar\nbaz foo\nend");

    assert(buffer_find(&b, "foo") == 1);
    assert(b.row == 1 && b.col == 4);
    assert(buffer_find(&b, "foo") == 1);
    assert(b.row == 0 && b.col == 0);
    assert(buffer_find(&b, "nothing") == 0);
    assert(buffer_find(&b, "") == 0);
    buffer_free(&b);
}

static void test_word_count(void) {
    Buffer b;
    buffer_init(&b);
    assert(buffer_word_count(&b) == 0);
    load(&b, "one two\n  three\n\nfour ");
    assert(buffer_word_count(&b) == 4);
    buffer_free(&b);
}

static void test_utf8_helpers(void) {
    assert(utf8_char_count("\xc4\x85\xc4\x99", 4) == 2);
    assert(utf8_offset("\xc4\x85"
                       "b",
                       1) == 2);
    assert(utf8_offset("ab", 5) == 2);
    assert(utf8_next("\xc4\x85", 0) == 2);
    assert(utf8_prev("\xc4\x85", 2) == 0);
}

int main(void) {
    test_load_splits_lines();
    test_insert_and_newline();
    test_backspace_and_delete_join_lines();
    test_utf8_characters_are_whole();
    test_cursor_moves_across_lines();
    test_vertical_moves_clamp_column();
    test_find_wraps_around();
    test_word_count();
    test_utf8_helpers();
    return 0;
}
