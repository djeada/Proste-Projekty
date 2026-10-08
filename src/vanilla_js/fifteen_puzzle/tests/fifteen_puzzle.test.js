const { test } = require('node:test');
const assert = require('node:assert/strict');
const { solvedBoard, gapIndex, slide, moveGap, shuffle, isSolved, isSolvable } = require('../src/fifteen_puzzle.js');

/* A small seeded random generator, so every test is repeatable. */
function seededRandom(seed) {
  let state = seed;
  return () => {
    state = (Math.imul(state, 1103515245) + 12345) >>> 0;
    return state / 4294967296;
  };
}

test('the solved board is solved and solvable', () => {
  const board = solvedBoard();
  assert.deepEqual(board.slice(0, 3), [1, 2, 3]);
  assert.deepEqual(board.slice(-2), [15, 0]);
  assert.equal(gapIndex(board), 15);
  assert.ok(isSolved(board));
  assert.ok(isSolvable(board));
});

test('a tile slides only when it is next to the gap', () => {
  const board = solvedBoard();
  assert.equal(slide(board, 10), false); /* diagonal to the gap */
  assert.equal(slide(board, 3), false); /* far away */
  assert.equal(slide(board, 16), false); /* outside the board */
  assert.equal(slide(board, 11), true); /* tile 12 is above the gap */
  assert.equal(board[15], 12);
  assert.equal(board[11], 0);
});

test('the gap stops at the edges of the board', () => {
  const board = solvedBoard();
  assert.equal(moveGap(board, 'down'), false);
  assert.equal(moveGap(board, 'right'), false);
  assert.equal(moveGap(board, 'up'), true);
  assert.equal(gapIndex(board), 11);
  assert.equal(moveGap(board, 'left'), true);
  assert.equal(gapIndex(board), 10);
});

test('moving back returns to the solved board', () => {
  const board = solvedBoard();
  moveGap(board, 'left');
  assert.equal(isSolved(board), false);
  moveGap(board, 'right');
  assert.ok(isSolved(board));
});

test('a board after a few moves stays solvable', () => {
  const board = solvedBoard();
  moveGap(board, 'up');
  moveGap(board, 'left');
  assert.ok(isSolvable(board));
});

test('swapping tiles 14 and 15 gives an unsolvable board', () => {
  const board = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 15, 14, 0];
  assert.equal(isSolvable(board), false);
  assert.equal(isSolved(board), false);
});

test('shuffle always gives a solvable, unsolved board', () => {
  for (let seed = 1; seed <= 50; seed++) {
    const board = shuffle([], seededRandom(seed));
    assert.ok(isSolvable(board), `seed ${seed}`);
    assert.equal(isSolved(board), false, `seed ${seed}`);
  }
});

test('the same random source gives the same shuffle', () => {
  const first = shuffle([], seededRandom(42));
  const second = shuffle([], seededRandom(42));
  assert.deepEqual(first, second);
});

test('shuffle keeps every tile exactly once', () => {
  const board = shuffle([], seededRandom(7));
  assert.deepEqual([...board].sort((a, b) => a - b), Array.from({ length: 16 }, (_, i) => i));
});
