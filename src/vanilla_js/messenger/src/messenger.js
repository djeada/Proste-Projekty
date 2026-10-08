'use strict';
// The chat protocol and the rules of the server: no sockets, no printing.

const MAX_LINE = 512; // longest line, without the newline
const NICK_MAX = 16;
const MAX_CLIENTS = 16;
const DEFAULT_PORT = 8888;

const COMMANDS = { '/nick': 'nick', '/list': 'list', '/quit': 'quit' };

// Returns { kind, arg }. The kind is 'empty', 'chat', 'nick', 'list', 'quit' or 'unknown'.
function parseLine(line) {
  if (!line.startsWith('/')) {
    const text = line.trim();
    return text ? { kind: 'chat', arg: text } : { kind: 'empty', arg: '' };
  }
  const space = line.indexOf(' ');
  const command = space === -1 ? line : line.slice(0, space);
  const arg = space === -1 ? '' : line.slice(space + 1).trim();
  return { kind: COMMANDS[command] || 'unknown', arg };
}

function validNick(nick) {
  return new RegExp(`^[A-Za-z0-9_-]{1,${NICK_MAX}}$`).test(nick);
}

// Returns the port number, or null if text is not a number from 1 to 65535.
function parsePort(text) {
  const port = Number(text);
  return /^\d+$/.test(text) && port >= 1 && port <= 65535 ? port : null;
}

// Returns { lines, rest }: the complete lines and the unfinished end of text.
// A rest of MAX_LINE characters counts as a line.
function splitLines(text) {
  const parts = text.split('\n');
  let rest = parts.pop();
  const lines = parts.map((line) => line.replace(/\r$/, ''));
  while (rest.length >= MAX_LINE) {
    lines.push(rest.slice(0, MAX_LINE));
    rest = rest.slice(MAX_LINE);
  }
  return { lines, rest };
}

function formatChat(nick, text) {
  return `<${nick}> ${text}`.slice(0, MAX_LINE - 1);
}

function formatJoin(nick) {
  return `* ${nick} joined`;
}

function formatLeave(nick) {
  return `* ${nick} left`;
}

function formatRename(oldNick, newNick) {
  return `* ${oldNick} is now known as ${newNick}`;
}

// nicks: the nicknames of the users who are online.
function formatList(nicks) {
  return nicks.length === 0 ? '* Nobody is online' : `* Online: ${nicks.join(', ')}`;
}

module.exports = {
  MAX_LINE,
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
};
