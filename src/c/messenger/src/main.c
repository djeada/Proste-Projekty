/* Sockets and terminal: runs the chat server or a chat client. */
/* getaddrinfo() needs the POSIX feature macro when compiling as strict C99. */
// NOLINTNEXTLINE(bugprone-reserved-identifier)
#define _POSIX_C_SOURCE 200809L

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "messenger.h"

typedef struct {
    int fd; /* -1 when the slot is free */
    char buf[MAX_LINE];
    size_t len;
} Peer;

typedef struct {
    Peer peers[MAX_CLIENTS];
    Roster roster;
} Server;

static void send_line(int fd, const char *text) {
    char out[MAX_LINE + 2];
    snprintf(out, sizeof out, "%s\n", text);
    size_t left = strlen(out);
    const char *p = out;
    while (left > 0) {
        ssize_t sent = send(fd, p, left, 0);
        if (sent < 0) {
            if (errno == EINTR) {
                continue;
            }
            return;
        }
        p += sent;
        left -= (size_t)sent;
    }
}

/* Sends line to every client except the one in slot except (use -1 to send to all). */
static void broadcast(Server *server, const char *line, int except) {
    puts(line);
    fflush(stdout);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (i != except && server->peers[i].fd >= 0) {
            send_line(server->peers[i].fd, line);
        }
    }
}

static void drop_peer(Server *server, int slot) {
    Peer *peer = &server->peers[slot];
    const char *nick = roster_get(&server->roster, slot);
    if (nick[0] != '\0') {
        char text[MAX_LINE];
        format_leave(text, sizeof text, nick);
        roster_set(&server->roster, slot, "");
        broadcast(server, text, -1);
    }
    close(peer->fd);
    peer->fd = -1;
    peer->len = 0;
}

static void change_nick(Server *server, int slot, const char *nick) {
    Peer *peer = &server->peers[slot];
    const char *old = roster_get(&server->roster, slot);
    char text[MAX_LINE];

    if (!valid_nick(nick)) {
        send_line(peer->fd, "* Invalid nickname: use 1-16 letters, digits, '_' or '-'");
        return;
    }
    if (roster_has_nick(&server->roster, nick)) {
        snprintf(text, sizeof text, "* The nickname %s is taken", nick);
        send_line(peer->fd, text);
        return;
    }
    if (old[0] != '\0') {
        format_rename(text, sizeof text, old, nick);
    } else {
        format_join(text, sizeof text, nick);
    }
    roster_set(&server->roster, slot, nick);
    broadcast(server, text, -1);
}

static void handle_line(Server *server, int slot, const char *line) {
    char arg[MAX_LINE];
    char text[MAX_LINE];
    const char *nick = roster_get(&server->roster, slot);

    switch (parse_line(line, arg, sizeof arg)) {
        case LINE_EMPTY:
            break;
        case LINE_CHAT:
            if (nick[0] == '\0') {
                send_line(server->peers[slot].fd, "* Set a nickname first: /nick <name>");
            } else {
                format_chat(text, sizeof text, nick, arg);
                broadcast(server, text, slot);
            }
            break;
        case LINE_NICK:
            change_nick(server, slot, arg);
            break;
        case LINE_LIST:
            format_list(&server->roster, text, sizeof text);
            send_line(server->peers[slot].fd, text);
            break;
        case LINE_QUIT:
            drop_peer(server, slot);
            break;
        case LINE_UNKNOWN:
            send_line(server->peers[slot].fd, "* Unknown command (try /nick, /list, /quit)");
            break;
    }
}

static void read_peer(Server *server, int slot) {
    Peer *peer = &server->peers[slot];
    char line[MAX_LINE + 1];
    ssize_t got = recv(peer->fd, peer->buf + peer->len, MAX_LINE - peer->len, 0);
    if (got <= 0) {
        drop_peer(server, slot);
        return;
    }
    peer->len += (size_t)got;
    while (peer->fd >= 0 && next_line(peer->buf, &peer->len, line)) {
        handle_line(server, slot, line);
    }
}

static void accept_peer(Server *server, int listen_fd) {
    int fd = accept(listen_fd, NULL, NULL);
    if (fd < 0) {
        return;
    }
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (server->peers[i].fd < 0) {
            server->peers[i].fd = fd;
            server->peers[i].len = 0;
            return;
        }
    }
    send_line(fd, "* The server is full");
    close(fd);
}

static int listen_on(int port) {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        return -1;
    }
    int yes = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons((unsigned short)port);
    if (bind(fd, (struct sockaddr *)&addr, sizeof addr) < 0 || listen(fd, 8) < 0) {
        close(fd);
        return -1;
    }
    return fd;
}

