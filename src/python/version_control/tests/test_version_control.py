import pytest

import version_control as vcs


def test_init_twice_fails(tmp_path):
    assert not vcs.repo_exists(tmp_path)
    vcs.init(tmp_path)
    assert vcs.repo_exists(tmp_path)
    with pytest.raises(vcs.VcsError):
        vcs.init(tmp_path)


def test_commands_fail_outside_a_repository(tmp_path):
    with pytest.raises(vcs.VcsError):
        vcs.commit_count(tmp_path)


def test_commits_are_numbered_and_store_message(tmp_path):
    vcs.init(tmp_path)
    (tmp_path / "a.txt").write_text("one\n")
    assert vcs.commit(tmp_path, "first") == 1
    (tmp_path / "a.txt").write_text("two\n")
    assert vcs.commit(tmp_path, "second") == 2
    assert vcs.commit_count(tmp_path) == 2

    stamp, message = vcs.commit_info(tmp_path, 2)
    assert message == "second"
    assert stamp > 0


def test_commit_ignores_subdirectories(tmp_path):
    vcs.init(tmp_path)
    (tmp_path / "top.txt").write_text("x")
    (tmp_path / "sub").mkdir()
    (tmp_path / "sub" / "inner.txt").write_text("y")

    vcs.commit(tmp_path, "only top")
    assert vcs.load_commit(tmp_path, 1) == {"top.txt": b"x"}


def test_commit_rejects_multi_line_message(tmp_path):
    vcs.init(tmp_path)
    (tmp_path / "a.txt").write_text("x")
    with pytest.raises(vcs.VcsError):
        vcs.commit(tmp_path, "two\nlines")
    assert vcs.commit_count(tmp_path) == 0


def test_changes_between_snapshots(tmp_path):
    vcs.init(tmp_path)
    (tmp_path / "keep.txt").write_text("same")
    (tmp_path / "edit.txt").write_text("old")
    (tmp_path / "gone.txt").write_text("bye")
    vcs.commit(tmp_path, "base")
    base = vcs.load_commit(tmp_path, 1)

    (tmp_path / "edit.txt").write_text("new")
    (tmp_path / "gone.txt").unlink()
    (tmp_path / "added.txt").write_text("hi")
    current = vcs.read_directory(tmp_path)

    assert vcs.changes(base, current) == [
        ("added", "added.txt"),
        ("modified", "edit.txt"),
        ("deleted", "gone.txt"),
    ]
    assert vcs.changes(base, base) == []


def test_checkout_restores_files(tmp_path):
    vcs.init(tmp_path)
    (tmp_path / "data.txt").write_text("version 1")
    vcs.commit(tmp_path, "v1")
    (tmp_path / "data.txt").write_text("version 2")
    vcs.commit(tmp_path, "v2")

    vcs.checkout(tmp_path, 1)
    assert (tmp_path / "data.txt").read_text() == "version 1"
    with pytest.raises(vcs.VcsError):
        vcs.checkout(tmp_path, 9)


def test_split_lines():
    assert vcs.split_lines(b"") == []
    assert vcs.split_lines(b"\n") == [""]
    assert vcs.split_lines(b"a\nb") == ["a", "b"]
    assert vcs.split_lines(b"a\nb\n") == ["a", "b"]


def test_diff_marks_changed_lines():
    old = ["a", "b", "c"]
    new = ["a", "x", "c"]
    assert vcs.diff_lines(old, new) == [(" ", "a"), ("-", "b"), ("+", "x"), (" ", "c")]


def test_diff_of_added_and_deleted_file():
    assert vcs.diff_lines([], ["one", "two"]) == [("+", "one"), ("+", "two")]
    assert vcs.diff_lines(["one"], []) == [("-", "one")]


def test_diff_keeps_order_of_insertions_and_deletions():
    old = ["x", "a", "y"]
    new = ["a", "z"]
    assert vcs.diff_lines(old, new) == [("-", "x"), (" ", "a"), ("-", "y"), ("+", "z")]


def test_format_time_shape():
    text = vcs.format_time(0)
    assert len(text) == 19
    assert text[4] == "-" and text[10] == " " and text[13] == ":"
