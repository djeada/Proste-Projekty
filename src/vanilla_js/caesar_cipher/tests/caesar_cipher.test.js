const { test } = require('node:test');
const assert = require('node:assert/strict');
const { shiftChar, encrypt, decrypt, crack } = require('../src/caesar_cipher.js');

const SENTENCE = 'The quick brown fox jumps over the lazy dog and then runs away into the forest.';

test('shiftChar moves letters and wraps around', () => {
  assert.equal(shiftChar('a', 3), 'd');
  assert.equal(shiftChar('z', 1), 'a');
  assert.equal(shiftChar('Z', 1), 'A');
  assert.equal(shiftChar('a', -1), 'z');
});

test('encrypt keeps case and leaves other characters unchanged', () => {
  assert.equal(encrypt('Hello, World!', 3), 'Khoor, Zruog!');
  assert.equal(encrypt('123 .,?', 5), '123 .,?');
  assert.equal(encrypt('ąęśćżźłóń', 7), 'ąęśćżźłóń');
  assert.equal(encrypt('zażółć', 1), 'abżółć');
});

test('negative and large keys', () => {
  assert.equal(encrypt('abc', -1), 'zab');
  assert.equal(encrypt('abc', 26), 'abc');
  assert.equal(encrypt('abc', 27), 'bcd');
  assert.equal(encrypt('abc', 52), 'abc');
  assert.equal(encrypt('abc', -27), 'zab');
});

test('decrypt reverses encrypt', () => {
  assert.equal(decrypt('Khoor, Zruog!', 3), 'Hello, World!');
});

test('round trip for many keys', () => {
  for (const key of [-30, -3, 0, 1, 7, 25, 52, 100]) {
    assert.equal(decrypt(encrypt(SENTENCE, key), key), SENTENCE);
  }
});

test('crack finds the key and the text', () => {
  assert.deepEqual(crack(encrypt(SENTENCE, 7)), { key: 7, text: SENTENCE });
});

test('crack returns the text unchanged when it has no letters', () => {
  assert.deepEqual(crack('1234 !?'), { key: 0, text: '1234 !?' });
});
