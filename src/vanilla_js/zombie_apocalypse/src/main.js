// The browser interface of Zombie Apocalypse: reads keys and mouse, draws on the canvas.

const PIXELS_PER_UNIT = 20;
const HUD_HEIGHT = 36;
const COLORS = {
  background: '#222c22',
  grid: '#2c382c',
  hud: '#141814',
  player: '#4696f0',
  playerOutline: '#14325f',
  gun: '#9696a0',
  zombie: '#5aaa46',
  zombieOutline: '#1e501a',
  eye: '#ebf0c8',
  bullet: '#fff03c',
  pickup: '#fafafa',
  cross: '#dc2828',
  text: '#f0f0f0',
  healthBack: '#464646',
  healthBar: '#c83c3c',
};
const ZOMBIE_DRAW_RADIUS = 14;
const PLAYER_DRAW_RADIUS = 10;

const canvas = document.getElementById('game');
const ctx = canvas.getContext('2d');
canvas.width = WORLD_WIDTH * PIXELS_PER_UNIT;
canvas.height = HUD_HEIGHT + WORLD_HEIGHT * PIXELS_PER_UNIT;

let game = newGame(Math.floor(Math.random() * 2 ** 32));
let lastHealth = game.health;
let flash = 0; // seconds left of the red flash after a hit
let lastTime = performance.now();
const keys = new Set();
const mouse = { x: 0, y: HUD_HEIGHT }; // in canvas pixels
let clicked = false; // a click since the last frame fires one shot

function restart() {
  game = newGame(Math.floor(Math.random() * 2 ** 32));
  lastHealth = game.health;
}

function toScreen(x, y) {
  return { x: x * PIXELS_PER_UNIT, y: HUD_HEIGHT + y * PIXELS_PER_UNIT };
}

// Returns the unit vector of (x, y), or the fallback when the vector is zero.
function unit(x, y, fallback) {
  const length = Math.hypot(x, y);
  return length < 1e-6 ? fallback : { x: x / length, y: y / length };
}

function readInput() {
  const right = keys.has('d') || keys.has('arrowright');
  const left = keys.has('a') || keys.has('arrowleft');
  const down = keys.has('s') || keys.has('arrowdown');
  const up = keys.has('w') || keys.has('arrowup');
  return {
    moveX: Number(right) - Number(left),
    moveY: Number(down) - Number(up),
    aimX: mouse.x / PIXELS_PER_UNIT - game.player.x,
    aimY: (mouse.y - HUD_HEIGHT) / PIXELS_PER_UNIT - game.player.y,
    fire: clicked,
  };
}

function drawCircle(x, y, radius, color) {
  ctx.fillStyle = color;
  ctx.beginPath();
  ctx.arc(x, y, radius, 0, Math.PI * 2);
  ctx.fill();
}

function outlineCircle(x, y, radius, color, width) {
  ctx.strokeStyle = color;
  ctx.lineWidth = width;
  ctx.beginPath();
  ctx.arc(x, y, radius, 0, Math.PI * 2);
  ctx.stroke();
}

function drawGround() {
  ctx.fillStyle = COLORS.background;
  ctx.fillRect(0, 0, canvas.width, canvas.height);
  ctx.strokeStyle = COLORS.grid;
  ctx.lineWidth = 1;
  ctx.beginPath();
  for (let x = 0; x <= canvas.width; x += PIXELS_PER_UNIT) {
    ctx.moveTo(x + 0.5, HUD_HEIGHT);
    ctx.lineTo(x + 0.5, canvas.height);
  }
  for (let y = HUD_HEIGHT; y <= canvas.height; y += PIXELS_PER_UNIT) {
    ctx.moveTo(0, y + 0.5);
    ctx.lineTo(canvas.width, y + 0.5);
  }
  ctx.stroke();
}

function drawZombie(center, target) {
  const d = unit(target.x - center.x, target.y - center.y, { x: 1, y: 0 });
  drawCircle(center.x, center.y, ZOMBIE_DRAW_RADIUS, COLORS.zombie);
  outlineCircle(center.x, center.y, ZOMBIE_DRAW_RADIUS, COLORS.zombieOutline, 3);
  for (const side of [-1, 1]) { // two eyes, looking towards the player
    const ex = center.x + d.x * 6 - d.y * 5 * side;
    const ey = center.y + d.y * 6 + d.x * 5 * side;
    drawCircle(ex, ey, 4, COLORS.eye);
    drawCircle(ex + d.x * 1.5, ey + d.y * 1.5, 2, '#141414');
  }
}

