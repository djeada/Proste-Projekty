// Demoscene fire: every pixel is a little less than the average of the pixels below it.
// The two hidden rows under the screen are random embers that feed the flames.
import { H, W, pixels, rnd, seed, showPixels, start } from './term.js';

const PALETTE = [
  0x000000, 0x1f0707, 0x2f0f07, 0x470f07, 0x571707, 0x671f07, 0x771f07, 0x8f2707,
  0x9f2f07, 0xaf3f07, 0xbf4707, 0xc74707, 0xdf4f07, 0xdf5707, 0xdf5707, 0xd75f07,
  0xd75f07, 0xd7670f, 0xcf6f0f, 0xcf770f, 0xcf7f0f, 0xcf8717, 0xc78717, 0xc78f17,
  0xc7971f, 0xbf9f1f, 0xbf9f1f, 0xbfa727, 0xbfa727, 0xbfaf2f, 0xb7af2f, 0xb7b72f,
  0xb7b737, 0xcfcf6f, 0xdfdf9f, 0xefefc7, 0xffffff,
];
const MAX_HEAT = PALETTE.length - 1;

seed(1993);
const heat = Array.from({ length: H + 2 }, () => new Array(W + 2).fill(0));
start(30);
for (;;) {
  for (let x = 1; x <= W; x++) {
    heat[H][x] = rnd(3) ? MAX_HEAT : 0;
    heat[H + 1][x] = rnd(3) ? MAX_HEAT : 0;
  }
  for (let y = 0; y < H; y++) {
    for (let x = 1; x <= W; x++) {
      const sum = heat[y + 1][x - 1] + heat[y + 1][x] + heat[y + 1][x + 1] + heat[y + 2][x];
      heat[y][x] = Math.floor((sum * 31) / 129);
    }
  }
  for (let y = 0; y < H; y++) {
    for (let x = 0; x < W; x++) pixels[y][x] = PALETTE[heat[y][x + 1]];
  }
  await showPixels();
}
