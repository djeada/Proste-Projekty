const { test } = require('node:test');
const assert = require('node:assert/strict');
const {
  createCanvas, clearCanvas, getPixel, setPixel, copyCanvas, drawLine, drawRect, floodFill,
  createHistory, pushHistory, undoHistory, WHITE, BLACK,
} = require('../src/graphics_editor.js');

const RED = [255, 0, 0];
const BLUE = [0, 0, 255];
const HISTORY_DEPTH = 20;

function count(canvas, color) {
  let total = 0;
  for (let y = 0; y < canvas.height; y++) {
    for (let x = 0; x < canvas.width; x++) {
      if (getPixel(canvas, x, y).every((value, i) => value === color[i])) total++;
    }
  }
  return total;
}

test('a new canvas is white', () => {
  const canvas = createCanvas(8, 6);
  assert.equal(canvas.pixels.length, 8 * 6 * 3);
  assert.equal(count(canvas, WHITE), 48);
});

test('set and get ignore positions outside the canvas', () => {
  const canvas = createCanvas(4, 4);
  setPixel(canvas, 1, 2, RED);
  assert.deepEqual(getPixel(canvas, 1, 2), RED);
  setPixel(canvas, -1, 0, RED);
  setPixel(canvas, 4, 0, RED);
  assert.equal(count(canvas, RED), 1);
  assert.deepEqual(getPixel(canvas, 4, 0), BLACK);
});

test('a horizontal line has the right endpoints and pixel count', () => {
  const canvas = createCanvas(10, 10);
  drawLine(canvas, 2, 5, 7, 5, 1, RED);
  assert.deepEqual(getPixel(canvas, 2, 5), RED);
  assert.deepEqual(getPixel(canvas, 7, 5), RED);
  assert.equal(count(canvas, RED), 6);
});

test('a diagonal line and its reverse paint the same pixels', () => {
  const forward = createCanvas(10, 10);
  const backward = createCanvas(10, 10);
  drawLine(forward, 0, 0, 4, 4, 1, RED);
  drawLine(backward, 4, 4, 0, 0, 1, RED);
  assert.equal(count(forward, RED), 5);
  assert.deepEqual(forward.pixels, backward.pixels);
});

test('a steep line has exactly one pixel in every row', () => {
  const canvas = createCanvas(10, 10);
  drawLine(canvas, 1, 0, 3, 9, 1, RED);
  assert.equal(count(canvas, RED), 10);
  for (let y = 0; y < 10; y++) {
    let inRow = 0;
    for (let x = 0; x < 10; x++) {
      if (getPixel(canvas, x, y).join() === RED.join()) inRow++;
    }
    assert.equal(inRow, 1);
  }
});

test('a thick brush is clipped at the border', () => {
  const canvas = createCanvas(5, 5);
  drawLine(canvas, 0, 0, 0, 0, 3, RED);
  assert.equal(count(canvas, RED), 4);
  drawLine(canvas, 0, 4, 0, 4, 5, BLUE);
  assert.equal(count(canvas, BLUE), 9);
});

test('a rectangle is only an outline', () => {
  const canvas = createCanvas(10, 10);
  drawRect(canvas, 2, 2, 5, 4, 1, RED);
  // A 4 x 3 rectangle has 2 * (4 + 3) - 4 = 10 border pixels.
  assert.equal(count(canvas, RED), 10);
  assert.deepEqual(getPixel(canvas, 3, 3), WHITE);
  assert.deepEqual(getPixel(canvas, 5, 4), RED);
});

test('flood fill stops at the border of the region', () => {
  const canvas = createCanvas(7, 7);
  drawRect(canvas, 1, 1, 5, 5, 1, RED);
  floodFill(canvas, 3, 3, BLUE);
  assert.equal(count(canvas, BLUE), 9);
  assert.equal(count(canvas, RED), 16);
  assert.deepEqual(getPixel(canvas, 0, 0), WHITE);
});

test('flood fill with the same color does nothing', () => {
  const canvas = createCanvas(4, 4);
  floodFill(canvas, 1, 1, WHITE);
  assert.equal(count(canvas, WHITE), 16);
  floodFill(canvas, 9, 9, RED);
  assert.equal(count(canvas, RED), 0);
});

test('flood fill covers a whole big canvas without recursion', () => {
  const canvas = createCanvas(320, 240);
  floodFill(canvas, 0, 0, RED);
  assert.equal(count(canvas, RED), 320 * 240);
});

test('undo restores the previous image', () => {
  const history = createHistory();
  const canvas = createCanvas(4, 4);
  pushHistory(history, canvas);
  setPixel(canvas, 0, 0, RED);
  const restored = undoHistory(history);
  assert.deepEqual(getPixel(restored, 0, 0), WHITE);
  assert.equal(undoHistory(history), null);
});

test('the undo history drops the oldest snapshot when full', () => {
  const history = createHistory();
  const canvas = createCanvas(2, 2);
  for (let i = 0; i < HISTORY_DEPTH + 5; i++) {
    setPixel(canvas, 0, 0, RED);
    pushHistory(history, canvas);
  }
  let undone = 0;
  while (undoHistory(history) !== null) undone++;
  assert.equal(undone, HISTORY_DEPTH);
});

test('a copy is independent of the original', () => {
  const original = createCanvas(3, 3);
  setPixel(original, 1, 1, RED);
  const clone = copyCanvas(original);
  setPixel(original, 0, 0, BLUE);
  assert.deepEqual(getPixel(clone, 1, 1), RED);
  assert.deepEqual(getPixel(clone, 0, 0), WHITE);
});

test('clearing a canvas paints it one color', () => {
  const canvas = createCanvas(3, 2);
  clearCanvas(canvas, BLUE);
  assert.equal(count(canvas, BLUE), 6);
});