function drawPlayer(center, facing, aim) {
  const d = unit(aim.x - center.x, aim.y - center.y, facing);
  ctx.strokeStyle = COLORS.gun;
  ctx.lineWidth = 6;
  ctx.beginPath();
  ctx.moveTo(center.x, center.y);
  ctx.lineTo(center.x + d.x * 24, center.y + d.y * 24);
  ctx.stroke();
  drawCircle(center.x, center.y, PLAYER_DRAW_RADIUS, COLORS.player);
  outlineCircle(center.x, center.y, PLAYER_DRAW_RADIUS, COLORS.playerOutline, 3);
}

function drawPickup(center) {
  ctx.fillStyle = COLORS.pickup;
  ctx.fillRect(center.x - 8, center.y - 8, 16, 16);
  ctx.fillStyle = COLORS.cross;
  ctx.fillRect(center.x - 2, center.y - 6, 4, 12);
  ctx.fillRect(center.x - 6, center.y - 2, 12, 4);
}

function drawGame() {
  drawGround();
  for (const pickup of game.pickups) {
    drawPickup(toScreen(pickup.x, pickup.y));
  }
  const player = toScreen(game.player.x, game.player.y);
  for (const zombie of game.zombies) {
    drawZombie(toScreen(zombie.x, zombie.y), player);
  }
  for (const bullet of game.bullets) {
    const b = toScreen(bullet.pos.x, bullet.pos.y);
    drawCircle(b.x, b.y, 4, COLORS.bullet);
  }
  drawPlayer(player, game.facing, mouse);

  drawHud();
  if (flash > 0) {
    ctx.globalAlpha = 0.4 * (flash / 0.3);
    ctx.fillStyle = '#c80000';
    ctx.fillRect(0, 0, canvas.width, canvas.height);
    ctx.globalAlpha = 1;
  }
  drawMessage();
}

function drawHud() {
  const barWidth = 160;
  const barX = canvas.width - barWidth - 10;
  ctx.fillStyle = COLORS.hud;
  ctx.fillRect(0, 0, canvas.width, HUD_HEIGHT);
  ctx.fillStyle = COLORS.text;
  ctx.font = '16px sans-serif';
  ctx.textBaseline = 'middle';
  ctx.textAlign = 'left';
  ctx.fillText(`Wave ${game.wave}   Score ${game.score}`, 10, HUD_HEIGHT / 2);
  ctx.textAlign = 'right';
  ctx.fillText('Health', barX - 10, HUD_HEIGHT / 2);
  ctx.fillStyle = COLORS.healthBack;
  ctx.fillRect(barX, HUD_HEIGHT / 2 - 6, barWidth, 12);
  ctx.fillStyle = COLORS.healthBar;
  ctx.fillRect(barX, HUD_HEIGHT / 2 - 6, (barWidth * game.health) / MAX_HEALTH, 12);
}

function drawMessage() {
  let message = '';
  if (game.gameOver) {
    message = 'GAME OVER - press R or click to play again';
  } else if (game.waveDelay > 0) {
    message = `Wave ${game.wave} cleared!`;
  }
  if (message === '') {
    return;
  }
  ctx.fillStyle = COLORS.text;
  ctx.font = 'bold 28px sans-serif';
  ctx.textAlign = 'center';
  ctx.textBaseline = 'middle';
  ctx.fillText(message, canvas.width / 2, canvas.height / 2);
}

function frame(time) {
  const dt = Math.min((time - lastTime) / 1000, 0.05);
  lastTime = time;
  update(game, readInput(), dt);
  clicked = false;
  if (game.health < lastHealth) {
    flash = 0.3;
  }
  lastHealth = game.health;
  flash = Math.max(0, flash - dt);
  drawGame();
  requestAnimationFrame(frame);
}

function mousePosition(event) {
  const rect = canvas.getBoundingClientRect();
  mouse.x = ((event.clientX - rect.left) * canvas.width) / rect.width;
  mouse.y = ((event.clientY - rect.top) * canvas.height) / rect.height;
}

canvas.addEventListener('mousemove', mousePosition);
canvas.addEventListener('mousedown', (event) => {
  if (event.button !== 0) {
    return;
  }
  mousePosition(event);
  clicked = true;
  if (game.gameOver) {
    restart();
  }
});
window.addEventListener('keydown', (event) => {
  const key = event.key.toLowerCase();
  if (key.startsWith('arrow')) {
    event.preventDefault();
  }
  keys.add(key);
  if (key === 'r' && game.gameOver) {
    restart();
  }
});
window.addEventListener('keyup', (event) => {
  keys.delete(event.key.toLowerCase());
});
window.addEventListener('blur', () => {
  keys.clear();
});

requestAnimationFrame(frame);
