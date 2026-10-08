// The "digital rain" from The Matrix: one falling stream of characters per column.
import { COLS, ROWS, ink, rnd, seed, showText, start, text } from './term.js';

const SHADE = [
  0xeaffea, 0x9cff9c, 0x4cf04c, 0x22d022, 0x18b018, 0x149414,
  0x107a10, 0x0c640c, 0x0a520a, 0x084208, 0x063406, 0x042804,
];
const TRAIL = SHADE.length;
const randomGlyph = () => String.fromCharCode(33 + rnd(94));

seed(1999);
const head = [];
const speed = [];
const glyph = Array.from({ length: ROWS }, () => new Array(COLS));
for (let x = 0; x < COLS; x++) {
  head.push(-rnd(ROWS * 4));
  speed.push(1 + rnd(3));
  for (let y = 0; y < ROWS; y++) glyph[y][x] = randomGlyph();
}
start(50);
for (let frame = 0; ; frame++) {
  for (const row of text) row.fill(' ');
  for (let x = 0; x < COLS; x++) {
    if (frame % speed[x] === 0) head[x]++;
    if (head[x] - TRAIL > ROWS) {
      head[x] = -rnd(ROWS);
      speed[x] = 1 + rnd(3);
    }
    for (let d = 0; d < TRAIL; d++) {
      const y = head[x] - d;
      if (y < 0 || y >= ROWS) continue;
      if (rnd(20) === 0) glyph[y][x] = randomGlyph();
      text[y][x] = glyph[y][x];
      ink[y][x] = SHADE[d];
    }
  }
  await showText();
}
