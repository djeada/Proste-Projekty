const boardElement = document.getElementById('board');
const scoreElement = document.getElementById('score');
const bestElement = document.getElementById('best');
const messageElement = document.getElementById('message');
const newGameBtn = document.getElementById('new-game');

const SIZE = 4;
let board = [];
let score = 0;
let bestScore = parseInt(localStorage.getItem('best2048')) || 0;
let gameOver = false;
let won = false;

function init() {
  board = Array(SIZE).fill(null).map(() => Array(SIZE).fill(0));
  score = 0;
  gameOver = false;
  won = false;
  
  scoreElement.textContent = '0';
  bestElement.textContent = bestScore;
  messageElement.textContent = '';
  messageElement.className = 'message';
  
  spawnRandom();
  spawnRandom();
  renderBoard();
}

function spawnRandom() {
  const emptyCells = [];
  for (let row = 0; row < SIZE; row++) {
    for (let col = 0; col < SIZE; col++) {
      if (board[row][col] === 0) {
        emptyCells.push({ row, col });
      }
    }
  }
  
  if (emptyCells.length === 0) return;
  
  const { row, col } = emptyCells[Math.floor(Math.random() * emptyCells.length)];
  board[row][col] = Math.random() < 0.9 ? 2 : 4;
}

function renderBoard() {
  boardElement.innerHTML = '';
  
  for (let row = 0; row < SIZE; row++) {
    for (let col = 0; col < SIZE; col++) {
      const value = board[row][col];
      const tile = document.createElement('div');
      tile.className = `tile tile-${value > 2048 ? 'super' : value}`;
      tile.textContent = value || '';
      boardElement.appendChild(tile);
    }
  }
}

function slideAndMergeLine(line) {
  // Remove zeros
  let filtered = line.filter(x => x !== 0);
  
  // Merge adjacent equal values
  let merged = [];
  let scoreGained = 0;
  
  for (let i = 0; i < filtered.length; i++) {
    if (i + 1 < filtered.length && filtered[i] === filtered[i + 1]) {
      const newVal = filtered[i] * 2;
      merged.push(newVal);
      scoreGained += newVal;
      i++; // Skip next
    } else {
      merged.push(filtered[i]);
    }
  }
  
  // Pad with zeros
  while (merged.length < SIZE) {
    merged.push(0);
  }
  
  return { line: merged, score: scoreGained };
}

function move(direction) {
  if (gameOver) return;
  
  let moved = false;
  
  if (direction === 'left') {
    for (let row = 0; row < SIZE; row++) {
      const line = board[row].slice();
      const result = slideAndMergeLine(line);
      if (JSON.stringify(board[row]) !== JSON.stringify(result.line)) {
        moved = true;
      }
      board[row] = result.line;
      score += result.score;
    }
  } else if (direction === 'right') {
    for (let row = 0; row < SIZE; row++) {
      const line = board[row].slice().reverse();
      const result = slideAndMergeLine(line);
      if (JSON.stringify(board[row]) !== JSON.stringify(result.line.reverse())) {
        moved = true;
        board[row] = result.line;
      }
      score += result.score;
    }
  } else if (direction === 'up') {
    for (let col = 0; col < SIZE; col++) {
      const line = [];
      for (let row = 0; row < SIZE; row++) {
        line.push(board[row][col]);
      }
      const result = slideAndMergeLine(line);
      for (let row = 0; row < SIZE; row++) {
        if (board[row][col] !== result.line[row]) {
          moved = true;
        }
        board[row][col] = result.line[row];
      }
      score += result.score;
    }
  } else if (direction === 'down') {
    for (let col = 0; col < SIZE; col++) {
      const line = [];
      for (let row = SIZE - 1; row >= 0; row--) {
        line.push(board[row][col]);
      }
      const result = slideAndMergeLine(line);
      for (let row = SIZE - 1; row >= 0; row--) {
        if (board[row][col] !== result.line[SIZE - 1 - row]) {
          moved = true;
        }
        board[row][col] = result.line[SIZE - 1 - row];
      }
      score += result.score;
    }
  }
  
  if (moved) {
    spawnRandom();
    scoreElement.textContent = score;
    
    if (score > bestScore) {
      bestScore = score;
      bestElement.textContent = bestScore;
      localStorage.setItem('best2048', bestScore);
    }
    
    renderBoard();
    checkGameState();
  }
}

function checkGameState() {
  // Check for 2048 tile
  for (let row = 0; row < SIZE; row++) {
    for (let col = 0; col < SIZE; col++) {
      if (board[row][col] >= 2048 && !won) {
        won = true;
        messageElement.textContent = 'You Win! 🎉';
        messageElement.className = 'message win';
      }
    }
  }
  
  // Check if can move
  if (!canMove()) {
    gameOver = true;
    if (!won) {
      messageElement.textContent = 'Game Over!';
      messageElement.className = 'message lose';
    }
  }
}

function canMove() {
  // Check for empty cells
  for (let row = 0; row < SIZE; row++) {
    for (let col = 0; col < SIZE; col++) {
      if (board[row][col] === 0) return true;
    }
  }
  
  // Check for adjacent equal values
  for (let row = 0; row < SIZE; row++) {
    for (let col = 0; col < SIZE; col++) {
      const val = board[row][col];
      if (col + 1 < SIZE && board[row][col + 1] === val) return true;
      if (row + 1 < SIZE && board[row + 1][col] === val) return true;
    }
  }
  
  return false;
}

document.addEventListener('keydown', (e) => {
  switch (e.key) {
    case 'ArrowUp':
      e.preventDefault();
      move('up');
      break;
    case 'ArrowDown':
      e.preventDefault();
      move('down');
      break;
    case 'ArrowLeft':
      e.preventDefault();
      move('left');
      break;
    case 'ArrowRight':
      e.preventDefault();
      move('right');
      break;
  }
});

// Touch support
let touchStartX = 0;
let touchStartY = 0;

document.addEventListener('touchstart', (e) => {
  touchStartX = e.touches[0].clientX;
  touchStartY = e.touches[0].clientY;
});

document.addEventListener('touchend', (e) => {
  const touchEndX = e.changedTouches[0].clientX;
  const touchEndY = e.changedTouches[0].clientY;
  
  const dx = touchEndX - touchStartX;
  const dy = touchEndY - touchStartY;
  
  if (Math.abs(dx) > Math.abs(dy)) {
    if (dx > 50) move('right');
    else if (dx < -50) move('left');
  } else {
    if (dy > 50) move('down');
    else if (dy < -50) move('up');
  }
});

newGameBtn.addEventListener('click', init);

init();
