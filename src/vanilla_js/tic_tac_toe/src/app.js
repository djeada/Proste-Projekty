const cells = document.querySelectorAll('.cell');
const message = document.getElementById('message');
const newGameBtn = document.getElementById('new-game');
const resetScoresBtn = document.getElementById('reset-scores');
const twoPlayersBtn = document.getElementById('two-players');
const vsComputerBtn = document.getElementById('vs-computer');
const scoreX = document.getElementById('score-x');
const scoreO = document.getElementById('score-o');
const scoreDraw = document.getElementById('score-draw');

const PLAYER_X = 'X';
const PLAYER_O = 'O';
const EMPTY = '';

const winningCombinations = [
  [0, 1, 2], [3, 4, 5], [6, 7, 8], // rows
  [0, 3, 6], [1, 4, 7], [2, 5, 8], // columns
  [0, 4, 8], [2, 4, 6]             // diagonals
];

let board = Array(9).fill(EMPTY);
let currentPlayer = PLAYER_X;
let gameOver = false;
let vsComputer = false;
let scores = { X: 0, O: 0, draw: 0 };

function init() {
  board = Array(9).fill(EMPTY);
  currentPlayer = PLAYER_X;
  gameOver = false;
  
  cells.forEach(cell => {
    cell.textContent = '';
    cell.className = 'cell';
  });
  
  message.textContent = `Player ${currentPlayer}'s turn`;
  message.className = 'message';
}

function makeMove(index) {
  if (board[index] !== EMPTY || gameOver) return false;
  
  board[index] = currentPlayer;
  cells[index].textContent = currentPlayer;
  cells[index].classList.add('taken', currentPlayer.toLowerCase());
  
  return true;
}

function checkWinner() {
  for (const combo of winningCombinations) {
    const [a, b, c] = combo;
    if (board[a] !== EMPTY && board[a] === board[b] && board[b] === board[c]) {
      return { winner: board[a], line: combo };
    }
  }
  return null;
}

function isBoardFull() {
  return board.every(cell => cell !== EMPTY);
}

function handleCellClick(e) {
  const index = parseInt(e.target.dataset.index);
  
  if (!makeMove(index)) return;
  
  const result = checkWinner();
  
  if (result) {
    gameOver = true;
    message.textContent = `Player ${result.winner} wins!`;
    message.className = 'message win';
    result.line.forEach(i => cells[i].classList.add('winning'));
    scores[result.winner]++;
    updateScoreboard();
    return;
  }
  
  if (isBoardFull()) {
    gameOver = true;
    message.textContent = "It's a draw!";
    message.className = 'message draw';
    scores.draw++;
    updateScoreboard();
    return;
  }
  
  currentPlayer = currentPlayer === PLAYER_X ? PLAYER_O : PLAYER_X;
  message.textContent = `Player ${currentPlayer}'s turn`;
  
  if (vsComputer && currentPlayer === PLAYER_O && !gameOver) {
    setTimeout(computerMove, 500);
  }
}

function computerMove() {
  const emptyCells = [];
  board.forEach((cell, index) => {
    if (cell === EMPTY) emptyCells.push(index);
  });
  
  if (emptyCells.length === 0) return;
  
  const randomIndex = emptyCells[Math.floor(Math.random() * emptyCells.length)];
  
  makeMove(randomIndex);
  
  const result = checkWinner();
  
  if (result) {
    gameOver = true;
    message.textContent = `Player ${result.winner} wins!`;
    message.className = 'message win';
    result.line.forEach(i => cells[i].classList.add('winning'));
    scores[result.winner]++;
    updateScoreboard();
    return;
  }
  
  if (isBoardFull()) {
    gameOver = true;
    message.textContent = "It's a draw!";
    message.className = 'message draw';
    scores.draw++;
    updateScoreboard();
    return;
  }
  
  currentPlayer = PLAYER_X;
  message.textContent = `Player ${currentPlayer}'s turn`;
}

function updateScoreboard() {
  scoreX.textContent = scores.X;
  scoreO.textContent = scores.O;
  scoreDraw.textContent = scores.draw;
}

function resetScores() {
  scores = { X: 0, O: 0, draw: 0 };
  updateScoreboard();
  init();
}

function setTwoPlayers() {
  vsComputer = false;
  twoPlayersBtn.classList.add('active');
  vsComputerBtn.classList.remove('active');
  init();
}

function setVsComputer() {
  vsComputer = true;
  vsComputerBtn.classList.add('active');
  twoPlayersBtn.classList.remove('active');
  init();
}

cells.forEach(cell => cell.addEventListener('click', handleCellClick));
newGameBtn.addEventListener('click', init);
resetScoresBtn.addEventListener('click', resetScores);
twoPlayersBtn.addEventListener('click', setTwoPlayers);
vsComputerBtn.addEventListener('click', setVsComputer);

init();
