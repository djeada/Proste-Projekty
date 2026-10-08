/* HTTP logic: request parsing, MIME types, safe paths, the notes store and routing. No sockets here. */
#include "http_server.h"

#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define MAX_REQUEST_BYTES 65536
#define NOTES_PATH "/api/notes"
#define JSON_TYPE "application/json; charset=utf-8"
#define BAD_TEXT "body must be 1 to 200 bytes of plain text"

void buffer_init(Buffer *buf) {
    buf->data = NULL;
    buf->len = 0;
    buf->cap = 0;
}

void buffer_free(Buffer *buf) {
    free(buf->data);
    buffer_init(buf);
}

void buffer_append(Buffer *buf, const char *data, size_t len) {
    if (buf->len + len + 1 > buf->cap) {
        size_t cap = buf->cap > 0 ? buf->cap : 256;
        while (buf->len + len + 1 > cap) {
            cap *= 2;
        }
        buf->data = realloc(buf->data, cap);
        if (buf->data == NULL) {
            abort(); /* out of memory: this small server cannot recover */
        }
        buf->cap = cap;
    }
    if (len > 0) {
        memcpy(buf->data + buf->len, data, len);
    }
    buf->len += len;
    buf->data[buf->len] = '\0';
}

/* ---- Notes store ---- */

void store_init(NoteStore *store) {
    store->count = 0;
    store->next_id = 1;
}

static int find_index(const NoteStore *store, int id) {
    for (size_t i = 0; i < store->count; i++) {
        if (store->items[i].id == id) {
            return (int)i;
        }
    }
    return -1;
}

int store_add(NoteStore *store, const char *text) {
    if (store->count == MAX_NOTES) {
        return 0;
    }
    Note *note = &store->items[store->count++];
    note->id = store->next_id++;
    snprintf(note->text, sizeof note->text, "%s", text);
    return note->id;
}

int store_update(NoteStore *store, int id, const char *text) {
    int i = find_index(store, id);
    if (i < 0) {
        return 0;
    }
    snprintf(store->items[i].text, sizeof store->items[i].text, "%s", text);
    return 1;
}

int store_delete(NoteStore *store, int id) {
    int i = find_index(store, id);
    if (i < 0) {
        return 0;
    }
    memmove(&store->items[i], &store->items[i + 1], (store->count - (size_t)i - 1) * sizeof(Note));
    store->count--;
    return 1;
}

/* ---- Request parsing ---- */

static int equal_ignore_case(const char *a, size_t a_len, const char *b) {
    if (a_len != strlen(b)) {
        return 0;
    }
    for (size_t i = 0; i < a_len; i++) {
        if (tolower((unsigned char)a[i]) != tolower((unsigned char)b[i])) {
            return 0;
        }
    }
    return 1;
}

static int parse_id(const char *text, int *id) {
    char *end;
    long value = strtol(text, &end, 10);
    if (!isdigit((unsigned char)text[0]) || *end != '\0' || value <= 0 || value > INT_MAX) {
        return 0;
    }
    *id = (int)value;
    return 1;
}

ParseResult parse_request(const char *raw, size_t len, Request *req) {
    if (len > MAX_REQUEST_BYTES) {
        return REQUEST_MALFORMED;
    }
    const char *head_end = NULL; /* the "\r\n" of the blank line that ends the headers */
    for (size_t i = 0; i + 4 <= len && head_end == NULL; i++) {
        if (memcmp(raw + i, "\r\n\r\n", 4) == 0) {
            head_end = raw + i;
        }
    }
    if (head_end == NULL) {
        return REQUEST_INCOMPLETE;
    }

    /* Request line, e.g. "POST /api/notes?x=1 HTTP/1.1" */
    const char *line_end = raw;
    while (line_end[0] != '\r' || line_end[1] != '\n') {
        line_end++;
    }
    char line[1024], target[1024], version[16];
    if ((size_t)(line_end - raw) >= sizeof line) {
        return REQUEST_MALFORMED;
    }
    memcpy(line, raw, (size_t)(line_end - raw));
    line[line_end - raw] = '\0';
    if (sscanf(line, "%15s %1023s %15s", req->method, target, version) != 3 ||
        strncmp(version, "HTTP/", 5) != 0) {
        return REQUEST_MALFORMED;
    }
    char *query = strchr(target, '?');
    if (query != NULL) {
        *query = '\0';
    }
    if (strlen(target) >= sizeof req->path) {
        return REQUEST_MALFORMED;
    }
    strcpy(req->path, target);

    /* Headers: only Content-Length matters here. */
    size_t content_length = 0;
    const char *p = line_end + 2;
    while (p < head_end) {
        const char *end = p;
        while (end[0] != '\r' || end[1] != '\n') {
            end++;
        }
        const char *colon = memchr(p, ':', (size_t)(end - p));
        if (colon == NULL) {
            return REQUEST_MALFORMED;
        }
        if (equal_ignore_case(p, (size_t)(colon - p), "Content-Length") &&
            (sscanf(colon + 1, " %zu", &content_length) != 1 || content_length > MAX_REQUEST_BYTES)) {
            return REQUEST_MALFORMED;
        }
        p = end + 2;
    }

    size_t body_start = (size_t)(head_end - raw) + 4;
    if (len - body_start < content_length) {
        return REQUEST_INCOMPLETE;
    }
    req->body = raw + body_start;
    req->body_len = content_length;
    return REQUEST_COMPLETE;
}

