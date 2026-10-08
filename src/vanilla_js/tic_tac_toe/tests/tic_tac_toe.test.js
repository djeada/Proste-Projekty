// Tests of the rules: no DOM, no input, no network.
const { test } = require('node:test');
const assert = require('node:assert/strict');
const {
  newBoard, otherMark, legalMoves, play, winningLine, winner, isOver, isDraw, bestMove,
} = require('../src/tic_tac_toe.js');

// Builds a board from 9 characters: 'X', 'O' or '.' for an empty cell.
function parse(text) {
  return [...text].map((char) => (char === '.' ? null : char));
}

test('new board has nine legal moves and no winner', () => {
  const board = newBoard();
  assert.deepEqual(legalMoves(board), [0, 1, 2, 3, 4, 5, 6, 7, 8]);
  assert.equal(winner(board), null);
  assert.equal(isOver(board), false);
});

test('play puts the mark on a copy and rejects a taken cell', () => {
  const board = newBoard();
  const after = play(board, 4, 'X');
  assert.equal(after[4], 'X');
  assert.equal(board[4], null);
  assert.throws(() => play(after, 4, 'O'));
});

test('detects every kind of line', () => {
  const cases = [
    ['XXX......', [0, 1, 2]], ['...XXX...', [3, 4, 5]], ['......XXX', [6, 7, 8]],
    ['X..X..X..', [0, 3, 6]], ['.X..X..X.', [1, 4, 7]], ['..X..X..X', [2, 5, 8]],
    ['X...X...X', [0, 4, 8]], ['..X.X.X..', [2, 4, 6]],
  ];
  for (const [text, line] of cases) {
    const board = parse(text);
    assert.deepEqual(winningLine(board), line);
    assert.equal(winner(board), 'X');
    assert.equal(isOver(board), true);
  }
});

test('an open game has no winner', () => {
  const board = parse('XO.......');
  assert.equal(winningLine(board), null);
  assert.equal(winner(board), null);
  assert.equal(isOver(board), false);
});

test('a full board without a winner is a draw', () => {
  const board = parse('XOXXOOOXX');
  assert.equal(isDraw(board), true);
  assert.equal(isOver(board), true);
});

test('a full board with a winner is not a draw', () => {
  const board = parse('XXXOOXOXO');
  assert.equal(winner(board), 'X');
  assert.equal(isDraw(board), false);
});

test('the computer takes an immediate win', () => {
  // X has 0 and 1, O has 3 and 4: O wins on cell 5.
  assert.equal(bestMove(parse('XX.OO....'), 'O'), 5);
});

test('the computer blocks an immediate loss', () => {
  // X threatens 0-1-2 and O cannot win at once, so O must take cell 2.
  assert.equal(bestMove(parse('XX..O...O'), 'O'), 2);
});

test('two computers always draw', () => {
  let board = newBoard();
  let turn = 'X';
  while (!isOver(board)) {
    board = play(board, bestMove(board, turn), turn);
    turn = otherMark(turn);
  }
  assert.equal(isDraw(board), true);
});
