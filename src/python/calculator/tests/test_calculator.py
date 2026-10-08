import re

import pytest

from calculator import CalculatorError, evaluate, tokenize


@pytest.mark.parametrize(
    'text, expected',
    [
        ('2 + 3 * 4', 14),
        ('2 + 3 * (4 - 1)', 11),
        ('10 - 4 - 3', 3),
        ('8 / 4 / 2', 1),
        ('(1 + 2) * (3 + 4)', 21),
        ('((2))', 2),
        ('-(2 + 3)', -5),
        ('-5 + 2', -3),
        ('2 * -3', -6),
        ('--4', 4),
        ('2 - -3', 5),
        ('0.1 + 0.2', 0.3),
        ('.5 * 4', 2),
        ('3. + 1', 4),
        ('  1+2\t', 3),
    ],
)
def test_valid_expressions(text, expected):
    assert evaluate(text) == pytest.approx(expected)


@pytest.mark.parametrize(
    'text, message',
    [
        ('', 'Empty expression'),
        ('   ', 'Empty expression'),
        ('1 / 0', 'Division by zero'),
        ('1 / (2 - 2)', 'Division by zero'),
        ('(1 + 2', 'Unbalanced parentheses'),
        ('(1 + (2)', 'Unbalanced parentheses'),
        ('1 + 2)', 'Unbalanced parentheses'),
        ('2 $ 3', "Unexpected character '$' at position 3"),
        ('1e5', "Unexpected character 'e' at position 2"),
        ('2 +', 'Unexpected end of expression'),
        ('2 3', "Unexpected '3' at position 3"),
        ('* 2', "Unexpected '*' at position 1"),
        ('()', "Unexpected ')' at position 2"),
        ('1.2.3', "Unexpected '.' at position 4"),
        ('9' * 400, 'Result is out of range'),
    ],
)
def test_invalid_expressions(text, message):
    with pytest.raises(CalculatorError, match=re.escape(message)):
        evaluate(text)


def test_tokenizer_splits_numbers_and_operators():
    kinds = [tok.kind for tok in tokenize('1.5*(2)')]
    assert kinds == ['number', '*', '(', 'number', ')', 'end']
    assert tokenize('1.5*(2)')[0].value == 1.5
