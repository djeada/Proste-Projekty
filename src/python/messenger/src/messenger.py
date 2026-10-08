"""The chat protocol and the rules of the server: no sockets, no printing."""
import re

MAX_LINE = 512  # longest line, without the newline
NICK_MAX = 16
MAX_CLIENTS = 16
DEFAULT_PORT = 8888

COMMANDS = {'/nick': 'nick', '/list': 'list', '/quit': 'quit'}


def parse_line(line):
    """Returns (kind, arg). The kind is 'empty', 'chat', 'nick', 'list', 'quit' or 'unknown'."""
    if not line.startswith('/'):
        text = line.strip()
        return ('chat', text) if text else ('empty', '')
    command, _, arg = line.partition(' ')
    return COMMANDS.get(command, 'unknown'), arg.strip()


def valid_nick(nick):
    return re.fullmatch(r'[A-Za-z0-9_-]{1,%d}' % NICK_MAX, nick) is not None


def parse_port(text):
    """Returns the port number, or None if text is not a number from 1 to 65535."""
    if text.isdigit() and 1 <= int(text) <= 65535:
        return int(text)
    return None


def split_lines(text):
    """Returns (complete lines, unfinished rest). A rest of MAX_LINE characters counts as a line."""
    *lines, rest = text.split('\n')
    lines = [line.rstrip('\r') for line in lines]
    while len(rest) >= MAX_LINE:
        lines.append(rest[:MAX_LINE])
        rest = rest[MAX_LINE:]
    return lines, rest


def format_chat(nick, text):
    return f'<{nick}> {text}'[:MAX_LINE - 1]


def format_join(nick):
    return f'* {nick} joined'


def format_leave(nick):
    return f'* {nick} left'


def format_rename(old_nick, new_nick):
    return f'* {old_nick} is now known as {new_nick}'


def format_list(nicks):
    """nicks: the nicknames of the users who are online."""
    nicks = list(nicks)
    if not nicks:
        return '* Nobody is online'
    return '* Online: ' + ', '.join(nicks)
