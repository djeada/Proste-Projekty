import unittest
import sys
import os

sys.path.insert(
    0, os.path.abspath(os.path.join(os.path.dirname(__file__), "../src/logic"))
)
from messenger import (
    MessengerServer,
    MessengerClient,
    Connection,
    create_socket,
    DEFAULT_PORT,
    MAX_MESSAGE_LENGTH,
    MAX_USERNAME_LENGTH,
)


class TestMessengerConstants(unittest.TestCase):
    def test_default_port(self):
        self.assertEqual(DEFAULT_PORT, 8888)

    def test_max_message_length(self):
        self.assertEqual(MAX_MESSAGE_LENGTH, 1024)

    def test_max_username_length(self):
        self.assertEqual(MAX_USERNAME_LENGTH, 32)


class TestMessengerServer(unittest.TestCase):
    def test_server_initialization(self):
        server = MessengerServer()
        self.assertEqual(server.port, DEFAULT_PORT)
        self.assertFalse(server.running)
        self.assertEqual(len(server.clients), 0)

    def test_server_custom_port(self):
        server = MessengerServer(port=9999)
        self.assertEqual(server.port, 9999)


class TestMessengerClient(unittest.TestCase):
    def test_client_initialization(self):
        client = MessengerClient()
        self.assertEqual(client.host, "localhost")
        self.assertEqual(client.port, DEFAULT_PORT)
        self.assertFalse(client.connected)

    def test_client_custom_host_port(self):
        client = MessengerClient(host="example.com", port=9999)
        self.assertEqual(client.host, "example.com")
        self.assertEqual(client.port, 9999)

    def test_set_username(self):
        client = MessengerClient()
        client.set_username("testuser")
        self.assertEqual(client.username, "testuser")

    def test_username_truncation(self):
        client = MessengerClient()
        long_name = "a" * 50
        client.set_username(long_name)
        self.assertEqual(len(client.username), MAX_USERNAME_LENGTH)


class TestCreateSocket(unittest.TestCase):
    def test_create_socket(self):
        sock = create_socket()
        self.assertIsNotNone(sock)
        sock.close()


if __name__ == "__main__":
    unittest.main()
