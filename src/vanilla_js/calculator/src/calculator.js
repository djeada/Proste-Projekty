// Expression logic: tokenizer, recursive-descent parser and evaluator. No DOM here.

const DIGITS = '0123456789';
const OPERATORS = '+-*/()';

class CalculatorError extends Error {}

function isDigit(ch) {
  return ch >= '0' && ch <= '9';
}

function tokenize(text) {
  const tokens = [];
  let i = 0;
  while (i < text.length) {
    const ch = text[i];
    if (/\s/.test(ch)) {
      i += 1;
    } else if (isDigit(ch) || (ch === '.' && isDigit(text[i + 1]))) {
      let end = i;
      while (isDigit(text[end])) end += 1;
      if (text[end] === '.') {
        end += 1;
        while (isDigit(text[end])) end += 1;
      }
      tokens.push({ kind: 'number', value: parseFloat(text.slice(i, end)), pos: i });
      i = end;
    } else if (OPERATORS.includes(ch)) {
      tokens.push({ kind: ch, value: null, pos: i });
      i += 1;
    } else {
      throw new CalculatorError(`Unexpected character '${ch}' at position ${i + 1}`);
    }
  }
  tokens.push({ kind: 'end', value: null, pos: text.length });
  return tokens;
}

class Parser {
  // Recursive descent: expression -> term -> factor. Each method returns the value of its part.
  constructor(text) {
    this.text = text;
    this.tokens = tokenize(text);
    this.index = 0;
  }

  peek() {
    return this.tokens[this.index];
  }

  advance() {
    this.index += 1;
  }

  // expression := term (('+' | '-') term)*
  expression() {
    let value = this.term();
    while (this.peek().kind === '+' || this.peek().kind === '-') {
      const op = this.peek().kind;
      this.advance();
      const right = this.term();
      value = op === '+' ? value + right : value - right;
    }
    return value;
  }

  // term := factor (('*' | '/') factor)*
  term() {
    let value = this.factor();
    while (this.peek().kind === '*' || this.peek().kind === '/') {
      const op = this.peek().kind;
      this.advance();
      const right = this.factor();
      if (op === '*') {
        value *= right;
      } else {
        if (right === 0) throw new CalculatorError('Division by zero');
        value /= right;
      }
    }
    return value;
  }

  // factor := '-' factor | number | '(' expression ')'
  factor() {
    const tok = this.peek();
    if (tok.kind === 'number') {
      this.advance();
      return tok.value;
    }
    if (tok.kind === '-') {
      this.advance();
      return -this.factor();
    }
    if (tok.kind === '(') {
      this.advance();
      const value = this.expression();
      const closing = this.peek();
      if (closing.kind === 'end') throw new CalculatorError('Unbalanced parentheses');
      if (closing.kind !== ')') throw this.unexpected(closing);
      this.advance();
      return value;
    }
    throw this.unexpected(tok);
  }

  unexpected(tok) {
    if (tok.kind === 'end') return new CalculatorError('Unexpected end of expression');
    return new CalculatorError(`Unexpected '${this.text[tok.pos]}' at position ${tok.pos + 1}`);
  }
}

function evaluate(text) {
  const parser = new Parser(text);
  if (parser.peek().kind === 'end') throw new CalculatorError('Empty expression');
  const value = parser.expression();
  const rest = parser.peek();
  if (rest.kind === ')') throw new CalculatorError('Unbalanced parentheses');
  if (rest.kind !== 'end') throw parser.unexpected(rest);
  if (!Number.isFinite(value)) throw new CalculatorError('Result is out of range');
  return value + 0; // turns -0 into 0 so it is never shown as "-0"
}

if (typeof module !== 'undefined') module.exports = { CalculatorError, tokenize, evaluate };
