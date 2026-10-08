// Browser interface: level choice, board of buttons, mine counter, timer. Rules come from minesweeper.js.

const LEVELS = {
  beginner: [9, 9, 10],
  intermediate: [16, 16, 40],
  expert: [16, 30, 99],
};

const MESSAGES = {
  won: 'You won! Every safe cell is open.',
  lost: 'Boom! You hit a mine.',
};

const levelSelect = document.getElementById('level');
const newGameButton = document.getElementById('new-game');
const infoElement = document.getElementById('info');
const messageElement = document.getElementById('message');
const boardElement = document.getElementById('board');

let game;
let seconds = 0;
let timerId = null;
let cells = [];

function newGame() {
  const [rows, cols, mines] = LEVELS[levelSelect.value];
  game = new Game(rows, cols, mines);
  seconds = 0;
  clearInterval(timerId);
  timerId = setInterval(tick, 1000);
  buildBoard(rows, cols);
  render();
}

function buildBoard(rows, cols) {
  boardElement.style.gridTemplateColumns = `repeat(${cols}, 28px)`;
  boardElement.replaceChildren();
  cells = [];
  for (let row = 0; row < rows; row++) {
    cells.push([]);
    for (let col = 0; col < cols; col++) {
      const cell = document.createElement('button');
      cell.type = 'button';
      cell.className = 'cell';
      cell.addEventListener('click', () => {
        game.reveal(row, col);
        render();
      });
      cell.addEventListener('contextmenu', (event) => {
        event.preventDefault();
        game.toggleFlag(row, col);
        render();
      });
      boardElement.append(cell);
      cells[row].push(cell);
    }
  }
}

function tick() {
  if (game.state !== 'playing') return;
  seconds++;
  render();
}

function render() {
  infoElement.textContent = `Mines left: ${game.minesLeft} | Time: ${seconds} s`;
  messageElement.textContent = MESSAGES[game.state] || '';
  for (let row = 0; row < game.rows; row++) {
    for (let col = 0; col < game.cols; col++) {
      renderCell(cells[row][col], row, col);
    }
  }
  if (game.state !== 'playing') clearInterval(timerId);
}

function renderCell(cell, row, col) {
  cell.className = 'cell';
  cell.textContent = '';
  delete cell.dataset.number;
  if (game.revealed[row][col]) {
    cell.classList.add('revealed');
    if (game.mine[row][col]) {
      cell.classList.add('mine');
      cell.textContent = '*';
    } else if (game.neighbors[row][col] > 0) {
      cell.textContent = game.neighbors[row][col];
      cell.dataset.number = game.neighbors[row][col];
    }
  } else if (game.flagged[row][col]) {
    cell.classList.add('flagged');
    cell.textContent = 'F';
  }
}

levelSelect.addEventListener('change', newGame);
newGameButton.addEventListener('click', newGame);
newGame();
