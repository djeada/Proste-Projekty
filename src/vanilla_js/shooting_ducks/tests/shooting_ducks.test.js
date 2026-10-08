const { test } = require('node:test');
const assert = require('node:assert/strict');
const {
  FIELD_WIDTH, DUCK_HALF_WIDTH, START_LIVES, WAVE_BREAK_SECONDS,
  createRng, nextRandom, ducksInWave, createGame, updateGame, shootAt,
} = require('../src/shooting_ducks.js');

function spawnFirstDuck(game) {
  updateGame(game, 0.001);
  return game.ducks[0];
}

test('a new game starts in wave 1 with three lives', () => {
  const game = createGame(42);
  assert.equal(game.wave, 1);
  assert.equal(game.score, 0);
  assert.equal(game.lives, START_LIVES);
  assert.deepEqual(game.ducks, []);
  assert.equal(game.ducksToSpawn, ducksInWave(1));
  assert.equal(game.gameOver, false);
});

test('the random source repeats for the same seed and stays in [0, 1)', () => {
  const a = createRng(7);
  const b = createRng(7);
  for (let i = 0; i < 1000; i++) {
    const value = nextRandom(a);
    assert.equal(value, nextRandom(b));
    assert.ok(value >= 0 && value < 1);
  }
});

test('a wave spawns all of its ducks over time', () => {
  const game = createGame(3);
  for (let i = 0; i < 100 && game.ducksToSpawn > 0; i++) updateGame(game, 0.1);
  assert.equal(game.ducksToSpawn, 0);
  assert.ok(game.ducks.length > 0);
});

test('movement is proportional to the elapsed time', () => {
  const game = createGame(5);
  const duck = spawnFirstDuck(game);
  const { x, speed } = duck;
  updateGame(game, 0.5);
  assert.ok(Math.abs(game.ducks[0].x - (x + speed * 0.5)) < 1e-9);
});

test('shooting hits a duck under the point and misses elsewhere', () => {
  const game = createGame(5);
  const duck = spawnFirstDuck(game);
  assert.equal(shootAt(game, duck.x + 50, duck.y), false);
  assert.equal(game.score, 0);
  assert.equal(game.ducks.length, 1);

  assert.equal(shootAt(game, duck.x, duck.y), true);
  assert.equal(game.score, 10);
  assert.deepEqual(game.ducks, []);
});

test('an escaped duck costs one life', () => {
  const game = createGame(9);
  const duck = spawnFirstDuck(game);
  duck.speed = 1;
  duck.x = FIELD_WIDTH + DUCK_HALF_WIDTH + 1;
  updateGame(game, 0.001);
  assert.equal(game.lives, START_LIVES - 1);
  assert.deepEqual(game.ducks, []);
  assert.equal(game.gameOver, false);
});

test('losing all lives ends the game and stops the play', () => {
  const game = createGame(9);
  game.ducksToSpawn = 0;
  for (let i = 0; i < START_LIVES; i++) {
    game.breakTimer = 0;
    game.ducks = [{ x: -DUCK_HALF_WIDTH - 1, y: 10, baseY: 10, speed: -1, age: 0, phase: 0 }];
    updateGame(game, 0.001);
  }
  assert.equal(game.gameOver, true);
  assert.equal(game.lives, 0);

  const score = game.score;
  updateGame(game, 1);
  assert.equal(shootAt(game, 0, 0), false);
  assert.equal(game.score, score);
});

test('clearing a wave pauses, then starts the next wave', () => {
  const game = createGame(11);
  game.ducksToSpawn = 0;
  game.ducks = [];
  updateGame(game, 0.01);
  assert.ok(game.breakTimer > 0);

  updateGame(game, WAVE_BREAK_SECONDS);
  assert.equal(game.wave, 2);
  assert.equal(game.ducksToSpawn, ducksInWave(2));
  assert.equal(game.breakTimer, 0);
});

test('later waves have more ducks and faster ones', () => {
  assert.ok(ducksInWave(2) > ducksInWave(1));
  const early = spawnFirstDuck(createGame(21));
  const lateGame = createGame(21);
  lateGame.wave = 5;
  const late = spawnFirstDuck(lateGame);
  assert.ok(Math.abs(late.speed) > Math.abs(early.speed));
});

test('ducks fly in both directions', () => {
  const game = createGame(1234);
  const directions = new Set();
  for (let i = 0; i < 40; i++) {
    game.ducks = [];
    game.ducksToSpawn = 1;
    game.spawnTimer = 0;
    updateGame(game, 0.001);
    directions.add(game.ducks[0].speed > 0);
  }
  assert.deepEqual([...directions].sort(), [false, true]);
});
