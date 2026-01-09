#include "text_editor.h"
#include <stdlib.h>
#include <string.h>

void buffer_init(TextBuffer *buf) {
    for (int i = 0; i < MAX_LINES; i++) {
        buf->lines[i] = NULL;
    }
    buf->num_lines = 0;
    buf->modified = 0;
    buf->filename[0] = '\0';
}

void buffer_free(TextBuffer *buf) {
    for (int i = 0; i < buf->num_lines; i++) {
        free(buf->lines[i]);
        buf->lines[i] = NULL;
    }
    buf->num_lines = 0;
    buf->modified = 0;
}

int buffer_load(TextBuffer *buf, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        return 0;
    }

    buffer_free(buf);
    strncpy(buf->filename, filename, sizeof(buf->filename) - 1);
    buf->filename[sizeof(buf->filename) - 1] = '\0';

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) && buf->num_lines < MAX_LINES) {
        // Remove trailing newline
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        buf->lines[buf->num_lines] = strdup(line);
        if (!buf->lines[buf->num_lines]) {
            fclose(file);
            return 0;
        }
        buf->num_lines++;
    }

    fclose(file);
    buf->modified = 0;
    return 1;
}

int buffer_save(TextBuffer *buf, const char *filename) {
    const char *target = filename ? filename : buf->filename;
    if (!target || target[0] == '\0') {
        return 0;
    }

    FILE *file = fopen(target, "w");
    if (!file) {
        return 0;
    }

    for (int i = 0; i < buf->num_lines; i++) {
        fprintf(file, "%s\n", buf->lines[i]);
    }

    fclose(file);
    strncpy(buf->filename, target, sizeof(buf->filename) - 1);
    buf->filename[sizeof(buf->filename) - 1] = '\0';
    buf->modified = 0;
    return 1;
}

int buffer_insert_line(TextBuffer *buf, int pos, const char *text) {
    if (pos < 0 || pos > buf->num_lines || buf->num_lines >= MAX_LINES) {
        return 0;
    }

    // Shift lines down
    for (int i = buf->num_lines; i > pos; i--) {
        buf->lines[i] = buf->lines[i - 1];
    }

    buf->lines[pos] = strdup(text);
    if (!buf->lines[pos]) {
        return 0;
    }
    buf->num_lines++;
    buf->modified = 1;
    return 1;
}

int buffer_delete_line(TextBuffer *buf, int pos) {
    if (pos < 0 || pos >= buf->num_lines) {
        return 0;
    }

    free(buf->lines[pos]);

    // Shift lines up
    for (int i = pos; i < buf->num_lines - 1; i++) {
        buf->lines[i] = buf->lines[i + 1];
    }
    buf->lines[buf->num_lines - 1] = NULL;
    buf->num_lines--;
    buf->modified = 1;
    return 1;
}

int buffer_append_line(TextBuffer *buf, const char *text) {
    return buffer_insert_line(buf, buf->num_lines, text);
}

const char *buffer_get_line(const TextBuffer *buf, int pos) {
    if (pos < 0 || pos >= buf->num_lines) {
        return NULL;
    }
    return buf->lines[pos];
}

int buffer_line_count(const TextBuffer *buf) {
    return buf->num_lines;
}

void buffer_print(const TextBuffer *buf) {
    for (int i = 0; i < buf->num_lines; i++) {
        printf("%4d: %s\n", i + 1, buf->lines[i]);
    }
}
