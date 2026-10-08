'use strict';
const { test } = require('node:test');
const assert = require('node:assert/strict');
const {
  MAX_LINE,
  parseLine,
  validNick,
  parsePort,
  splitLines,
  formatChat,
  formatJoin,
  formatLeave,
  formatRename,
  formatList,
} = require('../src/messenger.js');

test('chat text is stripped', () => {
  assert.deepEqual(parseLine('  hello   there '), { kind: 'chat', arg: 'hello   there' });
});

test('a blank line is empty', () => {
  assert.deepEqual(parseLine('   '), { kind: 'empty', arg: '' });
});

test('commands are recognised with their argument', () => {
  assert.deepEqual(parseLine('/nick  Alice '), { kind: 'nick', arg: 'Alice' });
  assert.deepEqual(parseLine('/nick'), { kind: 'nick', arg: '' });
  assert.deepEqual(parseLine('/list'), { kind: 'list', arg: '' });
  assert.deepEqual(parseLine('/quit'), { kind: 'quit', arg: '' });
});

test('unknown commands are not chat', () => {
  assert.equal(parseLine('/nickname Bob').kind, 'unknown');
  assert.equal(parseLine('/help').kind, 'unknown');
});

test('nicknames: letters, digits, _ and -, 1 to 16 characters', () => {
  assert.ok(validNick('Alice'));
  assert.ok(validNick('a_b-1'));
  assert.ok(validNick('0123456789abcdef')); // exactly 16 characters
  assert.ok(!validNick(''));
  assert.ok(!validNick('a b'));
  assert.ok(!validNick('0123456789abcdefg'));
  assert.ok(!validNick('<script>'));
});

test('ports must be numbers from 1 to 65535', () => {
  assert.equal(parsePort('8888'), 8888);
  assert.equal(parsePort('65535'), 65535);
  assert.equal(parsePort('0'), null);
  assert.equal(parsePort('65536'), null);
  assert.equal(parsePort('80a'), null);
  assert.equal(parsePort(''), null);
});

test('splitLines keeps the unfinished rest', () => {
  assert.deepEqual(splitLines('one\r\ntwo\npart'), { lines: ['one', 'two'], rest: 'part' });
});

test('splitLines cuts a very long line', () => {
  assert.deepEqual(splitLines('x'.repeat(MAX_LINE)), { lines: ['x'.repeat(MAX_LINE)], rest: '' });
});

test('chat lines fit in the line limit', () => {
  assert.equal(formatChat('Alice', 'hi'), '<Alice> hi');
  assert.equal(formatChat('Alice', 'x'.repeat(MAX_LINE)).length, MAX_LINE - 1);
});

test('notices', () => {
  assert.equal(formatJoin('Bob'), '* Bob joined');
  assert.equal(formatLeave('Bob'), '* Bob left');
  assert.equal(formatRename('Bob', 'Carl'), '* Bob is now known as Carl');
});

test('the list of online users', () => {
  assert.equal(formatList(['Alice', 'Bob']), '* Online: Alice, Bob');
  assert.equal(formatList([]), '* Nobody is online');
});
