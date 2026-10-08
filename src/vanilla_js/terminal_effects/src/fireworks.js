// Fireworks: rockets fly up, burst into sparks, and gravity pulls everything down.
// Old frames are dimmed instead of cleared, which leaves glowing trails.
import { H, W, pixels, rnd, seed, showPixels, start } from './term.js';

const MAX = 400;
const COLORS = [0xff5050, 0xffd040, 0x50ff80, 0x50c8ff, 0xd070ff, 0xffffff];
const particles = Array.from({ length: MAX }, () => ({ life: 0 }));

function spawn(x, y, vx, vy, life, color) {
  const p = particles.find((q) => !q.life);
  if (p) Object.assign(p, { x, y, vx, vy, life, color, rocket: false });
  return p;
}

function burst(x, y) {
  const color = COLORS[rnd(6)];
  for (let n = 0; n < 60; n++) {
    let dx;
    let dy;
    do {
      dx = (rnd(201) - 100) / 100;
      dy = (rnd(201) - 100) / 100;
    } while (dx * dx + dy * dy > 1);
    spawn(x, y, dx, dy, 25 + rnd(20), color);
  }
}

const dim = (c, num, den) =>
  (Math.floor(((c >> 16) * num) / den) << 16) |
  (Math.floor((((c >> 8) & 255) * num) / den) << 8) |
  Math.floor(((c & 255) * num) / den);

seed(2025);
start(30);
for (;;) {
  for (const row of pixels) {
    for (let x = 0; x < W; x++) row[x] = dim(row[x], 3, 4);
  }
  if (rnd(12) === 0) {
    const x = 8 + rnd(W - 16);
    const dx = (rnd(41) - 20) / 100;
    const dy = -(1.3 + rnd(40) / 100);
    const p = spawn(x, H - 1, dx, dy, 100, 0xffe0b0);
    if (p) p.rocket = true;
  }
  for (const p of particles) {
    if (!p.life) continue;
    p.x += p.vx;
    p.y += p.vy;
    p.vy += p.rocket ? 0.05 : 0.02;
    p.vx *= 0.97;
    p.life--;
    if (p.rocket && p.vy >= 0) {
      p.life = 0;
      burst(p.x, p.y);
      continue;
    }
    const [x, y] = [Math.trunc(p.x), Math.trunc(p.y)];
    if (x >= 0 && x < W && y >= 0 && y < H) pixels[y][x] = p.rocket ? p.color : dim(p.color, p.life + 15, 60);
  }
  await showPixels();
}
