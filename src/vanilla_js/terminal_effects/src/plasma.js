// Demoscene plasma: four sine waves added together and turned into colors.
import { H, W, pixels, showPixels, start } from './term.js';

start(30);
for (let frame = 0; ; frame++) {
  const t = frame * 0.07;
  for (let y = 0; y < H; y++) {
    for (let x = 0; x < W; x++) {
      const px = x - W / 2;
      const py = y - H / 2;
      const v =
        Math.sin(px * 0.12 + t) +
        Math.sin(py * 0.1 - t * 1.3) +
        Math.sin((px + py) * 0.08 + t * 0.7) +
        Math.sin(Math.sqrt(px * px + py * py) * 0.15 - t * 1.6);
      const hue = v * 0.8 + t * 0.5;
      const r = Math.trunc(128 + 127 * Math.sin(hue));
      const g = Math.trunc(128 + 127 * Math.sin(hue + 2.094));
      const b = Math.trunc(128 + 127 * Math.sin(hue + 4.189));
      pixels[y][x] = (r << 16) | (g << 8) | b;
    }
  }
  await showPixels();
}
