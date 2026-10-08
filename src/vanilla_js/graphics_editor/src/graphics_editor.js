/* The paint logic: canvas pixels, drawing primitives and the undo history. No DOM here. */
const HISTORY_DEPTH = 20;
const WHITE = [255, 255, 255];
const BLACK = [0, 0, 0];

/* Pixels are bytes, three per pixel (r, g, b), stored row by row. */
function createCanvas(width, height, fill = WHITE) {
  const canvas = { width, height, pixels: new Uint8Array(width * height * 3) };
  clearCanvas(canvas, fill);
  return canvas;
}

function clearCanvas(canvas, color = WHITE) {
  for (let i = 0; i < canvas.pixels.length; i += 3) {
    canvas.pixels[i] = color[0];
    canvas.pixels[i + 1] = color[1];
    canvas.pixels[i + 2] = color[2];
  }
}

function canvasInside(canvas, x, y) {
  return x >= 0 && y >= 0 && x < canvas.width && y < canvas.height;
}

/* Returns black for a position outside the canvas. */
function getPixel(canvas, x, y) {
  if (!canvasInside(canvas, x, y)) {
    return [...BLACK];
  }
  const i = (y * canvas.width + x) * 3;
  return [canvas.pixels[i], canvas.pixels[i + 1], canvas.pixels[i + 2]];
}

/* Positions outside the canvas are ignored, so drawing may run past the edges. */
function setPixel(canvas, x, y, color) {
  if (!canvasInside(canvas, x, y)) {
    return;
  }
  const i = (y * canvas.width + x) * 3;
  canvas.pixels[i] = color[0];
  canvas.pixels[i + 1] = color[1];
  canvas.pixels[i + 2] = color[2];
}

function sameColor(a, b) {
  return a[0] === b[0] && a[1] === b[1] && a[2] === b[2];
}

function copyCanvas(canvas) {
  return { width: canvas.width, height: canvas.height, pixels: canvas.pixels.slice() };
}

/* A square brush of the given width (1, 3 or 5) centred on (x, y). */
function stamp(canvas, x, y, width, color) {
  const half = Math.floor(width / 2);
  for (let dy = -half; dy <= half; dy++) {
    for (let dx = -half; dx <= half; dx++) {
      setPixel(canvas, x + dx, y + dy, color);
    }
  }
}

/*
 * Bresenham's line algorithm: step along the longer axis one pixel at a time and use an
 * error counter to decide when to also step along the shorter axis.
 */
function drawLine(canvas, x0, y0, x1, y1, width, color) {
  const dx = Math.abs(x1 - x0);
  const sx = x0 < x1 ? 1 : -1;
  const dy = -Math.abs(y1 - y0);
  const sy = y0 < y1 ? 1 : -1;
  let err = dx + dy;
  for (;;) {
    stamp(canvas, x0, y0, width, color);
    if (x0 === x1 && y0 === y1) {
      break;
    }
    const e2 = 2 * err;
    if (e2 >= dy) {
      err += dy;
      x0 += sx;
    }
    if (e2 <= dx) {
      err += dx;
      y0 += sy;
    }
  }
}

/* The outline of the rectangle whose opposite corners are (x0, y0) and (x1, y1). */
function drawRect(canvas, x0, y0, x1, y1, width, color) {
  drawLine(canvas, x0, y0, x1, y0, width, color);
  drawLine(canvas, x1, y0, x1, y1, width, color);
  drawLine(canvas, x1, y1, x0, y1, width, color);
  drawLine(canvas, x0, y1, x0, y0, width, color);
}

/*
 * Paints the region of pixels with the same color as (x, y), stopping at other colors.
 * The pixels still to visit go on an explicit stack, not on the call stack. A pixel is
 * painted as soon as it is pushed, so it is pushed only once.
 */
function floodFill(canvas, x, y, color) {
  if (!canvasInside(canvas, x, y)) {
    return;
  }
  const target = getPixel(canvas, x, y);
  if (sameColor(target, color)) {
    return;
  }
  setPixel(canvas, x, y, color);
  const stack = [[x, y]];
  while (stack.length > 0) {
    const [cx, cy] = stack.pop();
    for (const [nx, ny] of [[cx + 1, cy], [cx - 1, cy], [cx, cy + 1], [cx, cy - 1]]) {
      if (canvasInside(canvas, nx, ny) && sameColor(getPixel(canvas, nx, ny), target)) {
        setPixel(canvas, nx, ny, color);
        stack.push([nx, ny]);
      }
    }
  }
}

function createHistory() {
  return { snapshots: [] };
}

/* Saves a copy of the canvas. When the history is full the oldest snapshot is dropped. */
function pushHistory(history, canvas) {
  history.snapshots.push(copyCanvas(canvas));
  if (history.snapshots.length > HISTORY_DEPTH) {
    history.snapshots.shift();
  }
}

/* Returns the previous canvas, or null when there is nothing to undo. */
function undoHistory(history) {
  return history.snapshots.length > 0 ? history.snapshots.pop() : null;
}

if (typeof module !== 'undefined') module.exports = {
  createCanvas, clearCanvas, canvasInside, getPixel, setPixel, sameColor, copyCanvas,
  drawLine, drawRect, floodFill, createHistory, pushHistory, undoHistory,
  WHITE, BLACK,
};
