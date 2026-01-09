#ifndef MESSENGER_H
#define MESSENGER_H

#include <netinet/in.h>

#define MAX_MESSAGE_LENGTH 1024
#define MAX_USERNAME_LENGTH 32
#define DEFAULT_PORT 8888
#define MAX_CLIENTS 10

typedef struct {
    int socket_fd;
    struct sockaddr_in address;
    int port;
    int is_connected;
    char username[MAX_USERNAME_LENGTH];
} Connection;

// Utility functions
int create_socket(void);
int set_socket_reusable(int socket_fd);
int bind_socket(int socket_fd, int port);
int listen_socket(int socket_fd, int backlog);
int accept_connection(int socket_fd, struct sockaddr_in *client_addr);
int connect_to_server(int socket_fd, const char *host, int port);
int send_message(int socket_fd, const char *message);
int receive_message(int socket_fd, char *buffer, int buffer_size);
void close_connection(int socket_fd);

// Server functions
int start_server(int port);
void stop_server(int socket_fd);

// Client functions
int start_client(const char *host, int port);

#endif // MESSENGER_H
