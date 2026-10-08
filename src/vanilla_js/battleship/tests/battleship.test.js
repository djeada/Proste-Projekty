const { test } = require('node:test');
const assert = require('node:assert/strict');
const { BOARD_SIZE, FLEET, MISS, HIT, SUNK, REPEAT, INVALID, Board, Computer } = require('../src/battleship.js');

// A small seeded random source (mulberry32), so the tests are repeatable.
function seeded(seed) {
  let a = seed;
  return () => {
    a = (a + 0x6d2b79f5) | 0;
    let t = Math.imul(a ^ (a >>> 15), 1 | a);
    t = (t + Math.imul(t ^ (t >>> 7), 61 | t)) ^ t;
    return ((t ^ (t >>> 14)) >>> 0) / 4294967296;
  };
}

test('ships must stay on the board', () => {
  const board = new Board();
  assert.equal(board.place(0, 0, 0, true), true);
  assert.equal(board.place(1, 8, 0, true), false); // would leave the right edge
  assert.equal(board.place(1, 0, 9, false), false); // would leave the bottom edge
  assert.equal(board.place(1, 9, 0, false), true); // exactly fits in the last column
});

test('ships may touch but not overlap, and are placed once', () => {
  const board = new Board();
  board.place(0, 0, 0, true); // cells (0..4, 0)
  assert.equal(board.place(1, 4, 0, false), false); // overlaps at (4, 0)
  assert.equal(board.place(1, 5, 0, true), true); // touches the first ship: allowed
  assert.equal(board.place(1, 0, 2, true), false); // already placed
});

test('fire reports miss, hit and sunk', () => {
  const board = new Board();
  board.place(4, 0, 0, true); // length 2 ship at (0,0)-(1,0)
  assert.equal(board.fire(5, 5), MISS);
  assert.equal(board.fire(0, 0), HIT);
  assert.equal(board.fire(1, 0), SUNK);
  assert.equal(board.allSunk(), false); // the other ships are not placed
});

test('repeated and out-of-board shots change nothing', () => {
  const board = new Board();
  board.place(4, 0, 0, true);
  assert.equal(board.fire(0, 0), HIT);
  assert.equal(board.fire(0, 0), REPEAT);
  assert.equal(board.ships[4].hits, 1);
  assert.equal(board.fire(10, 0), INVALID);
  assert.equal(board.fire(-1, 3), INVALID);
});

test('all ships sunk means the fleet is destroyed', () => {
  const board = new Board();
  board.placeRandomly(seeded(1));
  assert.equal(board.fleetPlaced(), true);
  for (let y = 0; y < BOARD_SIZE; y++) {
    for (let x = 0; x < BOARD_SIZE; x++) board.fire(x, y);
  }
  assert.equal(board.allSunk(), true);
});

test('random fleet is valid and repeatable with the same seed', () => {
  for (let seed = 1; seed <= 20; seed++) {
    const board = new Board();
    board.placeRandomly(seeded(seed));
    const cells = board.shipAt.flat().filter((index) => index !== null);
    assert.equal(cells.length, FLEET.reduce((sum, n) => sum + n, 0));
    assert.equal(board.fleetPlaced(), true);
  }
  const first = new Board();
  first.placeRandomly(seeded(7));
  const again = new Board();
  again.placeRandomly(seeded(7));
  assert.deepEqual(first.shipAt, again.shipAt);
});

test('computer targets the neighbours of a hit', () => {
  const board = new Board();
  board.place(0, 0, 0, true);
  const computer = new Computer();
  board.fire(5, 5);
  computer.report(board, 5, 5, HIT);
  assert.equal(computer.targets.length, 4);
  const next = computer.choose(board, seeded(3));
  assert.ok([[4, 5], [6, 5], [5, 4], [5, 6]].some(([x, y]) => x === next.x && y === next.y));
});

test('computer forgets its targets when a ship sinks', () => {
  const board = new Board();
  const computer = new Computer();
  computer.report(board, 5, 5, HIT);
  assert.equal(computer.targets.length, 4);
  computer.report(board, 5, 5, SUNK);
  assert.deepEqual(computer.targets, []);
});

test('computer finishes a whole game without repeating a shot', () => {
  const random = seeded(42);
  const enemy = new Board();
  enemy.placeRandomly(random);
  const computer = new Computer();
  let shots = 0;
  while (!enemy.allSunk()) {
    const { x, y } = computer.choose(enemy, random);
    const result = enemy.fire(x, y);
    assert.ok([MISS, HIT, SUNK].includes(result));
    computer.report(enemy, x, y, result);
    shots++;
    assert.ok(shots <= BOARD_SIZE * BOARD_SIZE);
  }
});
