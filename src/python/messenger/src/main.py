"""Terminal interface and sockets: runs the chat server or a chat client."""
import codecs
import selectors
import socket
import sys
import threading

from messenger import (DEFAULT_PORT, MAX_CLIENTS, MAX_LINE, format_chat, format_join, format_leave,
                       format_list, format_rename, parse_line, parse_port, split_lines, valid_nick)


class LineReader:
    """Turns received bytes into complete lines of text."""

    def __init__(self):
        self.decoder = codecs.getincrementaldecoder('utf-8')(errors='replace')
        self.text = ''

    def feed(self, data):
        self.text += self.decoder.decode(data)
        lines, self.text = split_lines(self.text)
        return lines


def send_line(sock, text):
    try:
        sock.sendall((text + '\n').encode('utf-8'))
    except OSError:
        pass  # the connection is gone; the reader notices it on its next read


class Server:
    def __init__(self):
        self.readers = {}  # socket -> LineReader
        self.nicks = {}  # socket -> nickname, only for users who have chosen one
        self.selector = selectors.DefaultSelector()

    def serve(self, listener):
        self.selector.register(listener, selectors.EVENT_READ)
        while True:
            for key, _ in self.selector.select():
                if key.fileobj is listener:
                    self.accept(listener)
                else:
                    self.read(key.fileobj)

    def accept(self, listener):
        conn, _ = listener.accept()
        if len(self.readers) >= MAX_CLIENTS:
            send_line(conn, '* The server is full')
            conn.close()
            return
        self.readers[conn] = LineReader()
        self.selector.register(conn, selectors.EVENT_READ)

    def read(self, sock):
        try:
            data = sock.recv(MAX_LINE)
        except OSError:
            data = b''
        if not data:
            self.drop(sock)
            return
        for line in self.readers[sock].feed(data):
            if not self.handle_line(sock, line):
                self.drop(sock)
                return

    def handle_line(self, sock, line):
        """Returns False when the user has quit."""
        kind, arg = parse_line(line)
        if kind == 'chat':
            if sock in self.nicks:
                self.broadcast(format_chat(self.nicks[sock], arg), sender=sock)
            else:
                send_line(sock, '* Set a nickname first: /nick <name>')
        elif kind == 'nick':
            self.change_nick(sock, arg)
        elif kind == 'list':
            send_line(sock, format_list(self.nicks.values()))
        elif kind == 'unknown':
            send_line(sock, '* Unknown command (try /nick, /list, /quit)')
        return kind != 'quit'

    def change_nick(self, sock, nick):
        if not valid_nick(nick):
            send_line(sock, "* Invalid nickname: use 1-16 letters, digits, '_' or '-'")
        elif nick in self.nicks.values():
            send_line(sock, f'* The nickname {nick} is taken')
        elif sock in self.nicks:
            self.broadcast(format_rename(self.nicks[sock], nick))
            self.nicks[sock] = nick
        else:
            self.broadcast(format_join(nick))
            self.nicks[sock] = nick

    def broadcast(self, line, sender=None):
        """Sends line to every client except sender."""
        print(line, flush=True)
        for sock in self.readers:
            if sock is not sender:
                send_line(sock, line)

    def drop(self, sock):
        if sock in self.nicks:
            self.broadcast(format_leave(self.nicks.pop(sock)))
        self.selector.unregister(sock)
        del self.readers[sock]
        sock.close()


def run_server(port):
    listener = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    listener.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    try:
        listener.bind(('', port))
        listener.listen(8)
    except OSError as error:
        print(f'Cannot listen on port {port}: {error}', file=sys.stderr)
        return 1
    print(f'Server listening on port {port}', flush=True)
    Server().serve(listener)


def send_typed_lines(sock):
    """Sends every line typed by the user; end of input means /quit."""
    for line in sys.stdin:
        send_line(sock, line.rstrip('\r\n'))
    send_line(sock, '/quit')


def run_client(host, port, nick):
    try:
        sock = socket.create_connection((host, port))
    except OSError as error:
        print(f'Cannot connect to {host}:{port}: {error}', file=sys.stderr)
        return 1
    send_line(sock, f'/nick {nick}')
    threading.Thread(target=send_typed_lines, args=(sock,), daemon=True).start()

    incoming = LineReader()
    while True:
        data = sock.recv(4096)
        if not data:
            print('* Disconnected from the server', flush=True)
            return 0
        for line in incoming.feed(data):
            print(line, flush=True)


USAGE = ('Usage:\n'
         '  messenger server [port]\n'
         '  messenger client <host> [port] <nick>')


def main(argv):
    args = argv[1:]
    if len(args) in (1, 2) and args[0] == 'server':
        port = parse_port(args[1]) if len(args) == 2 else DEFAULT_PORT
        if port is not None:
            return run_server(port)
    elif len(args) in (3, 4) and args[0] == 'client':
        host, nick = args[1], args[-1]
        port = parse_port(args[2]) if len(args) == 4 else DEFAULT_PORT
        if port is not None and valid_nick(nick):
            return run_client(host, port, nick)
    print(USAGE, file=sys.stderr)
    return 1


if __name__ == '__main__':
    sys.exit(main(sys.argv))
