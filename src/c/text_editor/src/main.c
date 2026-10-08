/* Terminal user interface of the text editor, drawn with ncurses. */
#define _XOPEN_SOURCE_EXTENDED 1
#include <curses.h>
#include <errno.h>
#include <limits.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "text_editor.h"

#define CTRL(key) ((key) & 0x1f)
#define STATUS_SIZE 1024

typedef struct {
    Buffer buf;
    char filename[256]; /* empty for a file that has not been saved yet */
    char needle[128];   /* the last text searched for */
    char message[256];  /* shown in the status bar until the next key */
    size_t top;         /* first visible line */
    size_t left;        /* first visible column, in characters */
} Editor;

static int draw(Editor *e);

/* Stores the next key in ch. Returns 1 for special keys (arrows, F3, ...), 0 for characters. */
static int read_input(wint_t *ch) {
    *ch = 0;
    return get_wch(ch) == KEY_CODE_YES;
}

static size_t encode_char(wint_t ch, char *out) {
    mbstate_t state;
    memset(&state, 0, sizeof state);
    size_t n = wcrtomb(out, (wchar_t)ch, &state);
    return n == (size_t)-1 ? 0 : n;
}

static void check_memory(Editor *e, int result) {
    if (result != 0) {
        snprintf(e->message, sizeof e->message, "Out of memory");
    }
}

static void insert_text(Editor *e, const char *text, size_t length) {
    check_memory(e, buffer_insert(&e->buf, text, length));
}

/* Reads a line in the status bar. Returns 0 when cancelled with Esc. */
static int prompt(Editor *e, const char *label, char *answer, size_t size) {
    int rows = getmaxy(stdscr);
    for (;;) {
        snprintf(e->message, sizeof e->message, "%s%s", label, answer);
        int x = draw(e);
        move(rows - 1, x);
        refresh();

        wint_t ch;
        if (read_input(&ch)) {
            ch = ch == KEY_BACKSPACE ? '\b' : ch == KEY_ENTER ? '\n' : 0;
        }
        size_t length = strlen(answer);
        if (ch == '\n' || ch == '\r') {
            e->message[0] = '\0';
            return 1;
        }
        if (ch == 27) {
            e->message[0] = '\0';
            return 0;
        }
        if (ch == '\b' || ch == 127) {
            length = utf8_prev(answer, length);
            answer[length] = '\0';
        } else if (ch >= ' ') {
            char bytes[MB_LEN_MAX];
            size_t n = encode_char(ch, bytes);
            if (n > 0 && length + n < size) {
                memcpy(answer + length, bytes, n);
                answer[length + n] = '\0';
            }
        }
    }
}

static int confirm(Editor *e, const char *question) {
    snprintf(e->message, sizeof e->message, "%s", question);
    draw(e);
    wint_t ch;
    read_input(&ch);
    e->message[0] = '\0';
    return ch == 'y' || ch == 'Y';
}

static int open_file(Editor *e, const char *path) {
    snprintf(e->filename, sizeof e->filename, "%s", path);
    FILE *f = fopen(path, "rb");
    if (!f) {
        return errno == ENOENT; /* a missing file is a new file, created by Ctrl+S */
    }
    size_t capacity = 4096;
    size_t length = 0;
    char *text = malloc(capacity);
    while (text && !feof(f) && !ferror(f)) {
        if (length == capacity) {
            char *grown = realloc(text, 2 * capacity);
            if (!grown) {
                break;
            }
            text = grown;
            capacity *= 2;
        }
        length += fread(text + length, 1, capacity - length, f);
    }
    int ok = text && feof(f) && !ferror(f) && buffer_load(&e->buf, text, length) == 0;
    fclose(f);
    free(text);
    return ok;
}

