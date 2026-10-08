from text_editor import count_lines, count_words, find_next, line_col


def test_find_next_returns_first_match_after_start():
    assert find_next("foo bar foo", "foo", 0) == 0
    assert find_next("foo bar foo", "foo", 1) == 8


def test_find_next_wraps_around_to_the_beginning():
    assert find_next("foo bar", "foo", 1) == 0
    assert find_next("bar foo", "bar", 4) == 0


def test_find_next_reports_missing_text():
    assert find_next("foo bar", "baz", 0) == -1
    assert find_next("foo bar", "", 0) == -1


def test_line_col_counts_from_one():
    text = "ab\ncd"
    assert line_col(text, 0) == (1, 1)
    assert line_col(text, 2) == (1, 3)
    assert line_col(text, 3) == (2, 1)
    assert line_col(text, 5) == (2, 3)


def test_count_lines():
    assert count_lines("") == 1
    assert count_lines("a") == 1
    assert count_lines("a\n") == 1
    assert count_lines("a\n\nb") == 3


def test_count_words():
    assert count_words("") == 0
    assert count_words("one two\n  three\n\nfour ") == 4
