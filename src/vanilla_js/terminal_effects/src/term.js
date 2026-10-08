export const COLS = 64;
export const ROWS = 22;
export const W = COLS;
export const H = ROWS * 2;

export const pixels = Array.from({ length: H }, () => new Array(W).fill(0));
export const text = Array.from({ length: ROWS }, () => new Array(COLS).fill(' '));
export const ink = Array.from({ length: ROWS }, () => new Array(COLS).fill(0));

let framesLeft = 0;
let delay = 0;
let state = 1;
let lastFrame = 0;

export function start(delayMs) {
  framesLeft = Number(process.argv[2]) || 0;
  delay = process.env.NO_SLEEP ? 0 : delayMs;
  process.on('SIGINT', () => process.exit(0));
  process.on('SIGTERM', () => process.exit(0));
  process.on('exit', () => process.stdout.write('\x1b[0m\x1b[?25h\n'));
  process.stdout.write('\x1b[?25l\x1b[2J');
  lastFrame = performance.now();
}

const color = (layer, c) => `\x1b[${layer};2;${c >> 16};${(c >> 8) & 255};${c & 255}m`;

async function endFrame(out, status) {
  process.stdout.write(out + `\x1b[0m${status}\x1b[K`);
  const wait = delay - (performance.now() - lastFrame);
  await new Promise((resolve) => (wait > 0 ? setTimeout(resolve, wait) : setImmediate(resolve)));
  lastFrame = performance.now();
  if (framesLeft > 0 && --framesLeft === 0) process.exit(0);
}

export function showPixels(status = '') {
  let out = '\x1b[H';
  for (let y = 0; y < H; y += 2) {
    let fg = -1;
    let bg = -1;
    for (let x = 0; x < W; x++) {
      if (pixels[y][x] !== fg) out += color(38, (fg = pixels[y][x]));
      if (pixels[y + 1][x] !== bg) out += color(48, (bg = pixels[y + 1][x]));
      out += '▀';
    }
    out += '\x1b[0m\n';
  }
  return endFrame(out, status);
}

export function showText(status = '') {
  let out = '\x1b[H';
  for (let y = 0; y < ROWS; y++) {
    let fg = -1;
    out += color(48, 0);
    for (let x = 0; x < COLS; x++) {
      if (text[y][x] !== ' ' && ink[y][x] !== fg) out += color(38, (fg = ink[y][x]));
      out += text[y][x];
    }
    out += '\x1b[0m\n';
  }
  return endFrame(out, status);
}

export function seed(s) {
  state = s;
}

export function rnd(n) {
  state = (Math.imul(state, 1103515245) + 12345) >>> 0;
  return (state >>> 16) % n;
}
