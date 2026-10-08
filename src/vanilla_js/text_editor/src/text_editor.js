// Text helpers of the editor: search, positions and counts. No DOM here.

// Returns the index of the first match at or after start, wrapping to the beginning.
// Returns -1 when there is no match or the needle is empty.
function findNext(text, needle, start) {
  if (needle === '') {
    return -1;
  }
  const index = text.indexOf(needle, start);
  return index === -1 ? text.indexOf(needle) : index;
}

// Returns the 1-based { line, column } of the character at offset.
function lineCol(text, offset) {
  const before = text.slice(0, offset);
  return { line: before.split('\n').length, column: offset - before.lastIndexOf('\n') };
}

// Returns the offset of the first character of a line (1-based), or -1 when there is no such line.
function lineStart(text, line) {
  let offset = 0;
  for (let i = 1; i < line; i++) {
    const next = text.indexOf('\n', offset);
    if (next === -1) {
      return -1;
    }
    offset = next + 1;
  }
  return offset;
}

// A final newline does not start a new line, the same rule as in the C and Python versions.
function countLines(text) {
  return text.split('\n').length - (text.endsWith('\n') ? 1 : 0);
}

function countWords(text) {
  return text.split(/\s+/).filter(Boolean).length;
}

if (typeof module !== 'undefined') module.exports = { findNext, lineCol, lineStart, countLines, countWords };
