// A cube spinning around three axes. A z-buffer keeps the nearest face on top,
// and faces turned towards the viewer are lit brighter.
import { H, W, pixels, showPixels, start } from './term.js';

const FACE_COLOR = [0xff4060, 0x40e070, 0x4080ff, 0xffd040, 0xe050ff, 0x40e0ff];
const NORMAL = [[0, 0, -1], [1, 0, 0], [-1, 0, 0], [0, 0, 1], [0, -1, 0], [0, 1, 0]];

let A = 0;
let B = 0;
let C = 0;
start(30);
for (;;) {
  const [sA, cA, sB, cB, sC, cC] = [Math.sin(A), Math.cos(A), Math.sin(B), Math.cos(B), Math.sin(C), Math.cos(C)];
  const zbuf = Array.from({ length: H }, () => new Array(W).fill(0));
  for (const row of pixels) row.fill(0);

  const rotate = (i, j, k) => [
    j * sA * sB * cC - k * cA * sB * cC + j * cA * sC + k * sA * sC + i * cB * cC,
    j * cA * cC + k * sA * cC - j * sA * sB * sC + k * cA * sB * sC - i * cB * sC,
    k * cA * cB - j * sA * cB + i * sB,
  ];

  const plot = (i, j, k, color) => {
    const [x, y, z] = rotate(i, j, k);
    const ooz = 1 / (z + 60);
    const px = W / 2 + Math.trunc(60 * ooz * x);
    const py = H / 2 + Math.trunc(60 * ooz * y);
    if (px >= 0 && px < W && py >= 0 && py < H && ooz > zbuf[py][px]) {
      zbuf[py][px] = ooz;
      pixels[py][px] = color;
    }
  };

  const color = FACE_COLOR.map((c, face) => {
    const light = 0.3 + 0.7 * Math.max(0, -rotate(...NORMAL[face])[2]);
    let out = 0;
    for (let s = 0; s <= 16; s += 8) out |= Math.trunc(((c >> s) & 255) * light) << s;
    return out;
  });

  for (let u = 0; u < 40; u++) {
    for (let v = 0; v < 40; v++) {
      const a = -10 + u * 0.5;
      const b = -10 + v * 0.5;
      plot(a, b, -10, color[0]);
      plot(10, b, a, color[1]);
      plot(-10, b, -a, color[2]);
      plot(-a, b, 10, color[3]);
      plot(a, -10, -b, color[4]);
      plot(a, 10, b, color[5]);
    }
  }
  await showPixels();
  A += 0.05;
  B += 0.05;
  C += 0.01;
}
