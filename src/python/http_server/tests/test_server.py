import unittest
import sys
import os

sys.path.insert(
    0, os.path.abspath(os.path.join(os.path.dirname(__file__), "../src/logic"))
)
from server import HttpServer, parse_request_line


class TestHttpServer(unittest.TestCase):
    def test_create_response(self):
        server = HttpServer()
        response = server.create_response("Hello")
        self.assertIn(b"200 OK", response)
        self.assertIn(b"Hello", response)
        self.assertIn(b"Content-Length: 5", response)

    def test_handle_get_root(self):
        server = HttpServer()
        request = "GET / HTTP/1.1\r\nHost: localhost\r\n\r\n"
        response = server.handle_request(request)
        self.assertIn(b"200 OK", response)
        self.assertIn(b"Hello, world!", response)

    def test_handle_get_health(self):
        server = HttpServer()
        request = "GET /health HTTP/1.1\r\nHost: localhost\r\n\r\n"
        response = server.handle_request(request)
        self.assertIn(b"200 OK", response)
        self.assertIn(b'"status": "ok"', response)

    def test_handle_get_not_found(self):
        server = HttpServer()
        request = "GET /nonexistent HTTP/1.1\r\nHost: localhost\r\n\r\n"
        response = server.handle_request(request)
        self.assertIn(b"404 Not Found", response)

    def test_handle_bad_request(self):
        server = HttpServer()
        request = "INVALID"
        response = server.handle_request(request)
        self.assertIn(b"400 Bad Request", response)


class TestParseRequestLine(unittest.TestCase):
    def test_parse_full_line(self):
        method, path, version = parse_request_line("GET /index.html HTTP/1.1")
        self.assertEqual(method, "GET")
        self.assertEqual(path, "/index.html")
        self.assertEqual(version, "HTTP/1.1")

    def test_parse_partial_line(self):
        method, path, version = parse_request_line("GET /")
        self.assertEqual(method, "GET")
        self.assertEqual(path, "/")
        self.assertEqual(version, "HTTP/1.1")


if __name__ == "__main__":
    unittest.main()
