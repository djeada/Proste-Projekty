/* Rules of 2048: sliding, merging, spawning and win/lose checks. No DOM here. */
const SIZE = 4;
const PLAYING = 'playing';
const WON = 'won';
const LOST = 'lost';

function defaultRandomBelow(n) {
  return Math.floor(Math.random() * n);
}

/* Merges one line towards its start. Returns the new line and the points gained. */
function slideRow(line) {
  const tiles = line.filter((value) => value !== 0);
  const merged = [];
  let points = 0;
  let i = 0;
  while (i < tiles.length) {
    if (i + 1 < tiles.length && tiles[i] === tiles[i + 1]) {
      // Two equal tiles merge once; the next tile is not looked at again.
      merged.push(2 * tiles[i]);
      points += 2 * tiles[i];
      i += 2;
    } else {
      merged.push(tiles[i]);
      i += 1;
    }
  }
  while (merged.length < line.length) merged.push(0);
  return { line: merged, points };
}

/* Cells of one row or column, ordered from the side the tiles slide towards. */
function lineCells(direction, index) {
  const last = SIZE - 1;
  const cells = [];
  for (let k = 0; k < SIZE; k++) {
    if (direction === 'left') cells.push([index, k]);
    else if (direction === 'right') cells.push([index, last - k]);
    else if (direction === 'up') cells.push([k, index]);
    else cells.push([last - k, index]);
  }
  return cells;
}

/* Slides every line of the board in place. Returns the points gained. */
function slideBoard(board, direction) {
  let points = 0;
  for (let index = 0; index < SIZE; index++) {
    const cells = lineCells(direction, index);
    const result = slideRow(cells.map(([r, c]) => board[r][c]));
    points += result.points;
    cells.forEach(([r, c], k) => {
      board[r][c] = result.line[k];
    });
  }
  return points;
}

/* Puts a 2 (90%) or a 4 (10%) on a random empty cell. Returns false if the board is full. */
function spawnTile(board, randomBelow = defaultRandomBelow) {
  const empty = [];
  for (let r = 0; r < SIZE; r++) {
    for (let c = 0; c < SIZE; c++) {
      if (board[r][c] === 0) empty.push([r, c]);
    }
  }
  if (empty.length === 0) return false;
  const [r, c] = empty[randomBelow(empty.length)];
  board[r][c] = randomBelow(10) === 0 ? 4 : 2;
  return true;
}

function newGame(randomBelow = defaultRandomBelow) {
  const game = {
    board: Array.from({ length: SIZE }, () => Array(SIZE).fill(0)),
    score: 0,
    keepPlaying: false,
  };
  spawnTile(game.board, randomBelow);
  spawnTile(game.board, randomBelow);
  return game;
}

function canMove(board) {
  for (let r = 0; r < SIZE; r++) {
    for (let c = 0; c < SIZE; c++) {
      const value = board[r][c];
      if (value === 0) return true;
      if (r + 1 < SIZE && board[r + 1][c] === value) return true;
      if (c + 1 < SIZE && board[r][c + 1] === value) return true;
    }
  }
  return false;
}

function gameStatus(game) {
  const reached2048 = game.board.some((row) => row.some((value) => value >= 2048));
  if (reached2048 && !game.keepPlaying) return WON;
  if (!canMove(game.board)) return LOST;
  return PLAYING;
}

/* Slides in a direction. Returns true if the board changed (then a new tile appears). */
function move(game, direction, randomBelow = defaultRandomBelow) {
  if (gameStatus(game) !== PLAYING) return false;
  const before = JSON.stringify(game.board);
  game.score += slideBoard(game.board, direction);
  if (JSON.stringify(game.board) === before) return false;
  spawnTile(game.board, randomBelow);
  return true;
}

if (typeof module !== 'undefined') {
  module.exports = {
    PLAYING, WON, LOST, slideRow, slideBoard, spawnTile, newGame, canMove, gameStatus, move,
  };
}