static int connect_to(const char *host, int port) {
    struct addrinfo hints;
    struct addrinfo *result = NULL;
    char port_text[8];
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    snprintf(port_text, sizeof port_text, "%d", port);
    if (getaddrinfo(host, port_text, &hints, &result) != 0) {
        return -1;
    }
    int fd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (fd < 0 || connect(fd, result->ai_addr, result->ai_addrlen) < 0) {
        if (fd >= 0) {
            close(fd);
        }
        fd = -1;
    }
    freeaddrinfo(result);
    return fd;
}

static int run_server(int port) {
    int listen_fd = listen_on(port);
    if (listen_fd < 0) {
        perror("Cannot listen");
        return 1;
    }
    printf("Server listening on port %d\n", port);
    fflush(stdout);

    Server server;
    roster_init(&server.roster);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        server.peers[i].fd = -1;
        server.peers[i].len = 0;
    }

    struct pollfd fds[MAX_CLIENTS + 1];
    for (;;) {
        fds[0].fd = listen_fd;
        fds[0].events = POLLIN;
        for (int i = 0; i < MAX_CLIENTS; i++) {
            fds[i + 1].fd = server.peers[i].fd; /* a negative fd is ignored by poll() */
            fds[i + 1].events = POLLIN;
        }
        if (poll(fds, MAX_CLIENTS + 1, -1) < 0) {
            if (errno == EINTR) {
                continue;
            }
            perror("poll");
            return 1;
        }
        if (fds[0].revents & POLLIN) {
            accept_peer(&server, listen_fd);
        }
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (fds[i + 1].revents != 0 && server.peers[i].fd >= 0) {
                read_peer(&server, i);
            }
        }
    }
}

static int run_client(const char *host, int port, const char *nick) {
    int fd = connect_to(host, port);
    if (fd < 0) {
        fprintf(stderr, "Cannot connect to %s:%d\n", host, port);
        return 1;
    }
    char first[MAX_LINE];
    snprintf(first, sizeof first, "/nick %s", nick);
    send_line(fd, first);

    char in_buf[MAX_LINE];
    char net_buf[MAX_LINE];
    size_t in_len = 0;
    size_t net_len = 0;
    char line[MAX_LINE + 1];
    struct pollfd fds[2] = {{0, POLLIN, 0}, {fd, POLLIN, 0}};

    for (;;) {
        if (poll(fds, 2, -1) < 0) {
            if (errno == EINTR) {
                continue;
            }
            break;
        }

        if (fds[1].revents != 0) {
            ssize_t got = recv(fd, net_buf + net_len, MAX_LINE - net_len, 0);
            if (got <= 0) {
                puts("* Disconnected from the server");
                break;
            }
            net_len += (size_t)got;
            while (next_line(net_buf, &net_len, line)) {
                puts(line);
            }
            fflush(stdout);
        }

        if (fds[0].revents != 0) {
            ssize_t got = read(0, in_buf + in_len, MAX_LINE - in_len);
            if (got <= 0) {
                send_line(fd, "/quit"); /* end of input, like typing /quit */
                break;
            }
            in_len += (size_t)got;
            while (next_line(in_buf, &in_len, line)) {
                send_line(fd, line);
            }
        }
    }
    close(fd);
    return 0;
}

static void usage(void) {
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "  messenger server [port]\n");
    fprintf(stderr, "  messenger client <host> [port] <nick>\n");
}

int main(int argc, char **argv) {
    /* A write to a closed connection must give an error, not kill the program. */
    signal(SIGPIPE, SIG_IGN);

    if (argc == 2 && strcmp(argv[1], "server") == 0) {
        return run_server(DEFAULT_PORT);
    }
    if (argc == 3 && strcmp(argv[1], "server") == 0) {
        int port = parse_port(argv[2]);
        if (port < 0) {
            usage();
            return 1;
        }
        return run_server(port);
    }
    if (argc == 4 && strcmp(argv[1], "client") == 0) {
        if (!valid_nick(argv[3])) {
            fprintf(stderr, "Invalid nickname: %s\n", argv[3]);
            return 1;
        }
        return run_client(argv[2], DEFAULT_PORT, argv[3]);
    }
    if (argc == 5 && strcmp(argv[1], "client") == 0) {
        int port = parse_port(argv[3]);
        if (port < 0 || !valid_nick(argv[4])) {
            usage();
            return 1;
        }
        return run_client(argv[2], port, argv[4]);
    }
    usage();
    return 1;
}
