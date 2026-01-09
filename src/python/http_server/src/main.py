"""
Python implementation of a simple HTTP server.
"""
import sys
import os

sys.path.insert(
    0, os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
)

from src.python.http_server.src.logic.server import HttpServer, DEFAULT_PORT


def main() -> None:
    port = DEFAULT_PORT

    # Check for port argument
    if len(sys.argv) > 1:
        try:
            port = int(sys.argv[1])
        except ValueError:
            print(f"Invalid port: {sys.argv[1]}")
            sys.exit(1)

    server = HttpServer(port)
    server.start()


if __name__ == "__main__":
    main()
