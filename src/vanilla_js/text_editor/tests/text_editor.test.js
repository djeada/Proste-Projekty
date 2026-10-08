const test = require('node:test');
const assert = require('node:assert/strict');
const { findNext, lineCol, lineStart, countLines, countWords } = require('../src/text_editor.js');

test('findNext returns the first match at or after the start', () => {
  assert.equal(findNext('foo bar foo', 'foo', 0), 0);
  assert.equal(findNext('foo bar foo', 'foo', 1), 8);
});

test('findNext wraps around to the beginning', () => {
  assert.equal(findNext('foo bar', 'foo', 1), 0);
  assert.equal(findNext('bar foo', 'bar', 4), 0);
});

test('findNext reports a missing or empty needle as -1', () => {
  assert.equal(findNext('foo bar', 'baz', 0), -1);
  assert.equal(findNext('foo bar', '', 0), -1);
});

test('lineCol counts lines and columns from one', () => {
  const text = 'ab\ncd';
  assert.deepEqual(lineCol(text, 0), { line: 1, column: 1 });
  assert.deepEqual(lineCol(text, 2), { line: 1, column: 3 });
  assert.deepEqual(lineCol(text, 3), { line: 2, column: 1 });
  assert.deepEqual(lineCol(text, 5), { line: 2, column: 3 });
});

test('lineStart gives the offset of a line or -1', () => {
  const text = 'ab\ncd\n\nef';
  assert.equal(lineStart(text, 1), 0);
  assert.equal(lineStart(text, 2), 3);
  assert.equal(lineStart(text, 4), 7);
  assert.equal(lineStart(text, 5), -1);
});

test('countLines does not count a final newline as a line', () => {
  assert.equal(countLines(''), 1);
  assert.equal(countLines('a'), 1);
  assert.equal(countLines('a\n'), 1);
  assert.equal(countLines('a\n\nb'), 3);
});

test('countWords splits on any whitespace', () => {
  assert.equal(countWords(''), 0);
  assert.equal(countWords('one two\n  three\n\nfour '), 4);
});
