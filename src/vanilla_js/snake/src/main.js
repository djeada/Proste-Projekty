// The page: drawing on the canvas, keyboard and buttons.
const CELL = 24;
const canvas = document.getElementById('canvas');
const ctx = canvas.getContext('2d');
const scoreElement = document.getElementById('score');
const messageElement = document.getElementById('message');
const newGameButton = document.getElementById('new-game');

const KEY_DIRECTIONS = {
  ArrowUp: UP, w: UP, W: UP,
  ArrowDown: DOWN, s: DOWN, S: DOWN,
  ArrowLeft: LEFT, a: LEFT, A: LEFT,
  ArrowRight: RIGHT, d: RIGHT, D: RIGHT,
};

let game = null;
let paused = false;
let timer = null;

function draw() {
  ctx.fillStyle = '#2d2d2d';
  ctx.fillRect(0, 0, canvas.width, canvas.height);
  if (game.food) {
    ctx.fillStyle = '#f44336';
    ctx.fillRect(game.food.x * CELL + 2, game.food.y * CELL + 2, CELL - 4, CELL - 4);
  }
  game.body.forEach((part, i) => {
    ctx.fillStyle = i === 0 ? '#00b8d4' : '#009dc1';
    ctx.fillRect(part.x * CELL + 1, part.y * CELL + 1, CELL - 2, CELL - 2);
  });
  scoreElement.textContent = String(game.score);
  if (game.gameOver) {
    messageElement.textContent = 'Game over! Press R or Space to restart.';
    messageElement.className = 'message lose';
  } else if (paused) {
    messageElement.textContent = 'Paused. Press P to continue.';
    messageElement.className = 'message';
  } else {
    messageElement.textContent = '';
    messageElement.className = 'message';
  }
}

function scheduleStep() {
  clearTimeout(timer);
  timer = setTimeout(tick, game.delayMs());
}

function tick() {
  if (!paused) {
    game.step();
  }
  draw();
  if (!game.gameOver) {
    scheduleStep();
  }
}

function newGame() {
  game = new SnakeGame();
  paused = false;
  draw();
  scheduleStep();
}

document.addEventListener('keydown', (event) => {
  const direction = KEY_DIRECTIONS[event.key];
  if (direction) {
    game.turn(direction);
  } else if (event.key === 'p' || event.key === 'P') {
    paused = !paused;
    draw();
  } else if ((event.key === 'r' || event.key === 'R' || event.key === ' ') && game.gameOver) {
    newGame();
  } else {
    return;
  }
  event.preventDefault();
});

newGameButton.addEventListener('click', newGame);
newGame();
