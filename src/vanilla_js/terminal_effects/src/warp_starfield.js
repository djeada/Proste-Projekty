// Flying through a 3D starfield: the faster we go, the longer the streaks.
import { H, W, pixels, rnd, seed, showPixels, start } from './term.js';

const STARS = 300;

function spawn(z) {
  const x = (rnd(2001) - 1000) / 1000;
  const y = (rnd(2001) - 1000) / 1000;
  return { x, y, z };
}

function put(bright, x, y, z, b) {
  const px = W / 2 + Math.trunc((x / z) * 26);
  const py = H / 2 + Math.trunc((y / z) * 26);
  if (px >= 0 && px < W && py >= 0 && py < H && b > bright[py][px]) bright[py][px] = b;
}

seed(42);
const stars = Array.from({ length: STARS }, () => spawn(0.05 + rnd(1000) / 1000));
start(30);
for (let frame = 0; ; frame = (frame + 1) % 300) {
  const p = frame < 150 ? frame / 150 : (300 - frame) / 150;
  const speed = 0.003 + 0.03 * p * p;
  const trail = Math.trunc(speed * 300);
  const bright = Array.from({ length: H }, () => new Array(W).fill(0));
  for (let i = 0; i < STARS; i++) {
    stars[i].z -= speed;
    if (stars[i].z < 0.02) stars[i] = spawn(1.05);
    const { x, y, z } = stars[i];
    const b = Math.trunc((1.1 - z) * 230);
    for (let k = trail; k >= 1; k--) put(bright, x, y, z + k * speed * 0.6, Math.floor(b / (k + 1)));
    put(bright, x, y, z, b);
  }
  for (let y = 0; y < H; y++) {
    for (let x = 0; x < W; x++) {
      const b = bright[y][x];
      pixels[y][x] = (Math.floor((b * 3) / 4) << 16) | (Math.floor((b * 7) / 8) << 8) | b;
    }
  }
  await showPixels(` warp factor ${1 + Math.trunc(p * 8)}`);
}
