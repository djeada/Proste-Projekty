'use strict';

const { test } = require('node:test');
const assert = require('node:assert/strict');
const fs = require('fs');
const os = require('os');
const path = require('path');

const vcs = require('../src/version_control.js');

// Runs fn with a new empty temporary directory and removes it afterwards.
function inTempDir(fn) {
  const dir = fs.mkdtempSync(path.join(os.tmpdir(), 'vcs-test-'));
  try {
    fn(dir);
  } finally {
    fs.rmSync(dir, { recursive: true, force: true });
  }
}

function write(dir, name, text) {
  fs.writeFileSync(path.join(dir, name), text);
}

test('init creates a repository only once', () => {
  inTempDir((dir) => {
    assert.equal(vcs.repoExists(dir), false);
    vcs.init(dir);
    assert.equal(vcs.repoExists(dir), true);
    assert.throws(() => vcs.init(dir), vcs.VcsError);
  });
});

test('commands fail outside a repository', () => {
  inTempDir((dir) => {
    assert.throws(() => vcs.commitCount(dir), vcs.VcsError);
  });
});

test('commits are numbered and keep their message', () => {
  inTempDir((dir) => {
    vcs.init(dir);
    write(dir, 'a.txt', 'one\n');
    assert.equal(vcs.commit(dir, 'first'), 1);
    write(dir, 'a.txt', 'two\n');
    assert.equal(vcs.commit(dir, 'second'), 2);
    assert.equal(vcs.commitCount(dir), 2);

    const info = vcs.commitInfo(dir, 2);
    assert.equal(info.message, 'second');
    assert.ok(info.time > 0);
  });
});

test('commit ignores subdirectories', () => {
  inTempDir((dir) => {
    vcs.init(dir);
    write(dir, 'top.txt', 'x');
    fs.mkdirSync(path.join(dir, 'sub'));
    write(path.join(dir, 'sub'), 'inner.txt', 'y');

    vcs.commit(dir, 'only top');
    assert.deepEqual([...vcs.loadCommit(dir, 1).keys()], ['top.txt']);
  });
});

test('commit rejects a message with two lines', () => {
  inTempDir((dir) => {
    vcs.init(dir);
    write(dir, 'a.txt', 'x');
    assert.throws(() => vcs.commit(dir, 'two\nlines'), vcs.VcsError);
    assert.equal(vcs.commitCount(dir), 0);
  });
});

test('changes lists added, modified and deleted files', () => {
  inTempDir((dir) => {
    vcs.init(dir);
    write(dir, 'keep.txt', 'same');
    write(dir, 'edit.txt', 'old');
    write(dir, 'gone.txt', 'bye');
    vcs.commit(dir, 'base');
    const base = vcs.loadCommit(dir, 1);

    write(dir, 'edit.txt', 'new');
    fs.rmSync(path.join(dir, 'gone.txt'));
    write(dir, 'added.txt', 'hi');
    const current = vcs.readDirectory(dir);

    assert.deepEqual(vcs.changes(base, current), [
      { kind: 'added', name: 'added.txt' },
      { kind: 'modified', name: 'edit.txt' },
      { kind: 'deleted', name: 'gone.txt' },
    ]);
    assert.deepEqual(vcs.changes(base, base), []);
  });
});

test('checkout restores the files of a commit', () => {
  inTempDir((dir) => {
    vcs.init(dir);
    write(dir, 'data.txt', 'version 1');
    vcs.commit(dir, 'v1');
    write(dir, 'data.txt', 'version 2');
    vcs.commit(dir, 'v2');

    vcs.checkout(dir, 1);
    assert.equal(fs.readFileSync(path.join(dir, 'data.txt'), 'utf8'), 'version 1');
    assert.throws(() => vcs.checkout(dir, 9), vcs.VcsError);
  });
});

test('splitLines ignores the final newline', () => {
  assert.deepEqual(vcs.splitLines(Buffer.from('')), []);
  assert.deepEqual(vcs.splitLines(Buffer.from('\n')), ['']);
  assert.deepEqual(vcs.splitLines(Buffer.from('a\nb')), ['a', 'b']);
  assert.deepEqual(vcs.splitLines(Buffer.from('a\nb\n')), ['a', 'b']);
});

test('diffLines marks the changed line', () => {
  assert.deepEqual(vcs.diffLines(['a', 'b', 'c'], ['a', 'x', 'c']), [
    { tag: ' ', line: 'a' },
    { tag: '-', line: 'b' },
    { tag: '+', line: 'x' },
    { tag: ' ', line: 'c' },
  ]);
});

test('diffLines of an added and of a deleted file', () => {
  assert.deepEqual(vcs.diffLines([], ['one', 'two']), [
    { tag: '+', line: 'one' },
    { tag: '+', line: 'two' },
  ]);
  assert.deepEqual(vcs.diffLines(['one'], []), [{ tag: '-', line: 'one' }]);
});

test('diffLines keeps the order of removed and added lines', () => {
  assert.deepEqual(vcs.diffLines(['x', 'a', 'y'], ['a', 'z']), [
    { tag: '-', line: 'x' },
    { tag: ' ', line: 'a' },
    { tag: '-', line: 'y' },
    { tag: '+', line: 'z' },
  ]);
});

test('formatTime has the form YYYY-MM-DD HH:MM:SS', () => {
  assert.match(vcs.formatTime(0), /^\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}$/);
});
