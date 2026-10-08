const { test } = require('node:test');
const assert = require('node:assert/strict');
const { Timer, countdownMs, formatTime, formatLap, lapsText } = require('../src/timer.js');

test('formatTime shows minutes and seconds, hours only when needed', () => {
  assert.equal(formatTime(0), '00:00.000');
  assert.equal(formatTime(65432), '01:05.432');
  assert.equal(formatTime(3723004), '1:02:03.004');
});

test('formatLap and lapsText write one line per lap', () => {
  const lap = { split: 5120, total: 12300 };
  assert.equal(formatLap(2, lap), 'Lap 2: split 00:05.120  total 00:12.300');
  assert.equal(lapsText([lap]), 'Lap 1: split 00:05.120  total 00:12.300\n');
});

test('stopwatch pauses and resumes', () => {
  const timer = new Timer();
  timer.start(1000);
  assert.equal(timer.elapsed(1500), 500);
  timer.stop(2000);
  assert.equal(timer.elapsed(9000), 1000);
  timer.start(9000);
  assert.equal(timer.elapsed(9250), 1250);
});

test('start and stop ignore repeated calls', () => {
  const timer = new Timer();
  timer.stop(100);
  assert.equal(timer.elapsed(200), 0);
  timer.start(0);
  timer.start(500);
  assert.equal(timer.elapsed(1000), 1000);
});

test('laps have split and total time', () => {
  const timer = new Timer();
  assert.equal(timer.lap(0), null);
  timer.start(0);
  assert.deepEqual(timer.lap(1000), { split: 1000, total: 1000 });
  assert.deepEqual(timer.lap(1500), { split: 500, total: 1500 });
  timer.stop(2000);
  assert.equal(timer.lap(3000), null);
  assert.equal(timer.laps.length, 2);
});

test('reset clears time and laps', () => {
  const timer = new Timer();
  timer.start(0);
  timer.lap(100);
  timer.stop(200);
  timer.reset();
  assert.equal(timer.running, false);
  assert.deepEqual(timer.laps, []);
  assert.equal(timer.elapsed(500), 0);
});

test('countdown shows remaining time and finishes at zero', () => {
  const timer = new Timer(countdownMs(0, 3));
  assert.equal(timer.display(0), 3000);
  timer.start(0);
  assert.equal(timer.display(1000), 2000);
  assert.equal(timer.finished(2999), false);
  assert.equal(timer.finished(3000), true);
  assert.equal(timer.display(5000), 0);
});

test('countdown stops itself at zero and cannot restart until reset', () => {
  const timer = new Timer(2000);
  timer.start(0);
  timer.tick(1999);
  assert.equal(timer.running, true);
  timer.tick(2000);
  assert.equal(timer.running, false);
  timer.start(3000);
  assert.equal(timer.running, false);
  timer.reset();
  timer.start(4000);
  assert.equal(timer.running, true);
});

test('countdownMs validates minutes and seconds', () => {
  assert.equal(countdownMs(1, 30), 90000);
  assert.throws(() => countdownMs(0, 60), RangeError);
  assert.throws(() => countdownMs(NaN, 0), RangeError);
});
