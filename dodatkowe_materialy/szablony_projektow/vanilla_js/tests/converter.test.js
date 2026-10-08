const { test } = require('node:test');
const assert = require('node:assert/strict');
const { celsiusToFahrenheit, fahrenheitToCelsius, convert } = require('../src/converter.js');

test('freezing and boiling points', () => {
  assert.equal(celsiusToFahrenheit(0), 32);
  assert.equal(celsiusToFahrenheit(100), 212);
});

test('minus forty is the same in both scales', () => {
  assert.equal(fahrenheitToCelsius(-40), -40);
});

test('convert returns the other unit', () => {
  const result = convert(212, 'f');
  assert.equal(result.unit, 'C');
  assert.ok(Math.abs(result.value - 100) < 1e-9);
});

test('unknown unit is rejected', () => {
  assert.throws(() => convert(10, 'K'));
});
