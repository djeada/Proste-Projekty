// Rules of tic-tac-toe and the minimax search for the computer player.

const LINES = [
  [0, 1, 2], [3, 4, 5], [6, 7, 8],
  [0, 3, 6], [1, 4, 7], [2, 5, 8],
  [0, 4, 8], [2, 4, 6],
];

function newBoard() {
  return Array(9).fill(null);
}

function otherMark(mark) {
  return mark === 'X' ? 'O' : 'X';
}

function legalMoves(board) {
  const moves = [];
  for (let cell = 0; cell < 9; cell++) {
    if (board[cell] === null) moves.push(cell);
  }
  return moves;
}

// Returns a new board with mark on cell. Throws if the cell is taken.
function play(board, cell, mark) {
  if (!legalMoves(board).includes(cell)) throw new Error(`cell ${cell} is not empty`);
  const result = board.slice();
  result[cell] = mark;
  return result;
}

// Returns the three cells of a complete line, or null.
function winningLine(board) {
  for (const line of LINES) {
    const [a, b, c] = line;
    if (board[a] !== null && board[a] === board[b] && board[a] === board[c]) return line;
  }
  return null;
}

// Returns 'X', 'O' or null.
function winner(board) {
  const line = winningLine(board);
  return line ? board[line[0]] : null;
}

function isFull(board) {
  return !board.includes(null);
}

function isOver(board) {
  return winningLine(board) !== null || isFull(board);
}

function isDraw(board) {
  return winningLine(board) === null && isFull(board);
}

// Score for `me` when `turn` is to move: +10 for a win, -10 for a loss, 0 for a draw.
// The depth is subtracted from a win, so the quickest win scores highest and the slowest
// loss scores least badly.
function minimax(board, turn, me, depth) {
  const line = winningLine(board);
  if (line) return board[line[0]] === me ? 10 - depth : depth - 10;
  const moves = legalMoves(board);
  if (moves.length === 0) return 0;

  let best = turn === me ? -Infinity : Infinity;
  for (const cell of moves) {
    const score = minimax(play(board, cell, turn), otherMark(turn), me, depth + 1);
    best = turn === me ? Math.max(best, score) : Math.min(best, score);
  }
  return best;
}

// Returns the cell that gives mark the best result with minimax, or null if the board is full.
function bestMove(board, mark) {
  let bestCell = null;
  let bestScore = -Infinity;
  for (const cell of legalMoves(board)) {
    const score = minimax(play(board, cell, mark), otherMark(mark), mark, 1);
    if (bestCell === null || score > bestScore) {
      bestScore = score;
      bestCell = cell;
    }
  }
  return bestCell;
}

if (typeof module !== 'undefined') {
  module.exports = {
    newBoard, otherMark, legalMoves, play, winningLine, winner,
    isFull, isOver, isDraw, bestMove,
  };
}
