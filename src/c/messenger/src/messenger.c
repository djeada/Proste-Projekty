#include "messenger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int create_socket(void) {
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        perror("Socket creation failed");
        return -1;
    }
    return socket_fd;
}

int set_socket_reusable(int socket_fd) {
    int opt = 1;
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
        return -1;
    }
    return 0;
}

int bind_socket(int socket_fd, int port) {
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons((uint16_t)port);

    if (bind(socket_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        return -1;
    }
    return 0;
}

int listen_socket(int socket_fd, int backlog) {
    if (listen(socket_fd, backlog) < 0) {
        perror("Listen failed");
        return -1;
    }
    return 0;
}

int accept_connection(int socket_fd, struct sockaddr_in *client_addr) {
    socklen_t addr_len = sizeof(*client_addr);
    int client_fd = accept(socket_fd, (struct sockaddr *)client_addr, &addr_len);
    if (client_fd < 0) {
        perror("Accept failed");
        return -1;
    }
    return client_fd;
}

int connect_to_server(int socket_fd, const char *host, int port) {
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons((uint16_t)port);

    if (inet_pton(AF_INET, host, &server_addr.sin_addr) <= 0) {
        perror("Invalid address");
        return -1;
    }

    if (connect(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        return -1;
    }
    return 0;
}

int send_message(int socket_fd, const char *message) {
    ssize_t bytes_sent = send(socket_fd, message, strlen(message), 0);
    if (bytes_sent < 0) {
        perror("Send failed");
        return -1;
    }
    return (int)bytes_sent;
}

int receive_message(int socket_fd, char *buffer, int buffer_size) {
    memset(buffer, 0, (size_t)buffer_size);
    ssize_t bytes_received = recv(socket_fd, buffer, (size_t)(buffer_size - 1), 0);
    if (bytes_received < 0) {
        perror("Receive failed");
        return -1;
    }
    return (int)bytes_received;
}

void close_connection(int socket_fd) {
    if (socket_fd >= 0) {
        close(socket_fd);
    }
}

int start_server(int port) {
    int server_fd = create_socket();
    if (server_fd < 0) return -1;

    if (set_socket_reusable(server_fd) < 0) {
        close_connection(server_fd);
        return -1;
    }

    if (bind_socket(server_fd, port) < 0) {
        close_connection(server_fd);
        return -1;
    }

    if (listen_socket(server_fd, MAX_CLIENTS) < 0) {
        close_connection(server_fd);
        return -1;
    }

    return server_fd;
}

void stop_server(int socket_fd) {
    close_connection(socket_fd);
}

int start_client(const char *host, int port) {
    int client_fd = create_socket();
    if (client_fd < 0) return -1;

    if (connect_to_server(client_fd, host, port) < 0) {
        close_connection(client_fd);
        return -1;
    }

    return client_fd;
}
