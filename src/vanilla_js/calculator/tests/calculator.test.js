const { test } = require('node:test');
const assert = require('node:assert/strict');
const { CalculatorError, tokenize, evaluate } = require('../src/calculator.js');

test('precedence: * and / before + and -', () => {
  assert.equal(evaluate('2 + 3 * 4'), 14);
  assert.equal(evaluate('2 + 3 * (4 - 1)'), 11);
  assert.equal(evaluate('2 * 3 + 4 / 2'), 8);
});

test('operators with the same precedence work left to right', () => {
  assert.equal(evaluate('10 - 4 - 3'), 3);
  assert.equal(evaluate('8 / 4 / 2'), 1);
});

test('parentheses change the order of evaluation', () => {
  assert.equal(evaluate('(1 + 2) * (3 + 4)'), 21);
  assert.equal(evaluate('((2))'), 2);
  assert.equal(evaluate('-(2 + 3)'), -5);
});

test('unary minus', () => {
  assert.equal(evaluate('-5 + 2'), -3);
  assert.equal(evaluate('2 * -3'), -6);
  assert.equal(evaluate('--4'), 4);
  assert.equal(evaluate('2 - -3'), 5);
});

test('decimal numbers', () => {
  assert.ok(Math.abs(evaluate('0.1 + 0.2') - 0.3) < 1e-9);
  assert.equal(evaluate('.5 * 4'), 2);
  assert.equal(evaluate('3. + 1'), 4);
  assert.equal(evaluate('7.5 / 2.5'), 3);
});

test('whitespace is ignored', () => {
  assert.equal(evaluate('  1+2\t'), 3);
});

test('tokenizer splits numbers and operators', () => {
  assert.deepEqual(tokenize('1.5*(2)').map((t) => t.kind), ['number', '*', '(', 'number', ')', 'end']);
  assert.equal(tokenize('1.5*(2)')[0].value, 1.5);
});

const errorCases = [
    ['', 'Empty expression'],
    ['   ', 'Empty expression'],
    ['1 / 0', 'Division by zero'],
    ['1 / (2 - 2)', 'Division by zero'],
    ['(1 + 2', 'Unbalanced parentheses'],
    ['(1 + (2)', 'Unbalanced parentheses'],
    ['1 + 2)', 'Unbalanced parentheses'],
    ['2 $ 3', "Unexpected character '$' at position 3"],
    ['1e5', "Unexpected character 'e' at position 2"],
    ['2 +', 'Unexpected end of expression'],
    ['2 3', "Unexpected '3' at position 3"],
    ['* 2', "Unexpected '*' at position 1"],
    ['()', "Unexpected ')' at position 2"],
    ['1.2.3', "Unexpected '.' at position 4"],
    ['9'.repeat(400), 'Result is out of range'],
];

for (const [text, message] of errorCases) {
  test(`error: ${JSON.stringify(text).slice(0, 30)}`, () => {
    assert.throws(() => evaluate(text), { message });
  });
}

test('errors are CalculatorError instances', () => {
  assert.throws(() => evaluate('1 / 0'), CalculatorError);
});
