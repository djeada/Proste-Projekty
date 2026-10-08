/* The page: the canvas, toolbar buttons, mouse and keyboard. The paint logic is in graphics_editor.js. */
const CANVAS_W = 320;
const CANVAS_H = 240;
const DEFAULT_FILE_NAME = 'drawing.png';
const WIDTHS = [1, 3, 5];
const TOOL_KEYS = { b: 'brush', e: 'eraser', l: 'line', r: 'rect', f: 'fill', p: 'picker' };
const PALETTE = [
  [0, 0, 0], [255, 255, 255], [128, 128, 128], [160, 0, 0],
  [255, 0, 0], [255, 128, 0], [255, 230, 0], [0, 160, 0],
  [0, 220, 120], [0, 120, 255], [120, 0, 255], [255, 0, 200],
];

const paintArea = document.getElementById('paint');
const currentColor = document.getElementById('current-color');
const fileInput = document.getElementById('file-input');
const state = {
  canvas: createCanvas(CANVAS_W, CANVAS_H),
  preview: null,
  history: createHistory(),
  tool: 'brush',
  width: WIDTHS[0],
  color: BLACK,
  start: null,
  last: null,
};

/* Draws a canvas onto a <canvas> element of the same size. */
function paint(element, canvas) {
  const context = element.getContext('2d');
  const image = context.createImageData(canvas.width, canvas.height);
  for (let i = 0, j = 0; i < canvas.pixels.length; i += 3, j += 4) {
    image.data[j] = canvas.pixels[i];
    image.data[j + 1] = canvas.pixels[i + 1];
    image.data[j + 2] = canvas.pixels[i + 2];
    image.data[j + 3] = 255;
  }
  context.putImageData(image, 0, 0);
}

function rgb(color) {
  return `rgb(${color[0]}, ${color[1]}, ${color[2]})`;
}

function render() {
  paint(paintArea, state.preview || state.canvas);
}

function updateToolbar() {
  for (const button of document.querySelectorAll('[data-tool]')) {
    button.classList.toggle('active', button.dataset.tool === state.tool);
  }
  for (const button of document.querySelectorAll('[data-width]')) {
    button.classList.toggle('active', Number(button.dataset.width) === state.width);
  }
  for (const swatch of document.querySelectorAll('[data-color]')) {
    swatch.classList.toggle('active', sameColor(PALETTE[Number(swatch.dataset.color)], state.color));
  }
  currentColor.style.background = rgb(state.color);
}

function selectTool(tool) {
  state.tool = tool;
  updateToolbar();
}

function selectWidth(width) {
  state.width = width;
  updateToolbar();
}

function selectColor(color) {
  state.color = color;
  updateToolbar();
}

function strokeColor() {
  return state.tool === 'eraser' ? WHITE : state.color;
}

/* The shape being dragged is drawn from the press point (state.start) to (x, y). */
function drawShape(canvas, x, y) {
  const [x0, y0] = state.start;
  if (state.tool === 'line') {
    drawLine(canvas, x0, y0, x, y, state.width, state.color);
  } else {
    drawRect(canvas, x0, y0, x, y, state.width, state.color);
  }
}

function pressCanvas(x, y) {
  if (state.tool === 'picker') {
    if (canvasInside(state.canvas, x, y)) {
      selectColor(getPixel(state.canvas, x, y));
    }
    return;
  }
  pushHistory(state.history, state.canvas);
  state.start = [x, y];
  state.last = [x, y];
  if (state.tool === 'fill') {
    floodFill(state.canvas, x, y, state.color);
    state.start = null;
  } else if (state.tool === 'brush' || state.tool === 'eraser') {
    drawLine(state.canvas, x, y, x, y, state.width, strokeColor());
  }
  render();
}

function dragCanvas(x, y) {
  if (state.tool === 'brush' || state.tool === 'eraser') {
    drawLine(state.canvas, state.last[0], state.last[1], x, y, state.width, strokeColor());
  } else {
    state.preview = copyCanvas(state.canvas);
    drawShape(state.preview, x, y);
  }
  state.last = [x, y];
  render();
}

function releaseCanvas() {
  if (state.tool === 'line' || state.tool === 'rect') {
    drawShape(state.canvas, state.last[0], state.last[1]);
  }
  state.start = null;
  state.preview = null;
  render();
}

function undo() {
  const previous = undoHistory(state.history);
  if (previous !== null) {
    state.canvas = previous;
    render();
  }
}