/* ---- Responses ---- */

static const char *reason_phrase(int status) {
    switch (status) {
    case 200: return "OK";
    case 201: return "Created";
    case 204: return "No Content";
    case 400: return "Bad Request";
    case 403: return "Forbidden";
    case 404: return "Not Found";
    case 405: return "Method Not Allowed";
    case 507: return "Insufficient Storage";
    default: return "Unknown";
    }
}

void response_init(Response *res) {
    res->status = 200;
    res->content_type = NULL;
    buffer_init(&res->body);
}

void response_free(Response *res) {
    buffer_free(&res->body);
}

/* An error with a JSON body, e.g. {"error":"not found"}. The messages contain no quotes. */
void response_error(Response *res, int status, const char *message) {
    char json[128];
    int n = snprintf(json, sizeof json, "{\"error\":\"%s\"}", message);
    res->status = status;
    res->content_type = JSON_TYPE;
    buffer_append(&res->body, json, (size_t)n);
}

void response_format(const Response *res, Buffer *out) {
    char head[256];
    int n = snprintf(head, sizeof head, "HTTP/1.1 %d %s\r\nConnection: close\r\n", res->status,
                     reason_phrase(res->status));
    buffer_append(out, head, (size_t)n);
    if (res->content_type != NULL) { /* a 204 response has no body and no Content-Type */
        n = snprintf(head, sizeof head, "Content-Type: %s\r\nContent-Length: %zu\r\n", res->content_type,
                     res->body.len);
        buffer_append(out, head, (size_t)n);
    }
    buffer_append(out, "\r\n", 2);
    buffer_append(out, res->body.data, res->body.len);
}

/* ---- Files ---- */

static const struct {
    const char *extension;
    const char *type;
} MIME_TYPES[] = {
    {".html", "text/html; charset=utf-8"},
    {".css", "text/css; charset=utf-8"},
    {".js", "text/javascript; charset=utf-8"},
    {".json", "application/json; charset=utf-8"},
    {".txt", "text/plain; charset=utf-8"},
    {".png", "image/png"},
    {".jpg", "image/jpeg"},
    {".gif", "image/gif"},
    {".svg", "image/svg+xml"},
};

const char *mime_type(const char *file_name) {
    const char *dot = strrchr(file_name, '.');
    if (dot != NULL) {
        for (size_t i = 0; i < sizeof MIME_TYPES / sizeof MIME_TYPES[0]; i++) {
            if (equal_ignore_case(dot, strlen(dot), MIME_TYPES[i].extension)) {
                return MIME_TYPES[i].type;
            }
        }
    }
    return "application/octet-stream";
}

/* Turns the URL path into a file inside public_dir. Returns 0, or -1 if the path could leave it.
 * Percent escapes are refused, so "%2e%2e" cannot hide a "..". */
int safe_path(const char *public_dir, const char *url_path, char *out, size_t out_size) {
    if (strchr(url_path, '%') != NULL || strchr(url_path, '\\') != NULL) {
        return -1;
    }
    for (const char *p = url_path; (p = strstr(p, "/..")) != NULL; p += 3) {
        if (p[3] == '/' || p[3] == '\0') {
            return -1;
        }
    }
    const char *relative = url_path;
    while (*relative == '/') {
        relative++;
    }
    if (*relative == '\0') {
        relative = "index.html";
    }
    int n = snprintf(out, out_size, "%s/%s", public_dir, relative);
    return (n < 0 || (size_t)n >= out_size) ? -1 : 0;
}

