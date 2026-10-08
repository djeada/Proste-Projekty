const { test } = require('node:test');
const assert = require('node:assert/strict');
const {
  Category, NUM_ROUNDS, scoreFor, upperTotal, upperBonus, cardTotal,
  newGame, isOver, rollDice, toggleHold, chooseCategory, winner,
} = require('../src/yahtzee.js');

const always6 = () => 6;

function scripted(faces) {
  let next = 0;
  return () => faces[next++];
}

function emptyCard() {
  return new Array(13).fill(null);
}

test('scoreFor gives the right points for each category', () => {
  const cases = [
    [[3, 3, 3, 5, 5], Category.FULL_HOUSE, 25],
    [[4, 4, 4, 4, 4], Category.FULL_HOUSE, 0],
    [[1, 2, 3, 4, 6], Category.SMALL_STRAIGHT, 30],
    [[1, 1, 2, 3, 3], Category.SMALL_STRAIGHT, 0],
    [[2, 3, 4, 5, 6], Category.LARGE_STRAIGHT, 40],
    [[1, 2, 3, 4, 6], Category.LARGE_STRAIGHT, 0],
    [[4, 4, 4, 4, 4], Category.YAHTZEE, 50],
    [[4, 4, 4, 4, 5], Category.YAHTZEE, 0],
    [[2, 2, 2, 5, 6], Category.THREE_OF_A_KIND, 17],
    [[2, 2, 3, 5, 6], Category.THREE_OF_A_KIND, 0],
    [[2, 2, 2, 2, 6], Category.FOUR_OF_A_KIND, 14],
    [[2, 2, 2, 3, 6], Category.FOUR_OF_A_KIND, 0],
    [[5, 5, 1, 2, 5], Category.FIVES, 15],
    [[3, 3, 1, 3, 6], Category.THREES, 9],
    [[6, 6, 5, 5, 4], Category.CHANCE, 26],
  ];
  for (const [dice, category, expected] of cases) {
    assert.equal(scoreFor(dice, category), expected, `dice ${dice} category ${category}`);
  }
});

test('the upper bonus needs 63 points in the upper section', () => {
  const card = [3, 6, 9, 12, 15, 18, null, null, null, null, null, null, null];
  assert.equal(upperTotal(card), 63);
  assert.equal(upperBonus(card), 35);
  card[Category.SIXES] = 17;
  assert.equal(upperTotal(card), 62);
  assert.equal(upperBonus(card), 0);
});

test('cardTotal adds the upper part, the bonus and the lower part', () => {
  const card = emptyCard();
  assert.equal(cardTotal(card), 0);
  card[Category.SIXES] = 30;
  card[Category.FULL_HOUSE] = 25;
  card[Category.CHANCE] = 22;
  assert.equal(cardTotal(card), 77);
});

test('rolling keeps the held dice', () => {
  const game = newGame(1);
  rollDice(game, scripted([1, 2, 3, 4, 5]));
  assert.deepEqual(game.dice, [1, 2, 3, 4, 5]);
  toggleHold(game, 0);
  toggleHold(game, 2);
  rollDice(game, scripted([6, 6, 6]));
  assert.deepEqual(game.dice, [1, 6, 3, 6, 6]);
});

test('a turn allows at most three rolls', () => {
  const game = newGame(1);
  assert.equal(rollDice(game, always6), true);
  assert.equal(rollDice(game, always6), true);
  assert.equal(rollDice(game, always6), true);
  assert.equal(rollDice(game, always6), false);
  assert.equal(game.rolls, 3);
});

test('holding and choosing need a roll first', () => {
  const game = newGame(1);
  assert.equal(toggleHold(game, 0), false);
  assert.equal(chooseCategory(game, Category.CHANCE), false);
  assert.equal(game.cards[0][Category.CHANCE], null);
});

test('choosing a category ends the turn and passes it to the next player', () => {
  const game = newGame(2);
  rollDice(game, always6);
  assert.equal(chooseCategory(game, Category.SIXES), true);
  assert.equal(game.cards[0][Category.SIXES], 30);
  assert.equal(game.current, 1);
  assert.equal(game.rolls, 0);
  assert.deepEqual(game.dice, [0, 0, 0, 0, 0]);

  rollDice(game, always6);
  chooseCategory(game, Category.CHANCE);
  assert.equal(game.current, 0);
  assert.equal(game.round, 2);
});

test('each player can use a category only once', () => {
  const game = newGame(2);
  rollDice(game, always6);
  chooseCategory(game, Category.YAHTZEE);
  rollDice(game, always6);
  chooseCategory(game, Category.CHANCE);
  rollDice(game, always6);
  assert.equal(chooseCategory(game, Category.YAHTZEE), false);
  assert.equal(game.current, 0);
  assert.equal(chooseCategory(game, Category.SIXES), true);
});

test('the game ends after 13 rounds', () => {
  const game = newGame(2);
  for (let turn = 0; turn < 2 * NUM_ROUNDS; turn++) {
    assert.equal(isOver(game), false);
    rollDice(game, always6);
    assert.equal(chooseCategory(game, Math.floor(turn / 2)), true);
  }
  assert.equal(isOver(game), true);
  assert.equal(rollDice(game, always6), false);
  assert.equal(chooseCategory(game, Category.CHANCE), false);
});

test('the winner is the player with the most points, first player on a tie', () => {
  const game = newGame(3);
  game.cards[0][Category.CHANCE] = 20;
  game.cards[1][Category.YAHTZEE] = 50;
  game.cards[2][Category.CHANCE] = 30;
  assert.equal(winner(game), 1);
  game.cards[2][Category.YAHTZEE] = 50;
  assert.equal(winner(game), 2);
  game.cards[2][Category.CHANCE] = 0;
  assert.equal(winner(game), 1);
});
