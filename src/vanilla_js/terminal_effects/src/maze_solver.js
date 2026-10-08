// Carves a random maze with depth-first search, then finds the shortest path with BFS.
import { H, W, pixels, rnd, seed, showPixels, start } from './term.js';

const MW = W - 1;
const MH = H - 1;
const WALL = -2;
const OPEN = -1;
const RIPPLE = [0x20e0ff, 0x2098ff, 0x3060ff, 0x6040f0, 0x9030e0, 0x6040f0, 0x3060ff, 0x2098ff];

let maze;
let onPath;

function color(x, y, hx, hy) {
  if (x === 1 && y === 1) return 0x40ff70;
  if (x === MW - 2 && y === MH - 2) return 0xff4040;
  if (x === hx && y === hy) return 0xff40ff;
  if (onPath[y][x]) return 0xffe040;
  if (maze[y][x] === WALL) return 0x384050;
  if (maze[y][x] >= 0) return RIPPLE[Math.floor(maze[y][x] / 4) % 8];
  return 0;
}

function draw(status, hx = -1, hy = -1) {
  for (let y = 0; y < H; y++) {
    for (let x = 0; x < W; x++) pixels[y][x] = x < MW && y < MH ? color(x, y, hx, hy) : 0;
  }
  return showPixels(status);
}

async function carve() {
  const dirs = [[0, -2], [2, 0], [0, 2], [-2, 0]];
  const stack = [[1, 1]];
  maze[1][1] = OPEN;
  while (stack.length) {
    const [x, y] = stack[stack.length - 1];
    for (let i = 3; i > 0; i--) {
      const j = rnd(i + 1);
      [dirs[i], dirs[j]] = [dirs[j], dirs[i]];
    }
    const next = dirs.find(([dx, dy]) => {
      const [nx, ny] = [x + dx, y + dy];
      return nx > 0 && nx < MW - 1 && ny > 0 && ny < MH - 1 && maze[ny][nx] === WALL;
    });
    if (!next) {
      stack.pop();
      continue;
    }
    const [dx, dy] = next;
    maze[y + dy / 2][x + dx / 2] = OPEN;
    maze[y + dy][x + dx] = OPEN;
    stack.push([x + dx, y + dy]);
    await draw(' carving the maze (depth-first search)', x + dx, y + dy);
  }
}

async function solve() {
  const from = Array.from({ length: MH }, () => new Array(MW));
  const queue = [[1, 1]];
  let layer = 0;
  maze[1][1] = 0;
  for (let head = 0; head < queue.length; head++) {
    const [x, y] = queue[head];
    if (maze[y][x] > layer) {
      layer = maze[y][x];
      if (layer % 2 === 0) await draw(` searching (breadth-first search): distance ${layer}`);
    }
    if (x === MW - 2 && y === MH - 2) break;
    for (const [dx, dy] of [[0, -1], [1, 0], [0, 1], [-1, 0]]) {
      const [nx, ny] = [x + dx, y + dy];
      if (maze[ny][nx] === OPEN) {
        maze[ny][nx] = maze[y][x] + 1;
        from[ny][nx] = [x, y];
        queue.push([nx, ny]);
      }
    }
  }
  const status = ` shortest path: ${maze[MH - 2][MW - 2]} steps`;
  for (let [x, y] = [MW - 2, MH - 2]; x !== 1 || y !== 1; [x, y] = from[y][x]) {
    onPath[y][x] = true;
    await draw(status);
  }
  for (let k = 0; k < 60; k++) await draw(status);
}

seed(2024);
start(15);
for (;;) {
  maze = Array.from({ length: MH }, () => new Array(MW).fill(WALL));
  onPath = Array.from({ length: MH }, () => new Array(MW).fill(false));
  await carve();
  await solve();
}
