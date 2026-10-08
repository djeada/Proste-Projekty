/* HTTP logic: parsing, responses, MIME types, safe paths, the notes store and routing. No sockets. */
#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <stddef.h>

#define MAX_NOTE_BYTES 200
#define MAX_NOTES 100

typedef struct {
    char *data;
    size_t len;
    size_t cap;
} Buffer;

void buffer_init(Buffer *buf);
void buffer_free(Buffer *buf);
void buffer_append(Buffer *buf, const char *data, size_t len);

typedef struct {
    int id;
    char text[MAX_NOTE_BYTES + 1];
} Note;

/* Notes in memory, numbered 1, 2, 3, ... and never reused. */
typedef struct {
    Note items[MAX_NOTES];
    size_t count;
    int next_id;
} NoteStore;

void store_init(NoteStore *store);
int store_add(NoteStore *store, const char *text); /* the new id, or 0 if full */
int store_update(NoteStore *store, int id, const char *text); /* 1 if the note exists */
int store_delete(NoteStore *store, int id); /* 1 if the note existed */

typedef enum { REQUEST_COMPLETE, REQUEST_INCOMPLETE, REQUEST_MALFORMED } ParseResult;

typedef struct {
    char method[16];
    char path[512]; /* without the query string */
    const char *body; /* points into the raw request bytes */
    size_t body_len;
} Request;

ParseResult parse_request(const char *raw, size_t len, Request *req);

typedef struct {
    int status;
    const char *content_type; /* NULL when there is no body */
    Buffer body;
} Response;

void response_init(Response *res);
void response_free(Response *res);
void response_error(Response *res, int status, const char *message);
void response_format(const Response *res, Buffer *out);

const char *mime_type(const char *file_name);
int safe_path(const char *public_dir, const char *url_path, char *out, size_t out_size);
void handle_request(const Request *req, const char *public_dir, NoteStore *store, Response *res);

#endif
