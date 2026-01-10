const boardElement = document.getElementById('board');
const movesElement = document.getElementById('moves');
const messageElement = document.getElementById('message');
const newGameBtn = document.getElementById('new-game');

const SIZE = 4;
let board = [];
let moves = 0;
let gameWon = false;

function init() {
  // Create solved board
  board = [];
  for (let i = 1; i <= SIZE * SIZE - 1; i++) {
    board.push(i);
  }
  board.push(0); // Empty space
  
  // Shuffle the board
  shuffle();
  
  moves = 0;
  gameWon = false;
  movesElement.textContent = '0';
  messageElement.textContent = '';
  
  renderBoard();
}

function shuffle() {
  // Perform random moves to ensure solvability
  for (let i = 0; i < 200; i++) {
    const emptyIndex = board.indexOf(0);
    const possibleMoves = getMovableTiles(emptyIndex);
    const randomMove = possibleMoves[Math.floor(Math.random() * possibleMoves.length)];
    swap(emptyIndex, randomMove);
  }
}

function getMovableTiles(emptyIndex) {
  const movable = [];
  const row = Math.floor(emptyIndex / SIZE);
  const col = emptyIndex % SIZE;
  
  if (row > 0) movable.push(emptyIndex - SIZE); // Above
  if (row < SIZE - 1) movable.push(emptyIndex + SIZE); // Below
  if (col > 0) movable.push(emptyIndex - 1); // Left
  if (col < SIZE - 1) movable.push(emptyIndex + 1); // Right
  
  return movable;
}

function swap(i, j) {
  [board[i], board[j]] = [board[j], board[i]];
}

function renderBoard() {
  boardElement.innerHTML = '';
  const emptyIndex = board.indexOf(0);
  const movable = getMovableTiles(emptyIndex);
  
  for (let i = 0; i < board.length; i++) {
    const tile = document.createElement('div');
    tile.className = 'tile';
    
    if (board[i] === 0) {
      tile.classList.add('empty');
    } else {
      tile.textContent = board[i];
      
      // Check if tile is in correct position
      if (board[i] === i + 1) {
        tile.classList.add('correct');
      }
      
      // Check if tile can be moved
      if (movable.includes(i)) {
        tile.classList.add('movable');
        tile.addEventListener('click', () => moveTile(i));
      }
    }
    
    boardElement.appendChild(tile);
  }
}

function moveTile(tileIndex) {
  if (gameWon) return;
  
  const emptyIndex = board.indexOf(0);
  const movable = getMovableTiles(emptyIndex);
  
  if (movable.includes(tileIndex)) {
    swap(emptyIndex, tileIndex);
    moves++;
    movesElement.textContent = moves;
    renderBoard();
    checkWin();
  }
}

function checkWin() {
  for (let i = 0; i < SIZE * SIZE - 1; i++) {
    if (board[i] !== i + 1) return;
  }
  
  gameWon = true;
  messageElement.textContent = `Congratulations! Solved in ${moves} moves! 🎉`;
}

document.addEventListener('keydown', (e) => {
  if (gameWon) return;
  
  const emptyIndex = board.indexOf(0);
  const row = Math.floor(emptyIndex / SIZE);
  const col = emptyIndex % SIZE;
  let tileToMove = -1;
  
  switch (e.key) {
    case 'ArrowUp':
      if (row < SIZE - 1) tileToMove = emptyIndex + SIZE;
      break;
    case 'ArrowDown':
      if (row > 0) tileToMove = emptyIndex - SIZE;
      break;
    case 'ArrowLeft':
      if (col < SIZE - 1) tileToMove = emptyIndex + 1;
      break;
    case 'ArrowRight':
      if (col > 0) tileToMove = emptyIndex - 1;
      break;
  }
  
  if (tileToMove !== -1) {
    e.preventDefault();
    moveTile(tileToMove);
  }
});

newGameBtn.addEventListener('click', init);

init();
