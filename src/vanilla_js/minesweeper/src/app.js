const boardElement = document.getElementById('board');
const minesLeftElement = document.getElementById('mines-left');
const timerElement = document.getElementById('timer');
const messageElement = document.getElementById('message');
const newGameBtn = document.getElementById('new-game');
const rowsInput = document.getElementById('rows');
const colsInput = document.getElementById('cols');
const minesInput = document.getElementById('mines');

let rows = 10;
let cols = 10;
let numMines = 10;
let board = [];
let revealed = [];
let flagged = [];
let gameOver = false;
let gameStarted = false;
let timer = 0;
let timerInterval = null;

function init() {
  rows = parseInt(rowsInput.value) || 10;
  cols = parseInt(colsInput.value) || 10;
  numMines = Math.min(parseInt(minesInput.value) || 10, rows * cols - 1);
  
  board = Array(rows).fill(null).map(() => Array(cols).fill(0));
  revealed = Array(rows).fill(null).map(() => Array(cols).fill(false));
  flagged = Array(rows).fill(null).map(() => Array(cols).fill(false));
  gameOver = false;
  gameStarted = false;
  timer = 0;
  
  clearInterval(timerInterval);
  timerElement.textContent = '0';
  minesLeftElement.textContent = numMines;
  messageElement.textContent = '';
  messageElement.className = 'message';
  newGameBtn.textContent = '🙂';
  
  placeMines();
  calculateNumbers();
  renderBoard();
}

function placeMines() {
  let placed = 0;
  while (placed < numMines) {
    const row = Math.floor(Math.random() * rows);
    const col = Math.floor(Math.random() * cols);
    if (board[row][col] !== -1) {
      board[row][col] = -1;
      placed++;
    }
  }
}

function calculateNumbers() {
  for (let row = 0; row < rows; row++) {
    for (let col = 0; col < cols; col++) {
      if (board[row][col] === -1) continue;
      board[row][col] = countAdjacentMines(row, col);
    }
  }
}

function countAdjacentMines(row, col) {
  let count = 0;
  for (let i = -1; i <= 1; i++) {
    for (let j = -1; j <= 1; j++) {
      const r = row + i;
      const c = col + j;
      if (r >= 0 && r < rows && c >= 0 && c < cols && board[r][c] === -1) {
        count++;
      }
    }
  }
  return count;
}

function renderBoard() {
  boardElement.innerHTML = '';
  boardElement.style.gridTemplateColumns = `repeat(${cols}, 30px)`;
  
  for (let row = 0; row < rows; row++) {
    for (let col = 0; col < cols; col++) {
      const cell = document.createElement('div');
      cell.className = 'cell';
      cell.dataset.row = row;
      cell.dataset.col = col;
      
      if (revealed[row][col]) {
        cell.classList.add('revealed');
        if (board[row][col] === -1) {
          cell.classList.add('mine');
          cell.textContent = '💣';
        } else if (board[row][col] > 0) {
          cell.textContent = board[row][col];
          cell.dataset.value = board[row][col];
        }
      } else if (flagged[row][col]) {
        cell.classList.add('flagged');
        cell.textContent = '🚩';
      }
      
      cell.addEventListener('click', () => revealCell(row, col));
      cell.addEventListener('contextmenu', (e) => {
        e.preventDefault();
        toggleFlag(row, col);
      });
      
      boardElement.appendChild(cell);
    }
  }
}

function revealCell(row, col) {
  if (gameOver || revealed[row][col] || flagged[row][col]) return;
  
  if (!gameStarted) {
    gameStarted = true;
    timerInterval = setInterval(() => {
      timer++;
      timerElement.textContent = timer;
    }, 1000);
  }
  
  revealed[row][col] = true;
  
  if (board[row][col] === -1) {
    gameOver = true;
    clearInterval(timerInterval);
    revealAllMines();
    messageElement.textContent = 'Game Over! 💥';
    messageElement.className = 'message lose';
    newGameBtn.textContent = '😵';
    renderBoard();
    return;
  }
  
  if (board[row][col] === 0) {
    revealAdjacentCells(row, col);
  }
  
  renderBoard();
  checkWin();
}

function revealAdjacentCells(row, col) {
  for (let i = -1; i <= 1; i++) {
    for (let j = -1; j <= 1; j++) {
      const r = row + i;
      const c = col + j;
      if (r >= 0 && r < rows && c >= 0 && c < cols && !revealed[r][c] && !flagged[r][c]) {
        revealed[r][c] = true;
        if (board[r][c] === 0) {
          revealAdjacentCells(r, c);
        }
      }
    }
  }
}

function toggleFlag(row, col) {
  if (gameOver || revealed[row][col]) return;
  
  flagged[row][col] = !flagged[row][col];
  const flagCount = flagged.flat().filter(Boolean).length;
  minesLeftElement.textContent = numMines - flagCount;
  renderBoard();
}

function revealAllMines() {
  for (let row = 0; row < rows; row++) {
    for (let col = 0; col < cols; col++) {
      if (board[row][col] === -1) {
        revealed[row][col] = true;
      }
    }
  }
}

function checkWin() {
  let unrevealedSafeCells = 0;
  for (let row = 0; row < rows; row++) {
    for (let col = 0; col < cols; col++) {
      if (board[row][col] !== -1 && !revealed[row][col]) {
        unrevealedSafeCells++;
      }
    }
  }
  
  if (unrevealedSafeCells === 0) {
    gameOver = true;
    clearInterval(timerInterval);
    messageElement.textContent = 'You Win! 🎉';
    messageElement.className = 'message win';
    newGameBtn.textContent = '😎';
  }
}

newGameBtn.addEventListener('click', init);
rowsInput.addEventListener('change', init);
colsInput.addEventListener('change', init);
minesInput.addEventListener('change', init);

init();
