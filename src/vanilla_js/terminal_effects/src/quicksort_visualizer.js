// Quicksort, one frame per swap. White bars are being swapped, magenta is the pivot.
import { H, W, pixels, rnd, seed, showPixels, start } from './term.js';

const KEYS = [0xff3030, 0xffd030, 0x30e060, 0x30c0ff, 0xa040ff];

function rainbow(v) {
  const p = Math.floor(((v - 1) * 1024) / H);
  const f = p % 256;
  const [c1, c2] = [KEYS[p >> 8], KEYS[(p >> 8) + 1]];
  let c = 0;
  for (let s = 0; s <= 16; s += 8) c |= Math.floor((((c1 >> s) & 255) * (256 - f) + ((c2 >> s) & 255) * f) / 256) << s;
  return c;
}

const a = Array.from({ length: W }, (_, i) => 1 + Math.floor((i * H) / W));
let compares = 0;
let swaps = 0;

function draw(i1 = -1, i2 = -1, pivot = -1, done = 0) {
  for (let x = 0; x < W; x++) {
    const c = x < done ? 0x40ff70 : x === pivot ? 0xff40ff : x === i1 || x === i2 ? 0xffffff : rainbow(a[x]);
    for (let y = 0; y < H; y++) pixels[y][x] = y >= H - a[x] ? c : 0;
  }
  return showPixels(` quicksort: ${compares} comparisons, ${swaps} swaps`);
}

async function swap(i, j, pivot) {
  [a[i], a[j]] = [a[j], a[i]];
  swaps++;
  await draw(i, j, pivot);
}

async function quicksort(lo, hi) {
  if (lo >= hi) return;
  const pivot = a[hi];
  let i = lo;
  for (let j = lo; j < hi; j++) {
    compares++;
    if (a[j] < pivot) await swap(i++, j, hi);
  }
  await swap(i, hi, i);
  await quicksort(lo, i - 1);
  await quicksort(i + 1, hi);
}

seed(7);
start(30);
for (;;) {
  for (let i = W - 1; i > 0; i--) {
    const j = rnd(i + 1);
    [a[i], a[j]] = [a[j], a[i]];
  }
  compares = swaps = 0;
  for (let k = 0; k < 20; k++) await draw();
  await quicksort(0, W - 1);
  for (let done = 1; done <= W + 40; done++) await draw(-1, -1, -1, done);
}
