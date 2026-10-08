/* The user interface: a socket server on 127.0.0.1 that handles one connection at a time. */
#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#include "http_server.h"

#define DEFAULT_PORT 8000
#define READ_TIMEOUT_SECONDS 5

static int parse_port(const char *text, int *port) {
    char *end;
    long value = strtol(text, &end, 10);
    if (*text == '\0' || *end != '\0' || value < 0 || value > 65535) {
        return 0;
    }
    *port = (int)value;
    return 1;
}

/* Reads from the socket until the request is complete or the client goes away. */
static ParseResult read_request(int conn, Buffer *raw, Request *req) {
    char chunk[4096];
    for (;;) {
        ParseResult result = parse_request(raw->data, raw->len, req);
        if (result != REQUEST_INCOMPLETE) {
            return result;
        }
        ssize_t n = recv(conn, chunk, sizeof chunk, 0);
        if (n <= 0) {
            return REQUEST_INCOMPLETE;
        }
        buffer_append(raw, chunk, (size_t)n);
    }
}

static void send_all(int conn, const char *data, size_t len) {
    while (len > 0) {
        ssize_t n = send(conn, data, len, 0);
        if (n <= 0) {
            return;
        }
        data += n;
        len -= (size_t)n;
    }
}

static void serve(int conn, const char *public_dir, NoteStore *store) {
    Buffer raw, out;
    Request req;
    Response res;
    buffer_init(&raw);
    buffer_init(&out);
    response_init(&res);

    ParseResult result = read_request(conn, &raw, &req);
    if (result == REQUEST_MALFORMED) {
        response_error(&res, 400, "bad request");
        printf("400 bad request\n");
    } else if (result == REQUEST_COMPLETE) {
        handle_request(&req, public_dir, store, &res);
        printf("%s %s %d\n", req.method, req.path, res.status);
    }
    fflush(stdout);

    if (result != REQUEST_INCOMPLETE) {
        response_format(&res, &out);
        send_all(conn, out.data, out.len);
    }
    buffer_free(&raw);
    buffer_free(&out);
    response_free(&res);
}

int main(int argc, char **argv) {
    int port = DEFAULT_PORT;
    const char *public_dir = "public";
    if (argc > 1 && !parse_port(argv[1], &port)) {
        fprintf(stderr, "Invalid port: %s\n", argv[1]);
        return 1;
    }
    if (argc > 2) {
        public_dir = argv[2];
    }

    signal(SIGPIPE, SIG_IGN); /* a client that disconnects must not kill the server */

    int server = socket(AF_INET, SOCK_STREAM, 0);
    if (server < 0) {
        perror("socket");
        return 1;
    }
    int yes = 1;
    setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);

    struct sockaddr_in address;
    memset(&address, 0, sizeof address);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    address.sin_port = htons((uint16_t)port);
    if (bind(server, (struct sockaddr *)&address, sizeof address) < 0) {
        perror("bind");
        return 1;
    }
    if (listen(server, 16) < 0) {
        perror("listen");
        return 1;
    }
    printf("Serving %s on http://127.0.0.1:%d/\n", public_dir, port);
    fflush(stdout);

    NoteStore store;
    store_init(&store);
    for (;;) {
        int conn = accept(server, NULL, NULL);
        if (conn < 0) {
            perror("accept");
            continue;
        }
        struct timeval timeout = {READ_TIMEOUT_SECONDS, 0};
        setsockopt(conn, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout);
        serve(conn, public_dir, &store);
        close(conn);
    }
}
