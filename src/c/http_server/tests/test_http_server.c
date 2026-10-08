/* Tests of the logic: no sockets are used. */
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "http_server.h"

#ifndef PUBLIC_DIR
#define PUBLIC_DIR "public"
#endif

/* Sends one raw request to the logic; the caller frees res. */
static void send_request(const char *raw, NoteStore *store, Response *res) {
    Request req;
    response_init(res);
    assert(parse_request(raw, strlen(raw), &req) == REQUEST_COMPLETE);
    handle_request(&req, PUBLIC_DIR, store, res);
}

/* Sends a request with a plain-text body; Content-Length is measured from the body. */
static void send_body(const char *request_line, const char *body, NoteStore *store, Response *res) {
    char raw[1024];
    snprintf(raw, sizeof raw, "%s HTTP/1.1\r\nContent-Length: %zu\r\n\r\n%s", request_line, strlen(body), body);
    send_request(raw, store, res);
}

static void test_parse_request_line_and_headers(void) {
    Request req;
    const char *raw = "POST /api/notes?x=1 HTTP/1.1\r\nHost: localhost\r\nContent-Length: 5\r\n\r\nhello";
    assert(parse_request(raw, strlen(raw), &req) == REQUEST_COMPLETE);
    assert(strcmp(req.method, "POST") == 0);
    assert(strcmp(req.path, "/api/notes") == 0);
    assert(req.body_len == 5);
    assert(memcmp(req.body, "hello", 5) == 0);
}

static void test_parse_waits_for_more_bytes(void) {
    Request req;
    const char *short_body = "POST / HTTP/1.1\r\nContent-Length: 10\r\n\r\nhello";
    const char *no_blank_line = "GET / HTTP/1.1\r\nHost: x";
    assert(parse_request(short_body, strlen(short_body), &req) == REQUEST_INCOMPLETE);
    assert(parse_request(no_blank_line, strlen(no_blank_line), &req) == REQUEST_INCOMPLETE);
}

static void test_parse_rejects_bad_requests(void) {
    Request req;
    const char *bad_line = "INVALID\r\n\r\n";
    const char *bad_length = "POST / HTTP/1.1\r\nContent-Length: abc\r\n\r\n";
    const char *bad_header = "GET / HTTP/1.1\r\nNoColonHere\r\n\r\n";
    assert(parse_request(bad_line, strlen(bad_line), &req) == REQUEST_MALFORMED);
    assert(parse_request(bad_length, strlen(bad_length), &req) == REQUEST_MALFORMED);
    assert(parse_request(bad_header, strlen(bad_header), &req) == REQUEST_MALFORMED);
}

static void test_mime_types(void) {
    assert(strcmp(mime_type("index.html"), "text/html; charset=utf-8") == 0);
    assert(strcmp(mime_type("PUBLIC/STYLE.CSS"), "text/css; charset=utf-8") == 0);
    assert(strcmp(mime_type("logo.png"), "image/png") == 0);
    assert(strcmp(mime_type("archive.bin"), "application/octet-stream") == 0);
}

static void test_safe_path(void) {
    char out[1024];
    assert(safe_path("public", "/", out, sizeof out) == 0);
    assert(strcmp(out, "public/index.html") == 0);
    assert(safe_path("public", "/css/site.css", out, sizeof out) == 0);
    assert(strcmp(out, "public/css/site.css") == 0);
    assert(safe_path("public", "/../secret.txt", out, sizeof out) == -1);
    assert(safe_path("public", "/css/../../secret.txt", out, sizeof out) == -1);
    assert(safe_path("public", "/%2e%2e/secret.txt", out, sizeof out) == -1);
    assert(safe_path("public", "/a\\..\\secret", out, sizeof out) == -1);
}

static void test_store_add_update_delete(void) {
    NoteStore store;
    store_init(&store);
    int first = store_add(&store, "first");
    int second = store_add(&store, "second");
    assert(first == 1 && second == 2);
    assert(store_update(&store, first, "changed") == 1);
    assert(store_update(&store, 99, "missing") == 0);
    assert(store_delete(&store, first) == 1);
    assert(store_delete(&store, first) == 0);
    assert(store_add(&store, "third") == 3); /* ids are never reused */
    assert(store.count == 2);
}

static void test_store_is_full_at_max_notes(void) {
    NoteStore store;
    store_init(&store);
    for (int i = 0; i < MAX_NOTES; i++) {
        assert(store_add(&store, "x") != 0);
    }
    assert(store_add(&store, "one too many") == 0);
}

static void test_static_files(void) {
    NoteStore store;
    Response res;
    store_init(&store);

    send_request("GET / HTTP/1.1\r\n\r\n", &store, &res);
    assert(res.status == 200);
    assert(strcmp(res.content_type, "text/html; charset=utf-8") == 0);
    assert(strstr(res.body.data, "<title>Notes</title>") != NULL);
    response_free(&res);

    send_request("GET /missing.txt HTTP/1.1\r\n\r\n", &store, &res);
    assert(res.status == 404);
    response_free(&res);

    send_request("GET /../CMakeLists.txt HTTP/1.1\r\n\r\n", &store, &res);
    assert(res.status == 403);
    response_free(&res);

    send_request("GET /%2e%2e/CMakeLists.txt HTTP/1.1\r\n\r\n", &store, &res);
    assert(res.status == 403);
    response_free(&res);

    send_request("POST /index.html HTTP/1.1\r\n\r\n", &store, &res);
    assert(res.status == 405);
    response_free(&res);
}