static void save(Editor *e) {
    if (e->filename[0] == '\0') {
        char name[sizeof e->filename] = "";
        if (!prompt(e, "Save as: ", name, sizeof name) || name[0] == '\0') {
            snprintf(e->message, sizeof e->message, "Save cancelled");
            return;
        }
        snprintf(e->filename, sizeof e->filename, "%s", name);
    }
    FILE *f = fopen(e->filename, "wb");
    if (!f) {
        snprintf(e->message, sizeof e->message, "Cannot open the file for writing");
        return;
    }
    for (size_t i = 0; i < e->buf.count; i++) {
        fputs(buffer_line(&e->buf, i), f);
        fputc('\n', f);
    }
    if (fclose(f) != 0) {
        snprintf(e->message, sizeof e->message, "Cannot write the file");
        return;
    }
    e->buf.modified = 0;
    snprintf(e->message, sizeof e->message, "Saved");
}

static void find_next(Editor *e) {
    if (e->needle[0] == '\0') {
        snprintf(e->message, sizeof e->message, "Nothing to search for (Ctrl+F)");
        return;
    }
    size_t row = e->buf.row;
    size_t col = e->buf.col;
    if (!buffer_find(&e->buf, e->needle)) {
        snprintf(e->message, sizeof e->message, "Not found: %s", e->needle);
    } else if (e->buf.row < row || (e->buf.row == row && e->buf.col <= col)) {
        snprintf(e->message, sizeof e->message, "Found (wrapped around to the start)");
    } else {
        snprintf(e->message, sizeof e->message, "Found");
    }
}

static void go_to_line(Editor *e) {
    char answer[32] = "";
    if (!prompt(e, "Go to line: ", answer, sizeof answer)) {
        return;
    }
    char *end;
    unsigned long line = strtoul(answer, &end, 10);
    if (answer[0] == '\0' || *end != '\0' || line < 1 || line > e->buf.count) {
        snprintf(e->message, sizeof e->message, "Enter a line from 1 to %zu", e->buf.count);
        return;
    }
    buffer_goto_line(&e->buf, line - 1);
}

static void new_file(Editor *e) {
    buffer_free(&e->buf);
    buffer_init(&e->buf);
    e->filename[0] = '\0';
    e->top = 0;
    e->left = 0;
}

static void handle_special_key(Editor *e, int key) {
    int rows = getmaxy(stdscr);
    long page = rows > 2 ? rows - 2 : 1;
    switch (key) {
        case KEY_LEFT:
            buffer_left(&e->buf);
            break;
        case KEY_RIGHT:
            buffer_right(&e->buf);
            break;
        case KEY_UP:
            buffer_move_rows(&e->buf, -1);
            break;
        case KEY_DOWN:
            buffer_move_rows(&e->buf, 1);
            break;
        case KEY_PPAGE:
            buffer_move_rows(&e->buf, -page);
            break;
        case KEY_NPAGE:
            buffer_move_rows(&e->buf, page);
            break;
        case KEY_HOME:
            buffer_home(&e->buf);
            break;
        case KEY_END:
            buffer_end(&e->buf);
            break;
        case KEY_DC:
            check_memory(e, buffer_delete(&e->buf));
            break;
        case KEY_BACKSPACE:
            check_memory(e, buffer_backspace(&e->buf));
            break;
        case KEY_ENTER:
            insert_text(e, "\n", 1);
            break;
        case KEY_F(3):
            find_next(e);
            break;
        default:
            break;
    }
}

/* Handles one key. Returns 1 when the editor should quit. */
static int handle_input(Editor *e, wint_t ch, int special) {
    e->message[0] = '\0';
    if (special) {
        handle_special_key(e, (int)ch);
        return 0;
    }
    switch (ch) {
        case CTRL('q'):
            return !e->buf.modified || confirm(e, "Unsaved changes. Quit anyway? (y/n)");
        case CTRL('s'):
            save(e);
            break;
        case CTRL('n'):
            if (!e->buf.modified || confirm(e, "Unsaved changes. Start a new file? (y/n)")) {
                new_file(e);
            }
            break;
        case CTRL('f'): {
            char needle[sizeof e->needle];
            snprintf(needle, sizeof needle, "%s", e->needle);
            if (prompt(e, "Find: ", needle, sizeof needle)) {
                snprintf(e->needle, sizeof e->needle, "%s", needle);
                find_next(e);
            }
            break;
        }
        case CTRL('g'):
            go_to_line(e);
            break;
        case '\n':
        case '\r':
            insert_text(e, "\n", 1);
            break;
        case '\b':
        case 127:
            check_memory(e, buffer_backspace(&e->buf));
            break;
        case '\t':
            insert_text(e, "    ", 4);
            break;
        default:
            if (ch >= ' ') {
                char bytes[MB_LEN_MAX];
                size_t n = encode_char(ch, bytes);
                insert_text(e, bytes, n);
            }
            break;
    }
    return 0;
}

