// Browser user interface: draws the sky, grass and ducks on a canvas and reads the mouse and keys.
const canvas = document.getElementById('game');
const ctx = canvas.getContext('2d');
const WIDTH = canvas.width;
const HEIGHT = canvas.height;
const GROUND_Y = Math.round(HEIGHT * 0.85);

let game = createGame(Date.now());
let pointer = null;
let lastTime = null;

function toPixels(x, y) {
  return { px: x * WIDTH / FIELD_WIDTH, py: y * HEIGHT / FIELD_HEIGHT };
}

function createBackground() {
  const background = document.createElement('canvas');
  background.width = WIDTH;
  background.height = HEIGHT;
  const bg = background.getContext('2d');

  const sky = bg.createLinearGradient(0, 0, 0, GROUND_Y);
  sky.addColorStop(0, '#468cdc');
  sky.addColorStop(1, '#c8e6fa');
  bg.fillStyle = sky;
  bg.fillRect(0, 0, WIDTH, GROUND_Y);

  bg.fillStyle = '#ffffff';
  for (const [cx, cy] of [[120, 70], [400, 45], [660, 90]]) {
    for (const [dx, r] of [[-18, 16], [0, 22], [20, 16]]) {
      bg.beginPath();
      bg.arc(cx + dx, cy, r, 0, 2 * Math.PI);
      bg.fill();
    }
  }

  const grass = bg.createLinearGradient(0, GROUND_Y, 0, HEIGHT);
  grass.addColorStop(0, '#4ca03c');
  grass.addColorStop(1, '#388028');
  bg.fillStyle = grass;
  bg.fillRect(0, GROUND_Y, WIDTH, HEIGHT - GROUND_Y);

  bg.fillStyle = '#38801f';
  for (let x = 0; x < WIDTH; x += 14) {
    bg.beginPath();
    bg.moveTo(x, GROUND_Y);
    bg.lineTo(x + 7, GROUND_Y - 10);
    bg.lineTo(x + 14, GROUND_Y);
    bg.fill();
  }
  return background;
}

const background = createBackground();

function drawDuck(duck) {
  const { px, py } = toPixels(duck.x, duck.y);
  const direction = duck.speed > 0 ? 1 : -1;

  ctx.fillStyle = '#ffd228';
  ctx.beginPath();
  ctx.ellipse(px, py + 6, 30, 14, 0, 0, 2 * Math.PI);
  ctx.fill();

  const headX = px + direction * 20;
  const headY = py - 6;
  ctx.beginPath();
  ctx.arc(headX, headY, 13, 0, 2 * Math.PI);
  ctx.fill();

  ctx.fillStyle = '#fa781e';
  ctx.beginPath();
  ctx.moveTo(headX + direction * 10, headY - 3);
  ctx.lineTo(headX + direction * 24, headY + 1);
  ctx.lineTo(headX + direction * 10, headY + 5);
  ctx.fill();

  ctx.fillStyle = '#141420';
  ctx.beginPath();
  ctx.arc(headX + direction * 5, headY - 3, 3, 0, 2 * Math.PI);
  ctx.fill();

  ctx.fillStyle = '#e68c1e';
  ctx.beginPath();
  ctx.ellipse(px - direction * 4, py + 3, 15, 8, 0, 0, 2 * Math.PI);
  ctx.fill();
}

function drawText(text, x, y, font, color) {
  ctx.font = font;
  ctx.fillStyle = color;
  ctx.textAlign = 'center';
  ctx.textBaseline = 'middle';
  ctx.fillText(text, x, y);
}

function drawHud() {
  ctx.textAlign = 'left';
  ctx.textBaseline = 'middle';
  ctx.font = 'bold 20px sans-serif';
  ctx.fillStyle = '#141420';
  ctx.fillText(`Wave ${game.wave}   Score ${game.score}`, 16, 24);

  for (let i = 0; i < START_LIVES; i++) {
    ctx.fillStyle = i < game.lives ? '#dc3232' : '#969696';
    ctx.beginPath();
    ctx.arc(WIDTH - 30 - i * 26, 24, 9, 0, 2 * Math.PI);
    ctx.fill();
  }
}

function render() {
  ctx.drawImage(background, 0, 0);
  for (const duck of game.ducks) drawDuck(duck);
  drawHud();

  if (game.gameOver) {
    ctx.fillStyle = 'rgba(0, 0, 0, 0.6)';
    ctx.fillRect(0, 0, WIDTH, HEIGHT);
    drawText('GAME OVER', WIDTH / 2, HEIGHT / 2 - 50, 'bold 56px sans-serif', '#ffffff');
    drawText(`Score: ${game.score}`, WIDTH / 2, HEIGHT / 2, 'bold 30px sans-serif', '#ffffff');
    drawText('Click or press R to play again', WIDTH / 2, HEIGHT / 2 + 40, '20px sans-serif', '#ffffff');
    return;
  }

  if (game.breakTimer > 0) {
    drawText(`Wave ${game.wave} cleared!`, WIDTH / 2, HEIGHT / 2 - 60, 'bold 40px sans-serif', '#141420');
  }
  if (pointer) {
    ctx.strokeStyle = '#dc3232';
    ctx.lineWidth = 2;
    ctx.beginPath();
    ctx.arc(pointer.x, pointer.y, 12, 0, 2 * Math.PI);
    ctx.moveTo(pointer.x - 18, pointer.y);
    ctx.lineTo(pointer.x + 18, pointer.y);
    ctx.moveTo(pointer.x, pointer.y - 18);
    ctx.lineTo(pointer.x, pointer.y + 18);
    ctx.stroke();
  }
}

// Converts a mouse event to canvas pixels and to field coordinates.
function canvasPoint(event) {
  const rect = canvas.getBoundingClientRect();
  const px = (event.clientX - rect.left) * WIDTH / rect.width;
  const py = (event.clientY - rect.top) * HEIGHT / rect.height;
  return { px, py, fx: px * FIELD_WIDTH / WIDTH, fy: py * FIELD_HEIGHT / HEIGHT };
}

function restart() {
  game = createGame(Date.now());
}

canvas.addEventListener('mousemove', (event) => {
  const { px, py } = canvasPoint(event);
  pointer = { x: px, y: py };
});

canvas.addEventListener('mouseleave', () => {
  pointer = null;
});

canvas.addEventListener('click', (event) => {
  if (game.gameOver) {
    restart();
    return;
  }
  const { fx, fy } = canvasPoint(event);
  shootAt(game, fx, fy);
});

document.addEventListener('keydown', (event) => {
  if ((event.key === 'r' || event.key === 'R') && game.gameOver) restart();
});

function frame(time) {
  const dt = lastTime === null ? 0 : Math.min((time - lastTime) / 1000, 0.1);
  lastTime = time;
  updateGame(game, dt);
  render();
  requestAnimationFrame(frame);
}

requestAnimationFrame(frame);
