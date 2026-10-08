const { test } = require('node:test');
const assert = require('node:assert/strict');
const { Game, makeRandom } = require('../src/minesweeper.js');

function countMines(game) {
  return game.mine.flat().filter(Boolean).length;
}

test('a new game has no mines until the first reveal', () => {
  const game = new Game(9, 9, 10, makeRandom(42));
  assert.equal(game.state, 'playing');
  assert.equal(game.minesPlaced, false);
  assert.equal(countMines(game), 0);
  assert.equal(game.minesLeft, 10);
});

test('the first reveal is never a mine or next to one', () => {
  for (let seed = 1; seed <= 50; seed++) {
    const game = new Game(9, 9, 10, makeRandom(seed));
    game.reveal(0, 0);
    assert.equal(game.state, 'playing');
    assert.equal(countMines(game), 10);
    for (let r = 0; r <= 1; r++) {
      for (let c = 0; c <= 1; c++) {
        assert.equal(game.mine[r][c], false);
      }
    }
  }
});

test('first reveal in the middle of the expert board keeps its 3x3 area free', () => {
  const game = new Game(16, 30, 99, makeRandom(7));
  game.reveal(8, 15);
  assert.equal(countMines(game), 99);
  for (let r = 7; r <= 9; r++) {
    for (let c = 14; c <= 16; c++) {
      assert.equal(game.mine[r][c], false);
    }
  }
});

test('the same seed gives the same mine layout', () => {
  const first = new Game(16, 16, 40, makeRandom(123));
  const second = new Game(16, 16, 40, makeRandom(123));
  first.reveal(3, 3);
  second.reveal(3, 3);
  assert.deepEqual(first.mine, second.mine);
});

test('neighbor counts match the mine layout', () => {
  const game = new Game(16, 16, 40, makeRandom(99));
  game.reveal(0, 0);
  for (let r = 0; r < game.rows; r++) {
    for (let c = 0; c < game.cols; c++) {
      let expected = 0;
      for (let nr = Math.max(r - 1, 0); nr <= Math.min(r + 1, game.rows - 1); nr++) {
        for (let nc = Math.max(c - 1, 0); nc <= Math.min(c + 1, game.cols - 1); nc++) {
          if ((nr !== r || nc !== c) && game.mine[nr][nc]) expected++;
        }
      }
      if (!game.mine[r][c]) assert.equal(game.neighbors[r][c], expected);
    }
  }
});

test('flood fill opens the whole empty area and wins', () => {
  // 5x5 board with one mine in the top-right corner: all 24 safe cells are reachable.
  const game = new Game(5, 5, 1, () => 0.5);
  game.minesPlaced = true;
  game.mine[0][4] = true;
  game.neighbors[0][3] = 1;
  game.neighbors[1][3] = 1;
  game.neighbors[1][4] = 1;

  game.reveal(4, 0);
  assert.equal(game.revealedCount, 24);
  assert.equal(game.state, 'won');
  assert.equal(game.revealed[0][4], false);
});

test('flood fill stops at numbers', () => {
  // Mine in the middle of a 3x3 board: every other cell shows a 1, so nothing spreads.
  const game = new Game(3, 3, 1, () => 0.5);
  game.minesPlaced = true;
  game.mine[1][1] = true;
  for (let r = 0; r < 3; r++) {
    for (let c = 0; c < 3; c++) {
      if (!game.mine[r][c]) game.neighbors[r][c] = 1;
    }
  }

  game.reveal(0, 0);
  assert.equal(game.revealedCount, 1);
  assert.equal(game.state, 'playing');
});

test('revealing a mine loses and shows all mines', () => {
  const game = new Game(9, 9, 10, makeRandom(5));
  game.reveal(4, 4);
  let found = null;
  for (let r = 0; r < 9 && !found; r++) {
    for (let c = 0; c < 9; c++) {
      if (game.mine[r][c]) {
        found = [r, c];
        break;
      }
    }
  }
  game.reveal(...found);
  assert.equal(game.state, 'lost');
  for (let r = 0; r < 9; r++) {
    for (let c = 0; c < 9; c++) {
      if (game.mine[r][c]) assert.equal(game.revealed[r][c], true);
    }
  }
});

test('revealing all safe cells wins', () => {
  const game = new Game(9, 9, 10, makeRandom(77));
  game.reveal(4, 4);
  for (let r = 0; r < 9; r++) {
    for (let c = 0; c < 9; c++) {
      if (!game.mine[r][c]) game.reveal(r, c);
    }
  }
  assert.equal(game.state, 'won');
  assert.equal(game.revealedCount, 71);
});

test('flags toggle and change the mines-left count', () => {
  const game = new Game(9, 9, 10, makeRandom(3));
  game.toggleFlag(0, 0);
  game.toggleFlag(8, 8);
  assert.equal(game.flagged[0][0], true);
  assert.equal(game.minesLeft, 8);
  game.toggleFlag(0, 0);
  assert.equal(game.flagged[0][0], false);
  assert.equal(game.minesLeft, 9);
});

test('a flagged cell cannot be revealed', () => {
  const game = new Game(9, 9, 10, makeRandom(3));
  game.toggleFlag(2, 2);
  game.reveal(2, 2);
  assert.equal(game.minesPlaced, false);
  assert.equal(game.revealedCount, 0);
});

test('a flag on a revealed cell is ignored', () => {
  const game = new Game(9, 9, 10, makeRandom(3));
  game.reveal(4, 4);
  game.toggleFlag(4, 4);
  assert.equal(game.flagged[4][4], false);
  assert.equal(game.flagCount, 0);
});

test('moves outside the board are ignored', () => {
  const game = new Game(9, 9, 10, makeRandom(3));
  game.reveal(-1, 0);
  game.reveal(9, 9);
  game.toggleFlag(0, 30);
  assert.equal(game.state, 'playing');
  assert.equal(game.minesPlaced, false);
  assert.equal(game.flagCount, 0);
});
