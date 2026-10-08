/* The text buffer: lines of text and the cursor. No input or output here. */
#include "text_editor.h"

#include <stdlib.h>
#include <string.h>

static int is_continuation(char c) { return ((unsigned char)c & 0xC0) == 0x80; }

size_t utf8_next(const char *s, size_t pos) {
    if (s[pos] == '\0') {
        return pos;
    }
    pos++;
    while (is_continuation(s[pos])) {
        pos++;
    }
    return pos;
}

size_t utf8_prev(const char *s, size_t pos) {
    if (pos == 0) {
        return 0;
    }
    pos--;
    while (pos > 0 && is_continuation(s[pos])) {
        pos--;
    }
    return pos;
}

size_t utf8_char_count(const char *s, size_t bytes) {
    size_t count = 0;
    for (size_t i = 0; i < bytes; i++) {
        if (!is_continuation(s[i])) {
            count++;
        }
    }
    return count;
}

size_t utf8_offset(const char *s, size_t chars) {
    size_t pos = 0;
    for (size_t i = 0; i < chars && s[pos] != '\0'; i++) {
        pos = utf8_next(s, pos);
    }
    return pos;
}

static char *copy_bytes(const char *s, size_t n) {
    char *copy = malloc(n + 1);
    if (copy) {
        memcpy(copy, s, n);
        copy[n] = '\0';
    }
    return copy;
}

static int reserve(Buffer *b, size_t needed) {
    if (needed <= b->capacity) {
        return 0;
    }
    size_t capacity = b->capacity ? b->capacity * 2 : 8;
    while (capacity < needed) {
        capacity *= 2;
    }
    char **lines = realloc(b->lines, capacity * sizeof *lines);
    if (!lines) {
        return -1;
    }
    b->lines = lines;
    b->capacity = capacity;
    return 0;
}

static int append_line(Buffer *b, const char *text, size_t length) {
    if (reserve(b, b->count + 1) != 0) {
        return -1;
    }
    char *line = copy_bytes(text, length);
    if (!line) {
        return -1;
    }
    b->lines[b->count++] = line;
    return 0;
}

/* Puts the rest of the cursor line on a new line below and moves the cursor there. */
static int split_line(Buffer *b) {
    if (reserve(b, b->count + 1) != 0) {
        return -1;
    }
    char *line = b->lines[b->row];
    char *tail = copy_bytes(line + b->col, strlen(line) - b->col);
    if (!tail) {
        return -1;
    }
    line[b->col] = '\0';
    memmove(&b->lines[b->row + 2], &b->lines[b->row + 1],
            (b->count - b->row - 1) * sizeof *b->lines);
    b->lines[b->row + 1] = tail;
    b->count++;
    b->row++;
    b->col = 0;
    return 0;
}

/* Appends line row + 1 to line row and removes line row + 1. */
static int join_with_next(Buffer *b, size_t row) {
    char *first = b->lines[row];
    char *second = b->lines[row + 1];
    size_t first_length = strlen(first);
    size_t second_length = strlen(second);
    char *joined = realloc(first, first_length + second_length + 1);
    if (!joined) {
        return -1;
    }
    memcpy(joined + first_length, second, second_length + 1);
    free(second);
    memmove(&b->lines[row + 1], &b->lines[row + 2], (b->count - row - 2) * sizeof *b->lines);
    b->count--;
    b->lines[row] = joined;
    return 0;
}

static int insert_chunk(Buffer *b, const char *text, size_t length) {
    if (length == 0) {
        return 0;
    }
    char *line = b->lines[b->row];
    size_t line_length = strlen(line);
    char *grown = realloc(line, line_length + length + 1);
    if (!grown) {
        return -1;
    }
    memmove(grown + b->col + length, grown + b->col, line_length - b->col + 1);
    memcpy(grown + b->col, text, length);
    b->lines[b->row] = grown;
    b->col += length;
    return 0;
}

/* Keeps the byte column inside the line and on a character boundary. */
static void clamp_col(Buffer *b) {
    size_t length = buffer_line_length(b, b->row);
    if (b->col > length) {
        b->col = length;
    }
    while (b->col > 0 && b->col < length && is_continuation(buffer_line(b, b->row)[b->col])) {
        b->col--;
    }
}

void buffer_init(Buffer *b) {
    b->lines = NULL;
    b->count = 0;
    b->capacity = 0;
    b->row = 0;
    b->col = 0;
    b->modified = 0;
    buffer_load(b, "", 0);
}

