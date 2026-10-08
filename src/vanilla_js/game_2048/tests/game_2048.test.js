const test = require('node:test');
const assert = require('node:assert/strict');
const {
  PLAYING, WON, LOST, slideRow, slideBoard, spawnTile, newGame, canMove, gameStatus, move,
} = require('../src/game_2048.js');

/* A random source that returns the given values one by one. */
function scripted(values) {
  let i = 0;
  return (n) => {
    const value = values[i++];
    assert.ok(value >= 0 && value < n);
    return value;
  };
}

const noRandom = () => assert.fail('no random number should be drawn');
const EMPTY = () => Array.from({ length: 4 }, () => [0, 0, 0, 0]);

function gameWith(rows) {
  const game = newGame(scripted([0, 0, 0, 0]));
  game.board = rows.map((row) => [...row]);
  return game;
}

test('slideRow merges the classic tricky cases', () => {
  assert.deepEqual(slideRow([2, 2, 2, 2]), { line: [4, 4, 0, 0], points: 8 });
  assert.deepEqual(slideRow([4, 4, 8, 0]), { line: [8, 8, 0, 0], points: 8 });
  assert.deepEqual(slideRow([2, 0, 2, 4]), { line: [4, 4, 0, 0], points: 4 });
});

test('slideRow only slides when nothing merges', () => {
  assert.deepEqual(slideRow([0, 0, 0, 2]), { line: [2, 0, 0, 0], points: 0 });
  assert.deepEqual(slideRow([2, 4, 8, 16]), { line: [2, 4, 8, 16], points: 0 });
});

test('slideBoard moves tiles in all four directions', () => {
  const start = [
    [2, 0, 0, 0],
    [2, 0, 0, 0],
    [0, 0, 0, 0],
    [0, 0, 0, 4],
  ];
  const slid = (direction) => {
    const board = start.map((row) => [...row]);
    const points = slideBoard(board, direction);
    return { board, points };
  };
  assert.deepEqual(slid('up'), {
    board: [[4, 0, 0, 4], [0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0]],
    points: 4,
  });
  assert.deepEqual(slid('down'), {
    board: [[0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0], [4, 0, 0, 4]],
    points: 4,
  });
  assert.deepEqual(slid('left'), {
    board: [[2, 0, 0, 0], [2, 0, 0, 0], [0, 0, 0, 0], [4, 0, 0, 0]],
    points: 0,
  });
  assert.deepEqual(slid('right'), {
    board: [[0, 0, 0, 2], [0, 0, 0, 2], [0, 0, 0, 0], [0, 0, 0, 4]],
    points: 0,
  });
});

test('a move that changes nothing spawns nothing', () => {
  const game = gameWith([[2, 4, 0, 0], ...EMPTY().slice(1)]);
  assert.equal(move(game, 'left', noRandom), false);
  assert.deepEqual(game.board[0], [2, 4, 0, 0]);
  assert.equal(game.score, 0);
});

test('a move merges, adds the points and spawns one tile', () => {
  const game = gameWith([[2, 2, 0, 0], ...EMPTY().slice(1)]);
  // First empty cell (0,1); a roll of 1 gives a 2.
  assert.equal(move(game, 'left', scripted([0, 1])), true);
  assert.equal(game.score, 4);
  assert.deepEqual(game.board[0], [4, 2, 0, 0]);
});

test('a spawned tile is a 4 on a roll of 0', () => {
  const board = EMPTY();
  assert.equal(spawnTile(board, scripted([0, 0])), true);
  assert.equal(board[0][0], 4);
});

test('spawning on a full board does nothing', () => {
  const board = Array.from({ length: 4 }, () => [2, 2, 2, 2]);
  assert.equal(spawnTile(board, noRandom), false);
});

test('a full board without merges is lost', () => {
  const game = gameWith([[2, 4, 2, 4], [4, 2, 4, 2], [2, 4, 2, 4], [4, 2, 4, 2]]);
  assert.equal(canMove(game.board), false);
  assert.equal(gameStatus(game), LOST);
});

test('two equal neighbours keep the game going', () => {
  const game = gameWith([[2, 2, 2, 4], [4, 2, 4, 2], [2, 4, 2, 4], [4, 2, 4, 2]]);
  assert.equal(gameStatus(game), PLAYING);
});

test('reaching 2048 wins until the player continues', () => {
  const game = gameWith([[1024, 1024, 0, 0], ...EMPTY().slice(1)]);
  assert.equal(move(game, 'left', scripted([0, 5])), true);
  assert.equal(game.board[0][0], 2048);
  assert.equal(gameStatus(game), WON);

  assert.equal(move(game, 'right', noRandom), false);
  game.keepPlaying = true;
  assert.equal(gameStatus(game), PLAYING);
});
