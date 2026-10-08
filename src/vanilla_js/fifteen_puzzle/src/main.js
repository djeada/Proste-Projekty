/* Browser interface for the fifteen puzzle: draws the board, handles clicks and arrow keys. */
const boardElement = document.getElementById('board');
const movesElement = document.getElementById('moves');
const messageElement = document.getElementById('message');
const newGameButton = document.getElementById('new-game');
const ARROW_KEYS = { ArrowUp: 'up', ArrowDown: 'down', ArrowLeft: 'left', ArrowRight: 'right' };

let board = solvedBoard();
let moves = 0;

function play(move) {
  if (!isSolved(board) && move()) moves++;
  render();
}

function render() {
  boardElement.innerHTML = '';
  board.forEach((tile, cell) => {
    const button = document.createElement('button');
    button.className = tile === 0 ? 'tile gap' : 'tile';
    button.textContent = tile === 0 ? '' : String(tile);
    button.disabled = tile === 0;
    button.addEventListener('click', () => play(() => slide(board, cell)));
    boardElement.appendChild(button);
  });
  movesElement.textContent = `Moves: ${moves}`;
  messageElement.textContent = isSolved(board) ? `Solved in ${moves} moves! Press New game to play again.` : '';
}

function newGame() {
  shuffle(board);
  moves = 0;
  render();
}

document.addEventListener('keydown', (event) => {
  const direction = ARROW_KEYS[event.key];
  if (!direction) return;
  event.preventDefault();
  play(() => moveGap(board, direction));
});
newGameButton.addEventListener('click', newGame);

newGame();
