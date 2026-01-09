"""
Simple messenger client/server logic.
"""
import socket
import threading
from typing import Optional, List, Callable
from dataclasses import dataclass


DEFAULT_PORT = 8888
MAX_MESSAGE_LENGTH = 1024
MAX_USERNAME_LENGTH = 32


@dataclass
class Connection:
    """Represents a network connection."""

    socket_fd: socket.socket
    address: tuple
    port: int
    is_connected: bool
    username: str


def create_socket() -> socket.socket:
    """Create a TCP socket."""
    return socket.socket(socket.AF_INET, socket.SOCK_STREAM)


def set_socket_reusable(sock: socket.socket) -> None:
    """Set socket to be reusable."""
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)


class MessengerServer:
    """
    Simple messenger server.
    """

    def __init__(self, port: int = DEFAULT_PORT) -> None:
        self.port = port
        self.socket: Optional[socket.socket] = None
        self.clients: List[Connection] = []
        self.running = False

    def start(self) -> None:
        """Start the server."""
        self.socket = create_socket()
        set_socket_reusable(self.socket)
        self.socket.bind(("", self.port))
        self.socket.listen(10)
        self.running = True

        print(f"Server started on port {self.port}")

        try:
            while self.running:
                try:
                    self.socket.settimeout(1.0)
                    client_socket, address = self.socket.accept()
                    conn = Connection(
                        socket_fd=client_socket,
                        address=address,
                        port=address[1],
                        is_connected=True,
                        username="",
                    )
                    self.clients.append(conn)
                    print(f"Client connected from {address}")

                    thread = threading.Thread(
                        target=self.handle_client, args=(conn,)
                    )
                    thread.daemon = True
                    thread.start()
                except socket.timeout:
                    continue
        except KeyboardInterrupt:
            print("\nServer stopped.")
        finally:
            self.stop()

    def handle_client(self, conn: Connection) -> None:
        """Handle a connected client."""
        try:
            while conn.is_connected and self.running:
                try:
                    data = conn.socket_fd.recv(MAX_MESSAGE_LENGTH)
                    if not data:
                        break

                    message = data.decode("utf-8")

                    # Handle username setting
                    if message.startswith("/name "):
                        conn.username = message[6:].strip()[:MAX_USERNAME_LENGTH]
                        continue

                    # Broadcast message to all other clients
                    sender = conn.username or f"Client-{conn.port}"
                    broadcast_msg = f"{sender}: {message}"
                    self.broadcast(broadcast_msg, exclude=conn)

                except socket.error:
                    break
        finally:
            conn.is_connected = False
            if conn in self.clients:
                self.clients.remove(conn)
            conn.socket_fd.close()
            print(f"Client {conn.address} disconnected")

    def broadcast(self, message: str, exclude: Optional[Connection] = None) -> None:
        """Broadcast a message to all connected clients."""
        for client in self.clients:
            if client != exclude and client.is_connected:
                try:
                    client.socket_fd.send(message.encode("utf-8"))
                except socket.error:
                    client.is_connected = False

    def stop(self) -> None:
        """Stop the server."""
        self.running = False
        for client in self.clients:
            try:
                client.socket_fd.close()
            except socket.error:
                pass
        if self.socket:
            self.socket.close()


class MessengerClient:
    """
    Simple messenger client.
    """

    def __init__(self, host: str = "localhost", port: int = DEFAULT_PORT) -> None:
        self.host = host
        self.port = port
        self.socket: Optional[socket.socket] = None
        self.connected = False
        self.username = ""

    def connect(self) -> bool:
        """Connect to the server."""
        try:
            self.socket = create_socket()
            self.socket.connect((self.host, self.port))
            self.connected = True
            return True
        except socket.error as e:
            print(f"Connection failed: {e}")
            return False

    def set_username(self, username: str) -> None:
        """Set the username."""
        self.username = username[:MAX_USERNAME_LENGTH]
        if self.connected and self.socket:
            self.send_message(f"/name {self.username}")

    def send_message(self, message: str) -> bool:
        """Send a message to the server."""
        if not self.connected or not self.socket:
            return False
        try:
            self.socket.send(message[:MAX_MESSAGE_LENGTH].encode("utf-8"))
            return True
        except socket.error:
            self.connected = False
            return False

    def receive_message(self) -> Optional[str]:
        """Receive a message from the server."""
        if not self.connected or not self.socket:
            return None
        try:
            data = self.socket.recv(MAX_MESSAGE_LENGTH)
            if data:
                return data.decode("utf-8")
            return None
        except socket.error:
            self.connected = False
            return None

    def start_receiving(self, callback: Callable[[str], None]) -> None:
        """Start receiving messages in a separate thread."""
        def receive_loop():
            while self.connected:
                message = self.receive_message()
                if message:
                    callback(message)

        thread = threading.Thread(target=receive_loop)
        thread.daemon = True
        thread.start()

    def disconnect(self) -> None:
        """Disconnect from the server."""
        self.connected = False
        if self.socket:
            self.socket.close()