static void test_notes_api_create_list_update_delete(void) {
    NoteStore store;
    Response res;
    store_init(&store);

    send_request("GET /api/notes HTTP/1.1\r\n\r\n", &store, &res);
    assert(res.status == 200);
    assert(strcmp(res.body.data, "[]") == 0);
    response_free(&res);

    send_body("POST /api/notes", "  buy milk  ", &store, &res);
    assert(res.status == 201);
    assert(strcmp(res.body.data, "{\"id\":1,\"text\":\"buy milk\"}") == 0);
    response_free(&res);

    send_body("PUT /api/notes/1", "eggs", &store, &res);
    assert(res.status == 200);
    assert(strcmp(res.body.data, "{\"id\":1,\"text\":\"eggs\"}") == 0);
    response_free(&res);

    send_request("GET /api/notes HTTP/1.1\r\n\r\n", &store, &res);
    assert(strcmp(res.body.data, "[{\"id\":1,\"text\":\"eggs\"}]") == 0);
    response_free(&res);

    send_request("DELETE /api/notes/1 HTTP/1.1\r\n\r\n", &store, &res);
    assert(res.status == 204);
    assert(res.content_type == NULL && res.body.len == 0);
    response_free(&res);
}

static void test_notes_api_errors(void) {
    NoteStore store;
    Response res;
    store_init(&store);

    send_body("PUT /api/notes/7", "x", &store, &res);
    assert(res.status == 404);
    response_free(&res);

    send_request("DELETE /api/notes/7 HTTP/1.1\r\n\r\n", &store, &res);
    assert(res.status == 404);
    response_free(&res);

    send_request("DELETE /api/notes/abc HTTP/1.1\r\n\r\n", &store, &res);
    assert(res.status == 404);
    response_free(&res);

    send_request("DELETE /api/notes/1abc HTTP/1.1\r\n\r\n", &store, &res);
    assert(res.status == 404);
    response_free(&res);

    send_request("GET /api/notes/1/2 HTTP/1.1\r\n\r\n", &store, &res);
    assert(res.status == 404);
    response_free(&res);

    send_body("POST /api/notes", "   ", &store, &res);
    assert(res.status == 400);
    response_free(&res);

    send_body("POST /api/notes", "x", &store, &res); /* a valid note, to test PUT below */
    response_free(&res);
    char too_long[MAX_NOTE_BYTES + 2];
    memset(too_long, 'x', sizeof too_long - 1);
    too_long[sizeof too_long - 1] = '\0';
    send_body("POST /api/notes", too_long, &store, &res);
    assert(res.status == 400);
    response_free(&res);

    send_request("PATCH /api/notes HTTP/1.1\r\n\r\n", &store, &res);
    assert(res.status == 405);
    response_free(&res);
}

static void test_notes_api_store_full_is_507(void) {
    NoteStore store;
    Response res;
    store_init(&store);
    for (int i = 0; i < MAX_NOTES; i++) {
        store_add(&store, "x");
    }
    send_body("POST /api/notes", "one more", &store, &res);
    assert(res.status == 507);
    response_free(&res);
}

static void test_text_is_escaped_in_json(void) {
    NoteStore store;
    Response res;
    store_init(&store);
    /* A quote, a backslash and a newline inside the note are escaped in the JSON answer. */
    send_body("POST /api/notes", "a\"b\\c\nd", &store, &res);
    assert(res.status == 201);
    assert(strcmp(res.body.data, "{\"id\":1,\"text\":\"a\\\"b\\\\c\\u000ad\"}") == 0);
    response_free(&res);
}

static void test_response_format(void) {
    Response res;
    Buffer out;
    buffer_init(&out);
    response_init(&res);
    response_error(&res, 404, "not found");
    response_format(&res, &out);
    assert(strstr(out.data, "HTTP/1.1 404 Not Found\r\n") == out.data);
    assert(strstr(out.data, "Content-Length: 21\r\n") != NULL);
    assert(strstr(out.data, "\r\n\r\n{\"error\":\"not found\"}") != NULL);
    response_free(&res);
    buffer_free(&out);

    buffer_init(&out);
    response_init(&res);
    res.status = 204;
    response_format(&res, &out);
    assert(strcmp(out.data, "HTTP/1.1 204 No Content\r\nConnection: close\r\n\r\n") == 0);
    response_free(&res);
    buffer_free(&out);
}

int main(void) {
    test_parse_request_line_and_headers();
    test_parse_waits_for_more_bytes();
    test_parse_rejects_bad_requests();
    test_mime_types();
    test_safe_path();
    test_store_add_update_delete();
    test_store_is_full_at_max_notes();
    test_static_files();
    test_notes_api_create_list_update_delete();
    test_notes_api_errors();
    test_notes_api_store_full_is_507();
    test_text_is_escaped_in_json();
    test_response_format();
    printf("All HTTP logic tests passed.\n");
    return 0;
}