function clearAll() {
  pushHistory(state.history, state.canvas);
  clearCanvas(state.canvas);
  render();
}

function downloadPng() {
  const exporter = document.createElement('canvas');
  exporter.width = CANVAS_W;
  exporter.height = CANVAS_H;
  paint(exporter, state.canvas);
  exporter.toBlob((blob) => {
    const link = document.createElement('a');
    link.href = URL.createObjectURL(blob);
    link.download = DEFAULT_FILE_NAME;
    link.click();
    setTimeout(() => URL.revokeObjectURL(link.href), 1000);
  }, 'image/png');
}

/* The picture is drawn at its top-left corner; anything beyond the canvas is cut off. */
function loadImage(image) {
  const scratch = document.createElement('canvas');
  scratch.width = CANVAS_W;
  scratch.height = CANVAS_H;
  const context = scratch.getContext('2d');
  context.fillStyle = '#ffffff';
  context.fillRect(0, 0, CANVAS_W, CANVAS_H);
  context.drawImage(image, 0, 0);
  const data = context.getImageData(0, 0, CANVAS_W, CANVAS_H).data;
  pushHistory(state.history, state.canvas);
  for (let i = 0, j = 0; i < state.canvas.pixels.length; i += 3, j += 4) {
    state.canvas.pixels[i] = data[j];
    state.canvas.pixels[i + 1] = data[j + 1];
    state.canvas.pixels[i + 2] = data[j + 2];
  }
  render();
}

function openFile() {
  fileInput.click();
}

fileInput.addEventListener('change', () => {
  const file = fileInput.files[0];
  fileInput.value = '';
  if (!file) {
    return;
  }
  const url = URL.createObjectURL(file);
  const image = new Image();
  image.onload = () => {
    loadImage(image);
    URL.revokeObjectURL(url);
  };
  image.onerror = () => {
    URL.revokeObjectURL(url);
    window.alert('This file is not an image.');
  };
  image.src = url;
});

/* Positions are in canvas pixels, so the 2x display size is divided out. */
function canvasPosition(event) {
  const rect = paintArea.getBoundingClientRect();
  return {
    x: Math.floor(((event.clientX - rect.left) * CANVAS_W) / rect.width),
    y: Math.floor(((event.clientY - rect.top) * CANVAS_H) / rect.height),
  };
}

paintArea.addEventListener('mousedown', (event) => {
  if (event.button !== 0) {
    return;
  }
  const { x, y } = canvasPosition(event);
  pressCanvas(x, y);
});

window.addEventListener('mousemove', (event) => {
  if (state.start !== null) {
    const { x, y } = canvasPosition(event);
    dragCanvas(x, y);
  }
});

window.addEventListener('mouseup', (event) => {
  if (event.button === 0 && state.start !== null) {
    releaseCanvas();
  }
});

document.addEventListener('keydown', (event) => {
  const key = event.key.toLowerCase();
  if (event.ctrlKey || event.metaKey) {
    if (key === 'z') {
      event.preventDefault();
      undo();
    } else if (key === 's') {
      event.preventDefault();
      downloadPng();
    } else if (key === 'o') {
      event.preventDefault();
      openFile();
    }
  } else if (TOOL_KEYS[key]) {
    selectTool(TOOL_KEYS[key]);
  } else if (key === '1' || key === '2' || key === '3') {
    selectWidth(WIDTHS[Number(key) - 1]);
  } else if (key === 'n') {
    clearAll();
  }
});

function buildPalette() {
  const palette = document.getElementById('palette');
  PALETTE.forEach((color, index) => {
    const swatch = document.createElement('button');
    swatch.dataset.color = String(index);
    swatch.style.background = rgb(color);
    swatch.title = rgb(color);
    swatch.addEventListener('click', () => selectColor(color));
    palette.appendChild(swatch);
  });
}

for (const button of document.querySelectorAll('[data-tool]')) {
  button.addEventListener('click', () => selectTool(button.dataset.tool));
}
for (const button of document.querySelectorAll('[data-width]')) {
  button.addEventListener('click', () => selectWidth(Number(button.dataset.width)));
}
document.getElementById('undo').addEventListener('click', undo);
document.getElementById('clear').addEventListener('click', clearAll);
document.getElementById('open').addEventListener('click', openFile);
document.getElementById('save').addEventListener('click', downloadPng);

buildPalette();
updateToolbar();
render();
