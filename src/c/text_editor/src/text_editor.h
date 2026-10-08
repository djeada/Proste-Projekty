/* The text buffer: lines of text and the cursor. No input or output here. */
#ifndef TEXT_EDITOR_H
#define TEXT_EDITOR_H

#include <stddef.h>

typedef struct {
    char **lines;    /* every line is a NUL-terminated string without '\n' */
    size_t count;    /* number of lines, always at least 1 */
    size_t capacity; /* allocated slots in lines */
    size_t row;      /* cursor line */
    size_t col;      /* cursor position in bytes inside the line (UTF-8) */
    int modified;    /* 1 after any change, the UI sets it back to 0 on save */
} Buffer;

void buffer_init(Buffer *b);
void buffer_free(Buffer *b);

/* Replaces the contents with text, splitting it on '\n'. Returns 0 or -1 (out of memory). */
int buffer_load(Buffer *b, const char *text, size_t length);

const char *buffer_line(const Buffer *b, size_t row);
size_t buffer_line_length(const Buffer *b, size_t row);
size_t buffer_word_count(const Buffer *b);

/* Editing at the cursor. Text may contain '\n', which splits the line. */
int buffer_insert(Buffer *b, const char *text, size_t length);
int buffer_backspace(Buffer *b);
int buffer_delete(Buffer *b);

void buffer_left(Buffer *b);
void buffer_right(Buffer *b);
void buffer_move_rows(Buffer *b, long delta); /* up with negative, down with positive */
void buffer_home(Buffer *b);
void buffer_end(Buffer *b);
void buffer_goto_line(Buffer *b, size_t row);

/* Finds needle after the cursor, wrapping to the start. Moves the cursor to the match. */
int buffer_find(Buffer *b, const char *needle);

/* UTF-8 helpers: positions are byte offsets, lengths are in characters. */
size_t utf8_next(const char *s, size_t pos);
size_t utf8_prev(const char *s, size_t pos);
size_t utf8_char_count(const char *s, size_t bytes);
size_t utf8_offset(const char *s, size_t chars);

#endif /* TEXT_EDITOR_H */
