"""Tests of the chat rules: parsing, nicknames, line splitting and formatting."""
from messenger import (MAX_LINE, format_chat, format_join, format_leave, format_list, format_rename,
                       parse_line, parse_port, split_lines, valid_nick)


def test_chat_is_stripped():
    assert parse_line('  hello   there ') == ('chat', 'hello   there')


def test_blank_line_is_empty():
    assert parse_line('   ') == ('empty', '')


def test_commands():
    assert parse_line('/nick  Alice ') == ('nick', 'Alice')
    assert parse_line('/nick') == ('nick', '')
    assert parse_line('/list') == ('list', '')
    assert parse_line('/quit') == ('quit', '')


def test_unknown_commands_are_not_chat():
    assert parse_line('/nickname Bob')[0] == 'unknown'
    assert parse_line('/help')[0] == 'unknown'


def test_valid_nick():
    assert valid_nick('Alice')
    assert valid_nick('a_b-1')
    assert valid_nick('0123456789abcdef')  # exactly 16 characters
    assert not valid_nick('')
    assert not valid_nick('a b')
    assert not valid_nick('0123456789abcdefg')
    assert not valid_nick('<script>')


def test_parse_port():
    assert parse_port('8888') == 8888
    assert parse_port('65535') == 65535
    assert parse_port('0') is None
    assert parse_port('65536') is None
    assert parse_port('80a') is None
    assert parse_port('') is None


def test_split_lines_keeps_unfinished_rest():
    lines, rest = split_lines('one\r\ntwo\npart')
    assert lines == ['one', 'two']
    assert rest == 'part'


def test_split_lines_cuts_very_long_line():
    lines, rest = split_lines('x' * MAX_LINE)
    assert lines == ['x' * MAX_LINE]
    assert rest == ''


def test_format_chat_fits_line_limit():
    assert format_chat('Alice', 'hi') == '<Alice> hi'
    assert len(format_chat('Alice', 'x' * MAX_LINE)) == MAX_LINE - 1


def test_format_notices():
    assert format_join('Bob') == '* Bob joined'
    assert format_leave('Bob') == '* Bob left'
    assert format_rename('Bob', 'Carl') == '* Bob is now known as Carl'


def test_format_list():
    assert format_list(['Alice', 'Bob']) == '* Online: Alice, Bob'
    assert format_list([]) == '* Nobody is online'