static void handle_static(const Request *req, const char *public_dir, Response *res) {
    char file[1024];
    struct stat info;
    FILE *f = NULL;
    if (strcmp(req->method, "GET") != 0) {
        response_error(res, 405, "method not allowed");
    } else if (safe_path(public_dir, req->path, file, sizeof file) != 0) {
        response_error(res, 403, "forbidden");
    } else if (stat(file, &info) != 0 || !S_ISREG(info.st_mode) || (f = fopen(file, "rb")) == NULL) {
        response_error(res, 404, "not found");
    } else {
        char chunk[4096];
        size_t n;
        while ((n = fread(chunk, 1, sizeof chunk, f)) > 0) {
            buffer_append(&res->body, chunk, n);
        }
        res->status = 200;
        res->content_type = mime_type(file);
    }
    if (f != NULL) {
        fclose(f);
    }
}

/* ---- Notes API ---- */

/* Appends {"id":..,"text":".."} with quotes, backslashes and control characters escaped. */
static void append_note(Buffer *out, int id, const char *text) {
    char part[32];
    int n = snprintf(part, sizeof part, "{\"id\":%d,\"text\":\"", id);
    buffer_append(out, part, (size_t)n);
    for (const char *p = text; *p != '\0'; p++) {
        unsigned char c = (unsigned char)*p;
        if (c == '"' || c == '\\') {
            snprintf(part, sizeof part, "\\%c", c);
        } else if (c < 0x20) {
            snprintf(part, sizeof part, "\\u%04x", c);
        } else {
            part[0] = (char)c;
            part[1] = '\0';
        }
        buffer_append(out, part, strlen(part));
    }
    buffer_append(out, "\"}", 2);
}

/* Copies the body, without surrounding whitespace, into text. Returns 0 if it is empty or too long. */
static int read_note_text(const Request *req, char text[MAX_NOTE_BYTES + 1]) {
    const char *start = req->body;
    const char *end = req->body + req->body_len;
    while (start < end && isspace((unsigned char)*start)) {
        start++;
    }
    while (end > start && isspace((unsigned char)end[-1])) {
        end--;
    }
    size_t len = (size_t)(end - start);
    if (len == 0 || len > MAX_NOTE_BYTES) {
        return 0;
    }
    memcpy(text, start, len);
    text[len] = '\0';
    return 1;
}

/* Sends a note as JSON with the given status. */
static void respond_note(Response *res, int status, int id, const char *text) {
    res->status = status;
    res->content_type = JSON_TYPE;
    append_note(&res->body, id, text);
}

static void handle_notes(const Request *req, NoteStore *store, Response *res) {
    const char *rest = req->path + strlen(NOTES_PATH);
    char text[MAX_NOTE_BYTES + 1];
    int id = 0;

    if (*rest == '\0') { /* /api/notes */
        if (strcmp(req->method, "GET") == 0) {
            res->status = 200;
            res->content_type = JSON_TYPE;
            buffer_append(&res->body, "[", 1);
            for (size_t i = 0; i < store->count; i++) {
                if (i > 0) {
                    buffer_append(&res->body, ",", 1);
                }
                append_note(&res->body, store->items[i].id, store->items[i].text);
            }
            buffer_append(&res->body, "]", 1);
        } else if (strcmp(req->method, "POST") != 0) {
            response_error(res, 405, "method not allowed");
        } else if (!read_note_text(req, text)) {
            response_error(res, 400, BAD_TEXT);
        } else if ((id = store_add(store, text)) == 0) {
            response_error(res, 507, "too many notes");
        } else {
            respond_note(res, 201, id, text);
        }
        return;
    }

    /* /api/notes/<id> */
    if (*rest != '/' || !parse_id(rest + 1, &id)) {
        response_error(res, 404, "not found");
    } else if (strcmp(req->method, "PUT") == 0) {
        if (!read_note_text(req, text)) {
            response_error(res, 400, BAD_TEXT);
        } else if (!store_update(store, id, text)) {
            response_error(res, 404, "not found");
        } else {
            respond_note(res, 200, id, text);
        }
    } else if (strcmp(req->method, "DELETE") == 0) {
        if (store_delete(store, id)) {
            res->status = 204;
        } else {
            response_error(res, 404, "not found");
        }
    } else {
        response_error(res, 405, "method not allowed");
    }
}

void handle_request(const Request *req, const char *public_dir, NoteStore *store, Response *res) {
    size_t len = strlen(NOTES_PATH);
    int is_notes = strncmp(req->path, NOTES_PATH, len) == 0 && (req->path[len] == '\0' || req->path[len] == '/');
    if (is_notes) {
        handle_notes(req, store, res);
    } else {
        handle_static(req, public_dir, res);
    }
}
