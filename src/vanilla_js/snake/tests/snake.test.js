const { test } = require('node:test');
const assert = require('node:assert/strict');
const { WIDTH, HEIGHT, UP, DOWN, LEFT, RIGHT, SnakeGame } = require('../src/snake.js');

const firstFree = () => 0;
const lastFree = (n) => n - 1;

function makeGame(body, direction = RIGHT, randomBelow = firstFree) {
  const game = new SnakeGame(randomBelow);
  game.body = body.map(([x, y]) => ({ x, y }));
  game.direction = direction;
  game.nextDirection = direction;
  return game;
}

const cells = (game) => game.body.map((part) => [part.x, part.y]);

test('a new game starts in the middle, moving right', () => {
  const game = new SnakeGame(firstFree);
  assert.deepEqual(cells(game), [[10, 7]]);
  assert.equal(game.direction, RIGHT);
  assert.equal(game.score, 0);
  assert.equal(game.gameOver, false);
});

test('food is placed by the random choice', () => {
  assert.deepEqual(new SnakeGame(firstFree).food, { x: 0, y: 0 });
  assert.deepEqual(new SnakeGame(lastFree).food, { x: WIDTH - 1, y: HEIGHT - 1 });
});

test('food is never placed on the snake', () => {
  const game = makeGame([[5, 5], [4, 5], [3, 5]]);
  game.placeFood();
  assert.equal(game.body.some((part) => part.x === game.food.x && part.y === game.food.y), false);
});

test('a step moves the head one cell', () => {
  const game = new SnakeGame(firstFree);
  game.step();
  assert.deepEqual(cells(game), [[11, 7]]);
});

test('a turn at a right angle is accepted', () => {
  const game = new SnakeGame(firstFree);
  game.turn(UP);
  game.step();
  assert.deepEqual(cells(game), [[10, 6]]);
});

test('the snake cannot reverse into its neck', () => {
  const game = new SnakeGame(firstFree);
  game.turn(LEFT);
  game.step();
  assert.equal(game.direction, RIGHT);
  assert.deepEqual(cells(game), [[11, 7]]);
});

test('two turns in one step cannot reverse the snake', () => {
  const game = new SnakeGame(firstFree);
  game.turn(UP);
  game.turn(LEFT); // reverses the last step, so it is ignored
  game.step();
  assert.deepEqual(cells(game), [[10, 6]]);
  assert.equal(game.gameOver, false);
});

test('hitting the wall ends the game', () => {
  const game = makeGame([[0, 5]], LEFT);
  game.step();
  assert.equal(game.gameOver, true);
});

test('hitting itself ends the game', () => {
  const game = makeGame([[5, 5], [4, 5], [4, 6], [5, 6]], DOWN);
  game.step();
  assert.equal(game.gameOver, true);
});

test('eating food grows the snake and adds points', () => {
  const game = makeGame([[5, 5]]);
  game.food = { x: 6, y: 5 };
  game.step();
  assert.deepEqual(cells(game), [[6, 5], [5, 5]]);
  assert.equal(game.score, 10);
  assert.equal(game.gameOver, false);
});

test('moving without food keeps the length', () => {
  const game = makeGame([[5, 5], [4, 5], [3, 5]]);
  game.food = { x: 0, y: 0 };
  game.step();
  assert.deepEqual(cells(game), [[6, 5], [5, 5], [4, 5]]);
});

test('the game gets faster as the snake grows, down to 60 ms', () => {
  const game = makeGame([[5, 5]]);
  assert.equal(game.delayMs(), 150);
  game.body = Array.from({ length: 10 }, (_, i) => ({ x: i, y: 0 }));
  assert.equal(game.delayMs(), 105);
  game.body = Array.from({ length: 40 }, (_, i) => ({ x: i % WIDTH, y: 0 }));
  assert.equal(game.delayMs(), 60);
});

test('reset clears the game', () => {
  const game = makeGame([[5, 5], [4, 5]], DOWN);
  game.score = 30;
  game.gameOver = true;
  game.reset();
  assert.deepEqual(cells(game), [[10, 7]]);
  assert.equal(game.score, 0);
  assert.equal(game.gameOver, false);
});
