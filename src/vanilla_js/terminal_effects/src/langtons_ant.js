// Langton's ant: on an empty cell turn right, on a filled cell turn left,
// flip the cell and step forward. After ~10,000 steps of chaos it builds a "highway".
import { H, W, pixels, showPixels, start } from './term.js';

const DX = [0, 1, 0, -1];
const DY = [-1, 0, 1, 0];

start(30);
for (;;) {
  let [x, y, dir] = [W / 2, H / 2, 0];
  const cell = Array.from({ length: H }, () => new Array(W).fill(0));
  for (let step = 0; step < 12000; step++) {
    dir = (dir + (cell[y][x] ? 3 : 1)) % 4;
    cell[y][x] = 1 - cell[y][x];
    x = (x + DX[dir] + W) % W;
    y = (y + DY[dir] + H) % H;
    if (step % 60 === 0) {
      for (let py = 0; py < H; py++) {
        for (let px = 0; px < W; px++) pixels[py][px] = cell[py][px] ? 0x30c0a0 : 0;
      }
      pixels[y][x] = 0xff3050;
      await showPixels(` step ${step}`);
    }
  }
}
