const { test } = require('node:test');
const assert = require('node:assert/strict');
const {
  MAX_MISSES, WORD_LIST, GUESS, STATUS, chooseEntry, createGame, applyGuess, maskedWord, gameStatus,
} = require('../src/hangman.js');

const banana = () => createGame({ category: 'Fruit', word: 'banana' });

test('chooseEntry uses the random source', () => {
  const list = ['a', 'b', 'c'];
  assert.equal(chooseEntry(list, () => 0), 'a');
  assert.equal(chooseEntry(list, () => 0.99), 'c');
  assert.ok(WORD_LIST.includes(chooseEntry(WORD_LIST)));
});

test('word list has only lowercase letters', () => {
  for (const { word } of WORD_LIST) assert.match(word, /^[a-z]+$/);
});

test('a new game is fully masked', () => {
  const game = createGame({ category: 'Fruit', word: 'lemon' });
  assert.equal(maskedWord(game), '_ _ _ _ _');
  assert.equal(gameStatus(game), STATUS.PLAYING);
});

test('a hit reveals all occurrences', () => {
  const game = banana();
  assert.equal(applyGuess(game, 'a'), GUESS.HIT);
  assert.equal(maskedWord(game), '_ a _ a _ a');
  assert.equal(game.misses, 0);
});

test('a miss costs one attempt', () => {
  const game = banana();
  assert.equal(applyGuess(game, 'z'), GUESS.MISS);
  assert.equal(game.misses, 1);
});

test('a repeated guess is ignored', () => {
  const game = banana();
  applyGuess(game, 'b');
  applyGuess(game, 'q');
  assert.equal(applyGuess(game, 'b'), GUESS.REPEAT);
  assert.equal(applyGuess(game, 'q'), GUESS.REPEAT);
  assert.equal(game.misses, 1);
});

test('invalid guesses are rejected and uppercase letters count as lowercase', () => {
  const game = banana();
  for (const text of ['1', ' ', '', 'ab']) assert.equal(applyGuess(game, text), GUESS.INVALID);
  assert.equal(applyGuess(game, 'B'), GUESS.HIT);
  assert.equal(applyGuess(game, 'b'), GUESS.REPEAT);
  assert.equal(game.misses, 0);
});

test('the game is won when every letter is guessed', () => {
  const game = banana();
  for (const letter of 'ban') applyGuess(game, letter);
  assert.equal(gameStatus(game), STATUS.WON);
});

test('the game is still playing before the word is complete', () => {
  const game = banana();
  applyGuess(game, 'b');
  applyGuess(game, 'a');
  assert.equal(gameStatus(game), STATUS.PLAYING);
});

test('the game is lost after the maximum number of misses', () => {
  const game = banana();
  const wrong = 'cdefgh';
  for (const letter of wrong.slice(0, MAX_MISSES - 1)) applyGuess(game, letter);
  assert.equal(gameStatus(game), STATUS.PLAYING);
  applyGuess(game, wrong[MAX_MISSES - 1]);
  assert.equal(game.misses, MAX_MISSES);
  assert.equal(gameStatus(game), STATUS.LOST);
});
