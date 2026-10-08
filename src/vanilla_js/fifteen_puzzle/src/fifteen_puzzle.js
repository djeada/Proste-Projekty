/* Fifteen puzzle rules: moves, shuffle, solved and solvable checks. */
const PUZZLE_SIZE = 4;
const SHUFFLE_MOVES = 500;
const DIRECTIONS = { up: [-1, 0], down: [1, 0], left: [0, -1], right: [0, 1] };

function solvedBoard() {
  const board = [];
  for (let tile = 1; tile < PUZZLE_SIZE * PUZZLE_SIZE; tile++) board.push(tile);
  board.push(0);
  return board;
}

function gapIndex(board) {
  return board.indexOf(0);
}

/* The tile in `cell` slides into the gap if they are next to each other. */
function slide(board, cell) {
  const gap = gapIndex(board);
  if (cell < 0 || cell >= board.length || cell === gap) return false;
  const distance = Math.abs(Math.floor(cell / PUZZLE_SIZE) - Math.floor(gap / PUZZLE_SIZE))
    + Math.abs(cell % PUZZLE_SIZE - gap % PUZZLE_SIZE);
  if (distance !== 1) return false;
  board[gap] = board[cell];
  board[cell] = 0;
  return true;
}

/* The gap moves in `direction`: the tile on that side slides into it. */
function moveGap(board, direction) {
  const [dRow, dCol] = DIRECTIONS[direction];
  const gap = gapIndex(board);
  const row = Math.floor(gap / PUZZLE_SIZE) + dRow;
  const col = gap % PUZZLE_SIZE + dCol;
  if (row < 0 || row >= PUZZLE_SIZE || col < 0 || col >= PUZZLE_SIZE) return false;
  return slide(board, row * PUZZLE_SIZE + col);
}

/* Random legal moves from the solved board, so the result is always solvable. */
function shuffle(board, random = Math.random) {
  const names = Object.keys(DIRECTIONS);
  do {
    board.splice(0, board.length, ...solvedBoard());
    let moves = 0;
    while (moves < SHUFFLE_MOVES) {
      if (moveGap(board, names[Math.floor(random() * names.length)])) moves++;
    }
  } while (isSolved(board));
  return board;
}

function isSolved(board) {
  return board.join() === solvedBoard().join();
}

/*
 * On a board with an even width: solvable when inversions (tiles out of order,
 * the gap ignored) plus the gap's row from the top (0-based) is odd.
 */
function isSolvable(board) {
  let inversions = 0;
  for (let i = 0; i < board.length; i++) {
    for (let j = i + 1; j < board.length; j++) {
      if (board[i] !== 0 && board[j] !== 0 && board[i] > board[j]) inversions++;
    }
  }
  return (inversions + Math.floor(gapIndex(board) / PUZZLE_SIZE)) % 2 === 1;
}

if (typeof module !== 'undefined') {
  module.exports = { solvedBoard, gapIndex, slide, moveGap, shuffle, isSolved, isSolvable };
}
