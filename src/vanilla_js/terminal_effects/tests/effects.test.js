import assert from 'node:assert/strict';
import { spawnSync } from 'node:child_process';
import { readdirSync } from 'node:fs';
import { test } from 'node:test';
import { fileURLToPath } from 'node:url';

import { rnd, seed } from '../src/term.js';

const SRC = fileURLToPath(new URL('../src/', import.meta.url));
const EFFECTS = readdirSync(SRC)
  .filter((f) => f.endsWith('.js') && f !== 'term.js')
  .map((f) => f.replace('.js', ''));

function run(name, frames) {
  const result = spawnSync(process.execPath, [SRC + `${name}.js`, String(frames)], {
    encoding: 'utf8',
    env: { ...process.env, NO_SLEEP: '1' },
    timeout: 120000,
    maxBuffer: 1 << 28,
  });
  assert.equal(result.status, 0, result.stderr);
  return result.stdout;
}

for (const name of EFFECTS) {
  test(`${name} draws frames and restores the terminal`, () => {
    const out = run(name, 20);
    assert.ok(out.startsWith('\x1b[?25l\x1b[2J'));
    assert.equal(out.split('\x1b[H').length - 1, 20);
    assert.ok(out.endsWith('\x1b[0m\x1b[?25h\n'));
  });
}

test('random numbers match C and Python', () => {
  seed(1);
  assert.deepEqual([0, 0, 0, 0, 0].map(() => rnd(100)), [38, 26, 13, 83, 19]);
});

test('maze solver finds a path', () => {
  assert.ok(run('maze_solver', 1500).includes(' shortest path: '));
});
