#ifndef TEXT_EDITOR_H
#define TEXT_EDITOR_H

#include <stdio.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LINES 1000

typedef struct {
    char *lines[MAX_LINES];
    int num_lines;
    int modified;
    char filename[256];
} TextBuffer;

void buffer_init(TextBuffer *buf);
void buffer_free(TextBuffer *buf);
int buffer_load(TextBuffer *buf, const char *filename);
int buffer_save(TextBuffer *buf, const char *filename);
int buffer_insert_line(TextBuffer *buf, int pos, const char *text);
int buffer_delete_line(TextBuffer *buf, int pos);
int buffer_append_line(TextBuffer *buf, const char *text);
const char *buffer_get_line(const TextBuffer *buf, int pos);
int buffer_line_count(const TextBuffer *buf);
void buffer_print(const TextBuffer *buf);

#endif // TEXT_EDITOR_H
