const { test } = require('node:test');
const assert = require('node:assert/strict');
const {
  WORLD_WIDTH,
  WORLD_HEIGHT,
  MAX_HEALTH,
  KILL_SCORE,
  ZOMBIE_DAMAGE,
  newGame,
  update,
  nextRandom,
  waveSize,
  zombieSpeed,
  distance,
} = require('../src/zombie_apocalypse.js');

const NO_INPUT = { moveX: 0, moveY: 0, aimX: 0, aimY: 0, fire: false };
const WAVE_BREAK = 3.0;
const PLAYER_SPEED = 7.0;

// A game with the player in the middle and no zombies, pickups or spawning.
function emptyField(seed = 1) {
  const game = newGame(seed);
  game.toSpawn = 0;
  game.zombies = [];
  game.pickups = [];
  game.pickupTimer = 1000;
  return game;
}

test('a new game starts with full health in wave one', () => {
  const game = newGame(42);
  assert.equal(game.health, MAX_HEALTH);
  assert.equal(game.wave, 1);
  assert.equal(game.toSpawn, waveSize(1));
  assert.equal(game.score, 0);
  assert.equal(game.gameOver, false);
});

test('the player moves and stays inside the world', () => {
  const game = emptyField();
  update(game, { ...NO_INPUT, moveX: -1 }, 1);
  assert.ok(Math.abs(game.player.x - (WORLD_WIDTH / 2 - PLAYER_SPEED)) < 1e-9);
  for (let i = 0; i < 100; i++) {
    update(game, { ...NO_INPUT, moveX: -1 }, 0.5);
  }
  assert.ok(Math.abs(game.player.x - 0.5) < 1e-9);
});

test('moving diagonally is not faster than moving straight', () => {
  const game = emptyField();
  update(game, { ...NO_INPUT, moveX: 1, moveY: 1 }, 1);
  const moved = distance(game.player, { x: WORLD_WIDTH / 2, y: WORLD_HEIGHT / 2 });
  assert.ok(Math.abs(moved - PLAYER_SPEED) < 1e-9);
});

test('a shot flies in the aim direction', () => {
  const game = emptyField();
  update(game, { ...NO_INPUT, fire: true, aimX: 0, aimY: -5 }, 0.01);
  assert.equal(game.bullets.length, 1);
  assert.ok(Math.abs(game.bullets[0].vel.x) < 1e-9);
  assert.ok(game.bullets[0].vel.y < 0);
});

test('with no aim, a shot flies in the last movement direction', () => {
  const game = emptyField();
  update(game, { ...NO_INPUT, moveY: 1 }, 0.01);
  update(game, { ...NO_INPUT, fire: true }, 0.01);
  assert.equal(game.bullets.length, 1);
  assert.ok(game.bullets[0].vel.y > 0);
});

test('the shot cooldown limits the fire rate', () => {
  const game = emptyField();
  const shoot = { ...NO_INPUT, fire: true, aimX: 1 };
  update(game, shoot, 0.01);
  update(game, shoot, 0.01);
  assert.equal(game.bullets.length, 1);
  for (let i = 0; i < 30; i++) {
    update(game, shoot, 0.01);
  }
  assert.ok(game.bullets.length >= 2);
});

test('a bullet that leaves the world is removed', () => {
  const game = emptyField();
  game.bullets = [{ pos: { x: WORLD_WIDTH - 0.1, y: 5 }, vel: { x: 18, y: 0 } }];
  update(game, NO_INPUT, 0.1);
  assert.deepEqual(game.bullets, []);
});

test('zombies appear on the edge of the world', () => {
  const game = newGame(7);
  game.spawnTimer = 0;
  update(game, NO_INPUT, 0); // dt 0: the zombie does not move after spawning
  assert.equal(game.zombies.length, 1);
  const { x, y } = game.zombies[0];
  assert.ok(x === 0 || x === WORLD_WIDTH || y === 0 || y === WORLD_HEIGHT);
  assert.equal(game.toSpawn, waveSize(1) - 1);
});

test('the same seed gives the same random numbers', () => {
  const a = { rng: 123 };
  const b = { rng: 123 };
  for (let i = 0; i < 100; i++) {
    const value = nextRandom(a);
    assert.ok(value >= 0 && value < 1);
    assert.equal(value, nextRandom(b));
  }
});

test('a zombie walks towards the player', () => {
  const game = emptyField();
  game.zombies = [{ x: 5, y: 10 }];
  const before = distance(game.zombies[0], game.player);
  update(game, NO_INPUT, 0.5);
  const after = distance(game.zombies[0], game.player);
  assert.ok(after < before);
  assert.ok(Math.abs(before - after - zombieSpeed(1) * 0.5) < 1e-9);
});

test('a bullet kills a zombie and scores', () => {
  const game = emptyField();
  game.zombies = [{ x: 20, y: 10 }];
  game.bullets = [{ pos: { x: 20, y: 10 }, vel: { x: 0, y: 0 } }];
  update(game, NO_INPUT, 0.01);
  assert.deepEqual(game.zombies, []);
  assert.deepEqual(game.bullets, []);
  assert.equal(game.score, KILL_SCORE);
});

test('touching a zombie costs health and removes the zombie', () => {
  const game = emptyField();
  game.zombies = [{ x: game.player.x + 0.5, y: game.player.y }];
  update(game, NO_INPUT, 0.01);
  assert.deepEqual(game.zombies, []);
  assert.equal(game.health, MAX_HEALTH - ZOMBIE_DAMAGE);
  assert.equal(game.gameOver, false);
});

test('zero health is game over, and the game stops', () => {
  const game = emptyField();
  game.health = ZOMBIE_DAMAGE;
  game.zombies = [{ x: game.player.x + 0.5, y: game.player.y }];
  update(game, NO_INPUT, 0.01);
  assert.equal(game.gameOver, true);
  assert.equal(game.health, 0);
  const x = game.player.x;
  update(game, { ...NO_INPUT, moveX: 1 }, 1);
  assert.equal(game.player.x, x);
});

test('a health pack heals, but not above the maximum', () => {
  const game = emptyField();
  game.health = MAX_HEALTH - 5;
  game.pickups = [{ ...game.player }];
  update(game, NO_INPUT, 0.01);
  assert.deepEqual(game.pickups, []);
  assert.equal(game.health, MAX_HEALTH);
});

test('clearing a wave starts a break, then a bigger and faster wave', () => {
  const game = emptyField();
  update(game, NO_INPUT, 0.01);
  assert.ok(game.waveDelay > 0);
  update(game, NO_INPUT, WAVE_BREAK + 0.1);
  assert.equal(game.wave, 2);
  assert.equal(game.toSpawn, waveSize(2));
  assert.ok(waveSize(2) > waveSize(1));
  assert.ok(zombieSpeed(2) > zombieSpeed(1));
});
