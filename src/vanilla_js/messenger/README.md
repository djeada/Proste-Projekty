# Messenger (JavaScript)

A simple text chat for a local network (LAN), written for Node.js. One program runs as the **server**; any number of **clients** connect to it, pick a nickname and chat. Everything a client types is sent to the server, which passes it on to everybody.

The same chat is also written in [C](../../c/messenger) and [Python](../../python/messenger). All three versions speak the same line-based protocol, so a JavaScript server works with a C or Python client and the other way round.

![Screenshot](screenshot.png)

## Features

- One program with two modes: `server` and `client`
- The server accepts several clients at once (up to 16) and does not block while it waits for them
- Every chat line is relayed to all other connected clients
- Nicknames with `/nick <name>`, a list of online users with `/list`, leaving with `/quit`
- Join, leave and rename notices (`* Bob joined`)
- Duplicate and invalid nicknames are refused
- Only built-in Node.js modules (`net`, `readline`): no npm packages

## How to use

Start the server on one computer (the default port is 8888):

```sh
node src/main.js server
```

Start a client on the same or another computer. The nickname is the last argument; the port is optional:

```sh
node src/main.js client 192.168.1.20 Alice
node src/main.js client 192.168.1.20 9000 Bob
```

Type a line and press Enter to send it. Commands start with `/`:

| Command | What it does |
|---|---|
| `/nick <name>` | Set or change your nickname (letters, digits, `_` and `-`, up to 16 characters) |
| `/list` | Show who is online |
| `/quit` | Leave the chat (Ctrl+D does the same) |

Example session as seen by Bob, who typed `/list` once:

```
* Alice joined
* Bob joined
<Alice> hello everyone
* Online: Alice, Bob
<Bob> hi Alice
* Bob left
```

## How it works

**The protocol.** Every message is one line of text ending with a newline. A line that starts with `/` is a command; any other line is a chat message. The server sends back plain text lines, and the client prints them as they arrive. The first line a client sends is `/nick <name>`, so joining and renaming are the same command. The rules of the protocol are in `src/messenger.js`; the sockets and the terminal are in `src/main.js`.

**Data.** The server keeps a `Map` from each socket to its state: the unfinished end of the last received text, and the nickname (empty until the user chooses one).

**Server.** `net.createServer()` calls `Server.onConnect()` for every new connection. Node calls `onData()` when text arrives. `onData()` adds the text to the unfinished part, takes the complete lines out with `splitLines()`, and runs `handleLine()` for each one:

1. `parseLine()` finds out what the line is.
2. Chat goes to `broadcast()`, which writes the line to every socket.
3. `/nick` goes to `changeNick()`, which checks `validNick()` and whether the nickname is already in use.
4. `/list` answers with `formatList()`, and `/quit` makes `onData()` call `drop()`.

`drop()` announces the departure and ends the socket. It is also called on the `close` and `error` events, so it checks first that the client is still in the `Map`.

**Client.** `runClient()` writes `/nick <name>` to the socket. Text from the server is split into lines and printed. The `readline` module reads the terminal (or a pipe) line by line and writes each line to the socket. When the input ends, the client sends `/quit`. Node's event loop handles the socket and the input at the same time, so no extra threads are needed.

**Line limit.** A line is at most 512 characters. `splitLines()` cuts a longer line, and `formatChat()` shortens a chat line so that it always fits.

**Note.** A chat line is sent to every client except the one who wrote it, because that terminal already shows what the user typed. Join, leave and rename notices go to everybody.

## Project layout

```
messenger/
├── src/
│   ├── messenger.js    the protocol and the rules: parsing, nicknames, line splitting, formatting
│   └── main.js         the program: the server, the client and the command-line arguments
├── tests/
│   └── messenger.test.js  tests of the logic (no network needed)
├── package.json        the test command (node --test) and project information
└── README.md
```

## Requirements

- Node.js 18 or newer (no npm packages are needed)

## Run

```sh
node src/main.js server
```

In another terminal:

```sh
node src/main.js client 127.0.0.1 Alice
```

## Test

```sh
npm test
```

The tests check the protocol rules: parsing commands, nickname rules, port numbers, line splitting and the message formats. They do not open any sockets.

## Comparison with the other versions

- [C version](../../c/messenger)
- [Python version](../../python/messenger)

| | C | Python | JavaScript |
|---|---|---|---|
| Interface | terminal, POSIX sockets and `poll()` | terminal, `selectors` and a thread | terminal, `net` and `readline` |
| Lines of logic | 167 | 43 | 68 |
| Lines of interface | 308 | 143 | 166 |
| Tests | 9 | 11 | 11 |

JavaScript is event-driven: `net` calls a function when a connection arrives or data comes in, so the code never asks "is there data yet?" the way the C version does with `poll()`. The `readline` module gives the client its input lines without any buffer code, and the server gets the text already decoded from UTF-8 when `setEncoding()` is set. The logic file is the one place where the rules are written, so the same tests cover the protocol without starting any server.

## Ideas for extensions

- Private messages: `/msg <name> <text>`
- Rooms or channels, with `/join <room>`
- Replace the plain-text protocol with JSON lines, so clients can show more information
- Detect dead connections with a heartbeat (a ping every few seconds)
- Add a browser client that talks to the server through a WebSocket bridge
