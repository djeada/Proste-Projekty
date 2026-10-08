// A spinning torus drawn with ASCII characters: brighter character, more light.
import { COLS, ROWS, ink, showText, start, text } from './term.js';

const RAMP = '.,-~:;=!*#$@';

let A = 0;
let B = 0;
start(30);
for (;;) {
  const zbuf = Array.from({ length: ROWS }, () => new Array(COLS).fill(0));
  for (const row of text) row.fill(' ');
  const [sA, cA, sB, cB] = [Math.sin(A), Math.cos(A), Math.sin(B), Math.cos(B)];
  for (let j = 0; j < 90; j++) {
    const sj = Math.sin(j * 0.07);
    const cj = Math.cos(j * 0.07);
    const h = cj + 2;
    for (let i = 0; i < 314; i++) {
      const si = Math.sin(i * 0.02);
      const ci = Math.cos(i * 0.02);
      const D = 1 / (si * h * sA + sj * cA + 5);
      const t = si * h * cA - sj * sA;
      const x = COLS / 2 + Math.trunc(32 * D * (ci * h * cB - t * sB));
      const y = ROWS / 2 + Math.trunc(14 * D * (ci * h * sB + t * cB));
      const L = Math.max(0, Math.trunc(8 * ((sj * sA - si * cj * cA) * cB - si * cj * sA - sj * cA - ci * cj * sB)));
      if (x >= 0 && x < COLS && y >= 0 && y < ROWS && D > zbuf[y][x]) {
        zbuf[y][x] = D;
        text[y][x] = RAMP[L];
        ink[y][x] = ((L * 16) << 16) | ((80 + L * 15) << 8) | (L * 12);
      }
    }
  }
  await showText();
  A += 0.07;
  B += 0.03;
}
