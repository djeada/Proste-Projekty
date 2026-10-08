// Logic of the version control tool: commits, checkout, status and diffs.
'use strict';

const fs = require('fs');
const path = require('path');

const VCS_DIR = '.vcs';
const MAX_MESSAGE_BYTES = 254;

class VcsError extends Error {}

function commitDir(directory, number) {
  return path.join(directory, VCS_DIR, 'commits', String(number));
}

function sortedNames(files) {
  return [...files.keys()].sort();
}

function repoExists(directory) {
  return fs.existsSync(path.join(directory, VCS_DIR));
}

function init(directory) {
  if (repoExists(directory)) {
    throw new VcsError('Could not create a repository (already initialized?).');
  }
  fs.mkdirSync(path.join(directory, VCS_DIR, 'commits'), { recursive: true });
}

function commitCount(directory) {
  if (!repoExists(directory)) {
    throw new VcsError("Not a repository. Run 'version_control init' first.");
  }
  let count = 0;
  while (fs.existsSync(path.join(commitDir(directory, count + 1), 'meta'))) {
    count++;
  }
  return count;
}

// Returns a Map of file name -> Buffer for the regular files directly inside a directory.
function readDirectory(directory) {
  const files = new Map();
  for (const entry of fs.readdirSync(directory, { withFileTypes: true })) {
    if (entry.isFile()) {
      files.set(entry.name, fs.readFileSync(path.join(directory, entry.name)));
    }
  }
  return files;
}

// Saves the files of the directory as a new commit and returns its number.
function commit(directory, message) {
  if (message.includes('\n') || Buffer.byteLength(message, 'utf8') > MAX_MESSAGE_BYTES) {
    throw new VcsError(
      `Commit failed. The message must be one line and shorter than ${MAX_MESSAGE_BYTES + 1} bytes.`,
    );
  }
  const number = commitCount(directory) + 1;
  const folder = commitDir(directory, number);
  const files = path.join(folder, 'files');
  fs.mkdirSync(files, { recursive: true });
  for (const [name, data] of readDirectory(directory)) {
    fs.writeFileSync(path.join(files, name), data);
  }
  const stamp = Math.floor(Date.now() / 1000);
  fs.writeFileSync(path.join(folder, 'meta'), `${stamp}\n${message}\n`);
  return number;
}

// Returns { time, message } of a commit.
function commitInfo(directory, number) {
  const text = fs.readFileSync(path.join(commitDir(directory, number), 'meta'), 'utf8');
  const [stamp, message] = text.split('\n');
  return { time: Number(stamp), message };
}

// Returns a Map of file name -> Buffer stored in a commit.
function loadCommit(directory, number) {
  const folder = path.join(commitDir(directory, number), 'files');
  if (!fs.existsSync(folder)) {
    throw new VcsError(`There is no commit #${number}.`);
  }
  return readDirectory(folder);
}

// Writes the files of a commit into the directory, overwriting files with the same name.
function checkout(directory, number) {
  for (const [name, data] of loadCommit(directory, number)) {
    fs.writeFileSync(path.join(directory, name), data);
  }
}

// Compares two snapshots (Maps). Returns [{ kind, name }] with kind added, modified or deleted.
function changes(base, current) {
  const result = [];
  for (const name of sortedNames(current)) {
    if (!base.has(name)) {
      result.push({ kind: 'added', name });
    } else if (!base.get(name).equals(current.get(name))) {
      result.push({ kind: 'modified', name });
    }
  }
  for (const name of sortedNames(base)) {
    if (!current.has(name)) {
      result.push({ kind: 'deleted', name });
    }
  }
  return result;
}

function splitLines(data) {
  const lines = data.toString('utf8').split('\n');
  if (lines[lines.length - 1] === '') {
    lines.pop();
  }
  return lines;
}

// Returns [{ tag, line }]: tag ' ' for a common line, '-' for a removed one, '+' for an added one.
// The common lines come from a longest common subsequence table: lcs[i][j] is the length of the
// longest common subsequence of oldLines[i..] and newLines[j..].
function diffLines(oldLines, newLines) {
  const n = oldLines.length;
  const m = newLines.length;
  const lcs = Array.from({ length: n + 1 }, () => new Array(m + 1).fill(0));
  for (let i = n - 1; i >= 0; i--) {
    for (let j = m - 1; j >= 0; j--) {
      lcs[i][j] = oldLines[i] === newLines[j]
        ? lcs[i + 1][j + 1] + 1
        : Math.max(lcs[i + 1][j], lcs[i][j + 1]);
    }
  }

  const result = [];
  let i = 0;
  let j = 0;
  while (i < n || j < m) {
    if (i < n && j < m && oldLines[i] === newLines[j]) {
      result.push({ tag: ' ', line: oldLines[i] });
      i++;
      j++;
    } else if (i < n && (j === m || lcs[i + 1][j] >= lcs[i][j + 1])) {
      result.push({ tag: '-', line: oldLines[i] });
      i++;
    } else {
      result.push({ tag: '+', line: newLines[j] });
      j++;
    }
  }
  return result;
}

function formatTime(stamp) {
  const date = new Date(stamp * 1000);
  const pad = (value) => String(value).padStart(2, '0');
  return `${date.getFullYear()}-${pad(date.getMonth() + 1)}-${pad(date.getDate())} `
    + `${pad(date.getHours())}:${pad(date.getMinutes())}:${pad(date.getSeconds())}`;
}

module.exports = {
  VCS_DIR,
  VcsError,
  repoExists,
  init,
  commitCount,
  readDirectory,
  commit,
  commitInfo,
  loadCommit,
  checkout,
  changes,
  splitLines,
  diffLines,
  formatTime,
};
