// Smoke tests: run each effect's inline script against a tiny fake browser
// for a few simulated seconds and check that it keeps animating and drawing.
const { test } = require('node:test');
const assert = require('node:assert');
const fs = require('node:fs');
const path = require('node:path');
const vm = require('node:vm');

const SRC = path.join(__dirname, '..', 'src');
const EFFECTS = fs.readdirSync(SRC).filter(f => f.endsWith('.html') && f !== 'index.html');
const DRAW_CALLS = ['fillRect', 'fill', 'stroke', 'fillText', 'putImageData'];

// A 2D context that accepts any call, counts it and remembers NaN arguments.
const fakeContext = canvas => {
  const calls = {}, badCalls = [];
  const results = {
    createImageData: (w, h) => ({ width: w, height: h, data: new Uint8ClampedArray(w * h * 4) }),
    measureText: text => ({ width: String(text).length * 10 }),
    createLinearGradient: () => ({ addColorStop() {} }),
    createRadialGradient: () => ({ addColorStop() {} }),
  };
  const state = { canvas, calls, badCalls };
  return new Proxy(state, {
    get: (target, name) => name in target ? target[name] : (...args) => {
      calls[name] = (calls[name] || 0) + 1;
      if (args.some(a => typeof a === 'number' && Number.isNaN(a))) badCalls.push(name);
      return results[name]?.(...args);
    },
    set: (target, name, value) => {
      target[name] = value;
      return true;
    },
  });
};

const fakeElement = () => {
  const element = { style: {}, textContent: '', width: 300, height: 150 };
  element.getContext = () => (element.ctx ??= fakeContext(element));
  return element;
};

// Load the page's scripts, then pump requestAnimationFrame with a fake clock.
const run = (file, { frames = 200, frameMs = 60 } = {}) => {
  const html = fs.readFileSync(path.join(SRC, file), 'utf8');
  const scripts = [...html.matchAll(/<script>([\s\S]*?)<\/script>/g)].map(m => m[1]);
  const elements = {}, listeners = {};
  let now = 0, queue = [];

  const window = {
    innerWidth: 120,
    innerHeight: 90,
    devicePixelRatio: 2,
    document: {
      getElementById: id => (elements[id] ??= fakeElement()),
      querySelector: selector => (elements[selector] ??= fakeElement()),
      createElement: () => fakeElement(),
      body: { appendChild() {} },
    },
    performance: { now: () => now },
    requestAnimationFrame: callback => queue.push(callback),
    addEventListener: (type, listener) => (listeners[type] ??= []).push(listener),
    console,
  };
  window.window = window;

  vm.createContext(window);
  for (const code of scripts) vm.runInContext(code, window, { filename: file });

  for (let i = 0; i < frames; i++) {
    if (i === frames / 2) {  // turn the "phone" sideways halfway through
      window.innerWidth = 90;
      window.innerHeight = 160;
      (listeners.resize || []).forEach(listener => listener());
    }
    now += frameMs;
    const callbacks = queue;
    queue = [];
    callbacks.forEach(callback => callback(now));
  }
  return { elements, pending: queue.length };
};

for (const file of EFFECTS) {
  test(`${file} animates and draws`, () => {
    const { elements, pending } = run(file);
    const { calls, badCalls } = elements.screen.getContext('2d');
    const drawn = DRAW_CALLS.reduce((sum, name) => sum + (calls[name] || 0), 0);

    assert.ok(drawn > 0, 'nothing was drawn');
    assert.strictEqual(pending, 1, 'expected exactly one pending animation frame');
    assert.deepStrictEqual(badCalls, [], 'drawing calls received NaN');
  });
}

test('the gallery links to every effect', () => {
  const gallery = fs.readFileSync(path.join(SRC, 'index.html'), 'utf8');
  assert.strictEqual(EFFECTS.length, 11);
  for (const file of EFFECTS) assert.ok(gallery.includes(`href="${file}"`), `${file} is not linked`);
});
