// Zooms into the Mandelbrot set near the "seahorse valley".
import { H, W, pixels, showPixels, start } from './term.js';

const PALETTE = [
  0x000764, 0x02308c, 0x0a5bb4, 0x2085d2, 0x4cb0e6, 0x8bd6f2, 0xd2f0f7, 0xfff7c8,
  0xffe07a, 0xffb52e, 0xf4800c, 0xd94f07, 0xa82808, 0x7a1240, 0x4a0a6e, 0x1e0368,
];
const cx = -0.743643887037151;
const cy = 0.13182590420533;

let scale = 3;
let frame = 0;
start(30);
for (;;) {
  const maxIter = 60 + frame * 3;
  for (let y = 0; y < H; y++) {
    for (let x = 0; x < W; x++) {
      const re = cx + ((x - W / 2) * scale) / W;
      const im = cy + ((y - H / 2) * scale) / W;
      let zr = 0;
      let zi = 0;
      let it = 0;
      while (zr * zr + zi * zi < 4 && it < maxIter) {
        [zr, zi] = [zr * zr - zi * zi + re, 2 * zr * zi + im];
        it++;
      }
      pixels[y][x] = it === maxIter ? 0 : PALETTE[(it + frame) % 16];
    }
  }
  await showPixels(` zoom ${Math.trunc(3 / scale)}x, ${maxIter} iterations`);
  scale *= 0.93;
  if (++frame === 130) {
    frame = 0;
    scale = 3;
  }
}
