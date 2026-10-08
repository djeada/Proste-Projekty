/* Page for 2048: draws the board and turns key presses and button clicks into moves. */
const boardElement = document.getElementById('board');
const scoreElement = document.getElementById('score');
const messageElement = document.getElementById('message');
const newGameButton = document.getElementById('new-game');

const KEY_DIRECTIONS = {
  arrowup: 'up', w: 'up',
  arrowdown: 'down', s: 'down',
  arrowleft: 'left', a: 'left',
  arrowright: 'right', d: 'right',
};
const MESSAGES = {
  won: 'You reached 2048! Press C to keep playing or R for a new game.',
  lost: 'Game over! Press R to play again.',
};

let game = newGame();

function render() {
  scoreElement.textContent = game.score;
  boardElement.innerHTML = '';
  for (const row of game.board) {
    for (const value of row) {
      const cell = document.createElement('div');
      cell.className = 'cell';
      if (value !== 0) {
        const tile = document.createElement('div');
        tile.className = `tile tile-${value}`;
        tile.textContent = value;
        cell.appendChild(tile);
      }
      boardElement.appendChild(cell);
    }
  }
  messageElement.textContent = MESSAGES[gameStatus(game)] || '';
}

function restart() {
  game = newGame();
  render();
}

document.addEventListener('keydown', (event) => {
  const key = event.key.toLowerCase();
  const direction = KEY_DIRECTIONS[key];
  if (direction) {
    event.preventDefault();
    move(game, direction);
  } else if (key === 'r') {
    restart();
    return;
  } else if (key === 'c') {
    game.keepPlaying = true;
  } else {
    return;
  }
  render();
});

newGameButton.addEventListener('click', restart);
render();
