"""
Simple HTTP server logic.
"""
import socket
from typing import Tuple, Optional


DEFAULT_PORT = 8080
RESPONSE_TEMPLATE = """HTTP/1.1 {status}
Content-Type: {content_type}
Content-Length: {content_length}

{body}"""


class HttpServer:
    """
    Simple HTTP server.
    """

    def __init__(self, port: int = DEFAULT_PORT) -> None:
        self.port = port
        self.socket: Optional[socket.socket] = None
        self.running = False

    def create_response(
        self,
        body: str,
        status: str = "200 OK",
        content_type: str = "text/plain",
    ) -> bytes:
        """
        Create an HTTP response.

        :param body: Response body
        :param status: HTTP status code and message
        :param content_type: Content-Type header value
        :return: Encoded response bytes
        """
        response = RESPONSE_TEMPLATE.format(
            status=status,
            content_type=content_type,
            content_length=len(body),
            body=body,
        )
        return response.encode("utf-8")

    def handle_request(self, request: str) -> bytes:
        """
        Handle an HTTP request.

        :param request: The request string
        :return: Response bytes
        """
        # Parse the request line
        lines = request.split("\r\n")
        if not lines:
            return self.create_response("Bad Request", status="400 Bad Request")

        request_line = lines[0]
        parts = request_line.split(" ")

        if len(parts) < 2:
            return self.create_response("Bad Request", status="400 Bad Request")

        method = parts[0]
        path = parts[1]

        # Simple routing
        if method == "GET":
            if path == "/" or path == "/index.html":
                return self.create_response(
                    "Hello, world!",
                    content_type="text/plain",
                )
            elif path == "/health":
                return self.create_response(
                    '{"status": "ok"}',
                    content_type="application/json",
                )
            else:
                return self.create_response("Not Found", status="404 Not Found")
        else:
            return self.create_response(
                "Method Not Allowed", status="405 Method Not Allowed"
            )

    def start(self) -> None:
        """Start the server."""
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        # Bind to all interfaces (empty string) - suitable for demo/local use
        # For production, specify a specific interface like "127.0.0.1"
        self.socket.bind(("", self.port))  # nosec
        self.socket.listen(5)
        self.running = True

        print(f"Server listening on port {self.port}...")

        try:
            while self.running:
                client_socket, address = self.socket.accept()
                try:
                    request = client_socket.recv(1024).decode("utf-8")
                    if request:
                        response = self.handle_request(request)
                        client_socket.send(response)
                finally:
                    client_socket.close()
        except KeyboardInterrupt:
            print("\nServer stopped.")
        finally:
            self.stop()

    def stop(self) -> None:
        """Stop the server."""
        self.running = False
        if self.socket:
            self.socket.close()
            self.socket = None


def parse_request_line(line: str) -> Tuple[str, str, str]:
    """
    Parse an HTTP request line.

    :param line: The request line
    :return: Tuple of (method, path, version)
    """
    parts = line.split(" ")
    if len(parts) >= 3:
        return (parts[0], parts[1], parts[2])
    elif len(parts) == 2:
        return (parts[0], parts[1], "HTTP/1.1")
    else:
        return ("", "", "")