/* Keeps the cursor inside the visible part of the text area. */
static void follow_cursor(Editor *e, size_t text_rows, size_t cols, size_t x) {
    size_t row = e->buf.row;
    if (row < e->top) {
        e->top = row;
    }
    if (row >= e->top + text_rows) {
        e->top = row - text_rows + 1;
    }
    if (x < e->left) {
        e->left = x;
    }
    if (x >= e->left + cols) {
        e->left = x - cols + 1;
    }
}

static void draw_line(const char *line, size_t left, int width) {
    const char *p = line + utf8_offset(line, left);
    for (int x = 0; x < width && *p != '\0'; x++) {
        size_t n = utf8_next(p, 0);
        if ((unsigned char)*p < ' ') {
            addch(' ');
        } else {
            addnstr(p, (int)n);
        }
        p += n;
    }
}

/* Draws the status bar on row y and returns the column where its text ends. */
static int draw_status(const Editor *e, int y, int cols) {
    const Buffer *b = &e->buf;
    const char *line = buffer_line(b, b->row);
    char status[STATUS_SIZE];
    snprintf(status, sizeof status, " %s%s%s%s | Ln %zu, Col %zu | %zu lines, %zu words",
             e->message, e->message[0] ? " | " : "", e->filename[0] ? e->filename : "(new file)",
             b->modified ? " [modified]" : "", b->row + 1, utf8_char_count(line, b->col) + 1,
             b->count, buffer_word_count(b));
    attron(A_REVERSE);
    mvhline(y, 0, ' ', cols);
    mvaddnstr(y, 0, status, cols);
    attroff(A_REVERSE);
    size_t length = strlen(status);
    return (int)(length < (size_t)cols ? length : (size_t)cols - 1);
}

/* Draws the screen and returns the status bar column where its text ends. */
static int draw(Editor *e) {
    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    size_t text_rows = rows > 2 ? (size_t)(rows - 1) : 1;
    size_t x = utf8_char_count(buffer_line(&e->buf, e->buf.row), e->buf.col);
    follow_cursor(e, text_rows, (size_t)cols, x);

    erase();
    for (size_t y = 0; y < text_rows && e->top + y < e->buf.count; y++) {
        move((int)y, 0);
        draw_line(buffer_line(&e->buf, e->top + y), e->left, cols);
    }
    int status_x = draw_status(e, rows - 1, cols);
    move((int)(e->buf.row - e->top), (int)(x - e->left));
    refresh();
    return status_x;
}

int main(int argc, char **argv) {
    if (argc > 2) {
        fprintf(stderr, "usage: %s [file]\n", argv[0]);
        return 1;
    }
    Editor e;
    memset(&e, 0, sizeof e);
    buffer_init(&e.buf);
    if (argc == 2 && !open_file(&e, argv[1])) {
        fprintf(stderr, "text_editor: cannot open %s\n", argv[1]);
        buffer_free(&e.buf);
        return 1;
    }

    setlocale(LC_ALL, "");
    initscr();
    raw();
    noecho();
    keypad(stdscr, TRUE);
    set_escdelay(25);
    for (;;) {
        draw(&e);
        wint_t ch;
        int special = read_input(&ch);
        if (handle_input(&e, ch, special)) {
            break;
        }
    }
    endwin();
    buffer_free(&e.buf);
    return 0;
}
