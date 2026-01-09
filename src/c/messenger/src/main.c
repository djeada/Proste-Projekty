#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <arpa/inet.h>
#include "messenger.h"

static volatile int running = 1;

void handle_signal(int sig) {
    (void)sig;
    running = 0;
}

void *receive_thread(void *arg) {
    int socket_fd = *(int *)arg;
    char buffer[MAX_MESSAGE_LENGTH];

    while (running) {
        int bytes = receive_message(socket_fd, buffer, sizeof(buffer));
        if (bytes <= 0) {
            if (running) {
                printf("\nConnection closed.\n");
            }
            running = 0;
            break;
        }
        printf("\n%s\n> ", buffer);
        fflush(stdout);
    }
    return NULL;
}

void run_client(const char *host, int port, const char *username) {
    printf("Connecting to %s:%d as %s...\n", host, port, username);

    int client_fd = start_client(host, port);
    if (client_fd < 0) {
        printf("Failed to connect to server.\n");
        return;
    }

    printf("Connected! Type messages and press Enter. Type 'quit' to exit.\n\n");

    // Send username
    char intro[MAX_MESSAGE_LENGTH];
    snprintf(intro, sizeof(intro), "[%s joined the chat]", username);
    send_message(client_fd, intro);

    pthread_t recv_thread;
    pthread_create(&recv_thread, NULL, receive_thread, &client_fd);

    char message[MAX_MESSAGE_LENGTH];
    char formatted[MAX_MESSAGE_LENGTH];

    while (running) {
        printf("> ");
        fflush(stdout);

        if (!fgets(message, sizeof(message), stdin)) {
            break;
        }

        message[strcspn(message, "\n")] = '\0';

        if (strlen(message) == 0) {
            continue;
        }

        if (strcmp(message, "quit") == 0) {
            snprintf(formatted, sizeof(formatted), "[%s left the chat]", username);
            send_message(client_fd, formatted);
            break;
        }

        snprintf(formatted, sizeof(formatted), "%s: %s", username, message);
        if (send_message(client_fd, formatted) < 0) {
            break;
        }
    }

    running = 0;
    close_connection(client_fd);
    pthread_join(recv_thread, NULL);
    printf("Disconnected.\n");
}

void run_server(int port) {
    printf("Starting server on port %d...\n", port);

    int server_fd = start_server(port);
    if (server_fd < 0) {
        printf("Failed to start server.\n");
        return;
    }

    printf("Server started. Waiting for connections...\n");
    printf("Press Ctrl+C to stop.\n\n");

    signal(SIGINT, handle_signal);

    int client_fds[MAX_CLIENTS];
    int client_count = 0;

    for (int i = 0; i < MAX_CLIENTS; i++) {
        client_fds[i] = -1;
    }

    while (running) {
        struct sockaddr_in client_addr;
        int client_fd = accept_connection(server_fd, &client_addr);

        if (client_fd < 0) {
            if (running) {
                continue;
            }
            break;
        }

        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
        printf("Client connected: %s\n", client_ip);

        // Add to client list
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (client_fds[i] < 0) {
                client_fds[i] = client_fd;
                client_count++;
                break;
            }
        }

        // Simple echo server - receive and broadcast
        char buffer[MAX_MESSAGE_LENGTH];
        while (running) {
            int bytes = receive_message(client_fd, buffer, sizeof(buffer));
            if (bytes <= 0) {
                printf("Client %s disconnected.\n", client_ip);
                break;
            }

            printf("[%s] %s\n", client_ip, buffer);

            // Broadcast to all connected clients
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (client_fds[i] >= 0 && client_fds[i] != client_fd) {
                    send_message(client_fds[i], buffer);
                }
            }
        }

        // Remove from client list
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (client_fds[i] == client_fd) {
                client_fds[i] = -1;
                client_count--;
                break;
            }
        }
        close_connection(client_fd);
    }

    // Close all client connections
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_fds[i] >= 0) {
            close_connection(client_fds[i]);
        }
    }

    stop_server(server_fd);
    printf("Server stopped.\n");
}

void print_usage(const char *program) {
    printf("Usage:\n");
    printf("  %s server [port]           - Start server (default port: %d)\n", program, DEFAULT_PORT);
    printf("  %s client <host> [port]    - Connect to server\n", program);
    printf("  %s client <host> [port] <username> - Connect with username\n", program);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "server") == 0) {
        int port = (argc > 2) ? atoi(argv[2]) : DEFAULT_PORT;
        run_server(port);
    } else if (strcmp(argv[1], "client") == 0) {
        if (argc < 3) {
            printf("Error: Server host required.\n");
            print_usage(argv[0]);
            return 1;
        }
        const char *host = argv[2];
        int port = (argc > 3) ? atoi(argv[3]) : DEFAULT_PORT;
        const char *username = (argc > 4) ? argv[4] : "User";
        run_client(host, port, username);
    } else {
        print_usage(argv[0]);
        return 1;
    }

    return 0;
}