void buffer_free(Buffer *b) {
    for (size_t i = 0; i < b->count; i++) {
        free(b->lines[i]);
    }
    free(b->lines);
    b->lines = NULL;
    b->count = 0;
    b->capacity = 0;
    b->row = 0;
    b->col = 0;
    b->modified = 0;
}

int buffer_load(Buffer *b, const char *text, size_t length) {
    buffer_free(b);
    size_t start = 0;
    for (size_t i = 0; i <= length; i++) {
        int at_newline = i < length && text[i] == '\n';
        /* A final newline ends the last line; it does not start an empty one. */
        int at_end = i == length && (start < length || b->count == 0);
        if (at_newline || at_end) {
            if (append_line(b, text + start, i - start) != 0) {
                return -1;
            }
            start = i + 1;
        }
    }
    return 0;
}

const char *buffer_line(const Buffer *b, size_t row) { return b->lines[row]; }

size_t buffer_line_length(const Buffer *b, size_t row) { return strlen(b->lines[row]); }

size_t buffer_word_count(const Buffer *b) {
    size_t words = 0;
    for (size_t row = 0; row < b->count; row++) {
        int in_word = 0;
        for (const char *p = b->lines[row]; *p; p++) {
            int space = *p == ' ' || *p == '\t' || *p == '\r';
            if (!space && !in_word) {
                words++;
            }
            in_word = !space;
        }
    }
    return words;
}

int buffer_insert(Buffer *b, const char *text, size_t length) {
    size_t start = 0;
    for (size_t i = 0; i <= length; i++) {
        if (i == length || text[i] == '\n') {
            if (insert_chunk(b, text + start, i - start) != 0) {
                return -1;
            }
            if (i < length && split_line(b) != 0) {
                return -1;
            }
            start = i + 1;
        }
    }
    b->modified = 1;
    return 0;
}

int buffer_backspace(Buffer *b) {
    if (b->col > 0) {
        char *line = b->lines[b->row];
        size_t previous = utf8_prev(line, b->col);
        memmove(line + previous, line + b->col, strlen(line) - b->col + 1);
        b->col = previous;
        b->modified = 1;
        return 0;
    }
    if (b->row == 0) {
        return 0;
    }
    size_t previous_length = buffer_line_length(b, b->row - 1);
    if (join_with_next(b, b->row - 1) != 0) {
        return -1;
    }
    b->row--;
    b->col = previous_length;
    b->modified = 1;
    return 0;
}

int buffer_delete(Buffer *b) {
    char *line = b->lines[b->row];
    size_t length = strlen(line);
    if (b->col < length) {
        size_t next = utf8_next(line, b->col);
        memmove(line + b->col, line + next, length - next + 1);
        b->modified = 1;
        return 0;
    }
    if (b->row + 1 < b->count) {
        b->modified = 1;
        return join_with_next(b, b->row);
    }
    return 0;
}

void buffer_left(Buffer *b) {
    if (b->col > 0) {
        b->col = utf8_prev(b->lines[b->row], b->col);
    } else if (b->row > 0) {
        b->row--;
        b->col = buffer_line_length(b, b->row);
    }
}

void buffer_right(Buffer *b) {
    if (b->col < buffer_line_length(b, b->row)) {
        b->col = utf8_next(b->lines[b->row], b->col);
    } else if (b->row + 1 < b->count) {
        b->row++;
        b->col = 0;
    }
}

void buffer_move_rows(Buffer *b, long delta) {
    long row = (long)b->row + delta;
    if (row < 0) {
        row = 0;
    }
    if (row >= (long)b->count) {
        row = (long)b->count - 1;
    }
    b->row = (size_t)row;
    clamp_col(b);
}

void buffer_home(Buffer *b) { b->col = 0; }

void buffer_end(Buffer *b) { b->col = buffer_line_length(b, b->row); }

void buffer_goto_line(Buffer *b, size_t row) {
    b->row = row < b->count ? row : b->count - 1;
    b->col = 0;
}

int buffer_find(Buffer *b, const char *needle) {
    if (needle[0] == '\0') {
        return 0;
    }
    /* i == 0 searches the cursor line after the cursor, the last pass searches it from the start.
     */
    for (size_t i = 0; i <= b->count; i++) {
        size_t row = (b->row + i) % b->count;
        const char *line = b->lines[row];
        size_t from = i == 0 ? b->col + 1 : 0;
        if (from > strlen(line)) {
            continue;
        }
        const char *hit = strstr(line + from, needle);
        if (hit) {
            b->row = row;
            b->col = (size_t)(hit - line);
            return 1;
        }
    }
    return 0;
}
