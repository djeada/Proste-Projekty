'use strict';
// Sockets and terminal: runs the chat server or a chat client.

const net = require('node:net');
const readline = require('node:readline');
const {
  MAX_CLIENTS,
  DEFAULT_PORT,
  parseLine,
  validNick,
  parsePort,
  splitLines,
  formatChat,
  formatJoin,
  formatLeave,
  formatRename,
  formatList,
} = require('./messenger.js');

class Server {
  constructor() {
    this.clients = new Map(); // socket -> { pending: unfinished input, nick: '' or the nickname }
  }

  onConnect(socket) {
    if (this.clients.size >= MAX_CLIENTS) {
      socket.end('* The server is full\n');
      return;
    }
    this.clients.set(socket, { pending: '', nick: '' });
    socket.setEncoding('utf8');
    socket.on('data', (chunk) => this.onData(socket, chunk));
    socket.on('close', () => this.drop(socket));
    socket.on('error', () => this.drop(socket));
  }

  onData(socket, chunk) {
    const client = this.clients.get(socket);
    if (!client) {
      return;
    }
    const { lines, rest } = splitLines(client.pending + chunk);
    client.pending = rest;
    for (const line of lines) {
      if (!this.handleLine(socket, line)) {
        this.drop(socket);
        return;
      }
    }
  }

  // Returns false when the user has quit.
  handleLine(socket, line) {
    const { kind, arg } = parseLine(line);
    const client = this.clients.get(socket);
    if (kind === 'chat') {
      if (client.nick) {
        this.broadcast(formatChat(client.nick, arg), socket);
      } else {
        this.reply(socket, '* Set a nickname first: /nick <name>');
      }
    } else if (kind === 'nick') {
      this.changeNick(socket, arg);
    } else if (kind === 'list') {
      this.reply(socket, formatList(this.nicks()));
    } else if (kind === 'unknown') {
      this.reply(socket, '* Unknown command (try /nick, /list, /quit)');
    }
    return kind !== 'quit';
  }

  changeNick(socket, nick) {
    const client = this.clients.get(socket);
    if (!validNick(nick)) {
      this.reply(socket, "* Invalid nickname: use 1-16 letters, digits, '_' or '-'");
    } else if (this.nicks().includes(nick)) {
      this.reply(socket, `* The nickname ${nick} is taken`);
    } else if (client.nick) {
      this.broadcast(formatRename(client.nick, nick));
      client.nick = nick;
    } else {
      this.broadcast(formatJoin(nick));
      client.nick = nick;
    }
  }

  nicks() {
    return [...this.clients.values()].map((client) => client.nick).filter((nick) => nick !== '');
  }

  reply(socket, line) {
    socket.write(`${line}\n`);
  }

  // Sends line to every client except sender (if given).
  broadcast(line, sender = null) {
    console.log(line);
    for (const socket of this.clients.keys()) {
      if (socket !== sender) {
        this.reply(socket, line);
      }
    }
  }

  // Called when a user quits or disconnects; it may run twice, so it checks first.
  drop(socket) {
    const client = this.clients.get(socket);
    if (!client) {
      return;
    }
    this.clients.delete(socket);
    if (client.nick) {
      this.broadcast(formatLeave(client.nick));
    }
    socket.end();
  }
}

function runServer(port) {
  const server = new Server();
  const listener = net.createServer((socket) => server.onConnect(socket));
  listener.on('error', (error) => {
    console.error(`Cannot listen on port ${port}: ${error.message}`);
    process.exit(1);
  });
  listener.listen(port, () => console.log(`Server listening on port ${port}`));
}

function runClient(host, port, nick) {
  const socket = net.connect(port, host);
  socket.setEncoding('utf8');
  socket.write(`/nick ${nick}\n`);

  let pending = '';
  socket.on('data', (chunk) => {
    const { lines, rest } = splitLines(pending + chunk);
    pending = rest;
    for (const line of lines) {
      console.log(line);
    }
  });
  socket.on('end', () => {
    console.log('* Disconnected from the server');
    process.exit(0);
  });
  socket.on('error', (error) => {
    console.error(`Cannot connect to ${host}:${port}: ${error.message}`);
    process.exit(1);
  });

  // Every typed line goes to the server; the end of input means /quit.
  const input = readline.createInterface({ input: process.stdin });
  input.on('line', (line) => socket.write(`${line}\n`));
  input.on('close', () => socket.write('/quit\n'));
}

const USAGE = [
  'Usage:',
  '  node src/main.js server [port]',
  '  node src/main.js client <host> [port] <nick>',
].join('\n');

function main(args) {
  if (args.length <= 2 && args[0] === 'server') {
    const port = args.length === 2 ? parsePort(args[1]) : DEFAULT_PORT;
    if (port !== null) {
      runServer(port);
      return;
    }
  } else if ((args.length === 3 || args.length === 4) && args[0] === 'client') {
    const host = args[1];
    const nick = args[args.length - 1];
    const port = args.length === 4 ? parsePort(args[2]) : DEFAULT_PORT;
    if (port !== null && validNick(nick)) {
      runClient(host, port, nick);
      return;
    }
  }
  console.error(USAGE);
  process.exitCode = 1;
}

main(process.argv.slice(2));
