// Conway's Game of Life on a wrapping board, cells colored by age.
import { H, W, pixels, rnd, seed, showPixels, start } from './term.js';

const AGE_COLOR = [0xffffff, 0xc8fff4, 0x8ef5e6, 0x4fd8e0, 0x2aa8d8, 0x2a72c8, 0x3044a8, 0x2a2a80];
const GLIDER = [[0, 1], [1, 2], [2, 0], [2, 1], [2, 2]];

seed(42);
let grid = Array.from({ length: H }, () => Array.from({ length: W }, () => (rnd(3) === 0 ? 1 : 0)));
start(40);
for (let gen = 0; ; gen++) {
  let alive = 0;
  for (let y = 0; y < H; y++) {
    for (let x = 0; x < W; x++) {
      const age = grid[y][x];
      if (age) alive++;
      pixels[y][x] = age ? AGE_COLOR[Math.min(age, 8) - 1] : 0;
    }
  }
  await showPixels(` generation ${gen}, alive ${alive}`);

  const next = Array.from({ length: H }, () => new Array(W).fill(0));
  for (let y = 0; y < H; y++) {
    for (let x = 0; x < W; x++) {
      let n = 0;
      for (let dy = -1; dy <= 1; dy++) {
        for (let dx = -1; dx <= 1; dx++) {
          if ((dy || dx) && grid[(y + dy + H) % H][(x + dx + W) % W]) n++;
        }
      }
      const age = grid[y][x];
      next[y][x] = n === 3 || (age && n === 2) ? age + 1 : 0;
    }
  }
  grid = next;
  if (gen % 20 === 19) {
    const gx = rnd(W);
    const gy = rnd(H);
    for (const [dy, dx] of GLIDER) grid[(gy + dy) % H][(gx + dx) % W] = 1;
  }
}
