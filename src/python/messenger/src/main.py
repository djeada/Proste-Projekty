"""
Python implementation of a simple messenger client/server.
"""
import sys
import os

sys.path.insert(
    0, os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
)

from src.python.messenger.src.logic.messenger import (
    MessengerServer,
    MessengerClient,
    DEFAULT_PORT,
)


def run_server(port: int = DEFAULT_PORT) -> None:
    """Run the messenger server."""
    server = MessengerServer(port)
    server.start()


def run_client(host: str = "localhost", port: int = DEFAULT_PORT) -> None:
    """Run the messenger client."""
    client = MessengerClient(host, port)

    if not client.connect():
        print("Failed to connect to server.")
        return

    print(f"Connected to {host}:{port}")
    print("Enter your username:")

    try:
        username = input().strip()
        client.set_username(username)
        print(f"Welcome, {username}! Type messages to send. Ctrl+C to exit.")

        # Start receiving messages
        def on_message(msg: str):
            print(f"\r{msg}")
            print(f"{username}: ", end="", flush=True)

        client.start_receiving(on_message)

        # Send messages
        while True:
            try:
                message = input(f"{username}: ")
                if message:
                    client.send_message(message)
            except EOFError:
                break
    except KeyboardInterrupt:
        print("\nDisconnecting...")
    finally:
        client.disconnect()


def main() -> None:
    if len(sys.argv) < 2:
        print("Usage: python main.py <server|client> [host] [port]")
        print("  server [port]      - Start the server")
        print("  client [host port] - Connect to server")
        sys.exit(1)

    mode = sys.argv[1].lower()

    if mode == "server":
        port = int(sys.argv[2]) if len(sys.argv) > 2 else DEFAULT_PORT
        run_server(port)
    elif mode == "client":
        host = sys.argv[2] if len(sys.argv) > 2 else "localhost"
        port = int(sys.argv[3]) if len(sys.argv) > 3 else DEFAULT_PORT
        run_client(host, port)
    else:
        print(f"Unknown mode: {mode}")
        print("Use 'server' or 'client'")
        sys.exit(1)


if __name__ == "__main__":
    main()
