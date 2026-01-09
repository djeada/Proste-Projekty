#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../src/messenger.h"

void test_create_socket() {
    int socket_fd = create_socket();
    assert(socket_fd >= 0);
    close_connection(socket_fd);
}

void test_set_socket_reusable() {
    int socket_fd = create_socket();
    assert(socket_fd >= 0);
    int result = set_socket_reusable(socket_fd);
    assert(result == 0);
    close_connection(socket_fd);
}

void test_bind_socket() {
    int socket_fd = create_socket();
    assert(socket_fd >= 0);
    set_socket_reusable(socket_fd);
    // Use a high port to avoid conflicts
    int result = bind_socket(socket_fd, 59999);
    assert(result == 0);
    close_connection(socket_fd);
}

void test_listen_socket() {
    int socket_fd = create_socket();
    assert(socket_fd >= 0);
    set_socket_reusable(socket_fd);
    bind_socket(socket_fd, 59998);
    int result = listen_socket(socket_fd, 5);
    assert(result == 0);
    close_connection(socket_fd);
}

void test_start_stop_server() {
    int server_fd = start_server(59997);
    assert(server_fd >= 0);
    stop_server(server_fd);
}

int main() {
    test_create_socket();
    test_set_socket_reusable();
    test_bind_socket();
    test_listen_socket();
    test_start_stop_server();
    printf("All tests passed!\n");
    return 0;
}
