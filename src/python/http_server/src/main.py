"""Serves files from public/ and a notes API on http://127.0.0.1:8000/. Usage: main.py [port] [public_dir]"""
import socket
import sys
from pathlib import Path
from typing import Optional

from http_server import NotesStore, ParseError, Request, error_response, handle, parse_request

DEFAULT_PORT = 8000
TIMEOUT_SECONDS = 5.0


def read_request(conn: socket.socket) -> Optional[Request]:
    """Read from the socket until the request is complete. Return None if the client leaves early."""
    raw = b""
    while True:
        request = parse_request(raw)  # raises ParseError for a malformed request
        if request is not None:
            return request
        chunk = conn.recv(4096)
        if not chunk:
            return None
        raw += chunk


def serve(conn: socket.socket, public_dir: Path, store: NotesStore) -> None:
    conn.settimeout(TIMEOUT_SECONDS)
    try:
        request = read_request(conn)
    except ParseError as err:
        response = error_response(400, str(err))
        print(f"400 {err}", flush=True)
    else:
        if request is None:
            return
        response = handle(request, public_dir, store)
        print(f"{request.method} {request.path} {response.status}", flush=True)
    conn.sendall(response.to_bytes())


def main() -> None:
    port = DEFAULT_PORT
    public_dir = Path("public")
    if len(sys.argv) > 1:
        try:
            port = int(sys.argv[1])
        except ValueError:
            port = -1
        if not 0 <= port <= 65535:
            sys.exit(f"Invalid port: {sys.argv[1]}")
    if len(sys.argv) > 2:
        public_dir = Path(sys.argv[2])

    store = NotesStore()
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server:
        server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        server.bind(("127.0.0.1", port))
        server.listen()
        print(f"Serving {public_dir} on http://127.0.0.1:{server.getsockname()[1]}/", flush=True)
        try:
            while True:
                conn, _address = server.accept()
                with conn:
                    try:
                        serve(conn, public_dir, store)
                    except OSError as err:  # a client that is slow or disconnects
                        print(f"connection error: {err}", flush=True)
        except KeyboardInterrupt:
            print("\nServer stopped.")


if __name__ == "__main__":
    main()
