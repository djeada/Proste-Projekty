"""Expression logic: tokenizer, recursive-descent parser and evaluator. No input or output here."""
import math
from collections import namedtuple

DIGITS = '0123456789'
OPERATORS = '+-*/()'

Token = namedtuple('Token', 'kind value pos')


class CalculatorError(Exception):
    """Raised for every invalid expression; the message is shown to the user."""


def _is_digit(ch):
    return '0' <= ch <= '9'


def tokenize(text):
    tokens = []
    i = 0
    while i < len(text):
        ch = text[i]
        if ch.isspace():
            i += 1
        elif _is_digit(ch) or (ch == '.' and i + 1 < len(text) and _is_digit(text[i + 1])):
            end = i
            while end < len(text) and _is_digit(text[end]):
                end += 1
            if end < len(text) and text[end] == '.':
                end += 1
                while end < len(text) and _is_digit(text[end]):
                    end += 1
            tokens.append(Token('number', float(text[i:end]), i))
            i = end
        elif ch in OPERATORS:
            tokens.append(Token(ch, None, i))
            i += 1
        else:
            raise CalculatorError(f"Unexpected character '{ch}' at position {i + 1}")
    tokens.append(Token('end', None, len(text)))
    return tokens


class Parser:
    """Recursive descent: expression -> term -> factor. Each method returns the value of its part."""

    def __init__(self, text):
        self.text = text
        self.tokens = tokenize(text)
        self.index = 0

    def peek(self):
        return self.tokens[self.index]

    def advance(self):
        self.index += 1

    def expression(self):
        """expression := term (('+' | '-') term)*"""
        value = self.term()
        while self.peek().kind in ('+', '-'):
            op = self.peek().kind
            self.advance()
            right = self.term()
            value = value + right if op == '+' else value - right
        return value

    def term(self):
        """term := factor (('*' | '/') factor)*"""
        value = self.factor()
        while self.peek().kind in ('*', '/'):
            op = self.peek().kind
            self.advance()
            right = self.factor()
            if op == '*':
                value *= right
            else:
                if right == 0:
                    raise CalculatorError('Division by zero')
                value /= right
        return value

    def factor(self):
        """factor := '-' factor | number | '(' expression ')'"""
        tok = self.peek()
        if tok.kind == 'number':
            self.advance()
            return tok.value
        if tok.kind == '-':
            self.advance()
            return -self.factor()
        if tok.kind == '(':
            self.advance()
            value = self.expression()
            closing = self.peek()
            if closing.kind == 'end':
                raise CalculatorError('Unbalanced parentheses')
            if closing.kind != ')':
                raise self.unexpected(closing)
            self.advance()
            return value
        raise self.unexpected(tok)

    def unexpected(self, tok):
        if tok.kind == 'end':
            return CalculatorError('Unexpected end of expression')
        return CalculatorError(f"Unexpected '{self.text[tok.pos]}' at position {tok.pos + 1}")


def evaluate(text):
    """Returns the value of the expression as a float, or raises CalculatorError."""
    parser = Parser(text)
    if parser.peek().kind == 'end':
        raise CalculatorError('Empty expression')
    value = parser.expression()
    rest = parser.peek()
    if rest.kind == ')':
        raise CalculatorError('Unbalanced parentheses')
    if rest.kind != 'end':
        raise parser.unexpected(rest)
    if not math.isfinite(value):
        raise CalculatorError('Result is out of range')
    return value + 0.0  # turns -0.0 into 0.0 so it is never shown as "-0"
