import json
from pathlib import Path

import pytest

from http_server import (
    MAX_NOTES,
    NotesStore,
    ParseError,
    Request,
    handle,
    mime_type,
    parse_request,
)

PUBLIC = Path(__file__).resolve().parent.parent / "public"


def send(method, path, body="", store=None):
    """Handle a request whose body is plain text."""
    request = Request(method, path, {}, body.encode("utf-8"))
    return handle(request, PUBLIC, store or NotesStore())


def test_parse_request_with_headers_and_body():
    raw = b"POST /api/notes?x=1 HTTP/1.1\r\nHost: localhost\r\nContent-Length: 5\r\n\r\nhello"
    request = parse_request(raw)
    assert request.method == "POST"
    assert request.path == "/api/notes"
    assert request.headers["host"] == "localhost"
    assert request.body == b"hello"


def test_parse_request_waits_for_the_whole_body():
    raw = b"POST /api/notes HTTP/1.1\r\nContent-Length: 10\r\n\r\nhello"
    assert parse_request(raw) is None


def test_parse_request_waits_for_the_end_of_headers():
    assert parse_request(b"GET / HTTP/1.1\r\nHost: x") is None


def test_parse_request_rejects_a_bad_request_line():
    with pytest.raises(ParseError):
        parse_request(b"INVALID\r\n\r\n")


def test_parse_request_rejects_a_bad_content_length():
    with pytest.raises(ParseError):
        parse_request(b"POST / HTTP/1.1\r\nContent-Length: abc\r\n\r\n")


def test_mime_type_is_chosen_by_extension():
    assert mime_type("index.html") == "text/html; charset=utf-8"
    assert mime_type("style.CSS") == "text/css; charset=utf-8"
    assert mime_type("logo.png") == "image/png"
    assert mime_type("archive.bin") == "application/octet-stream"


def test_root_serves_index_html():
    response = send("GET", "/")
    assert response.status == 200
    assert response.content_type == "text/html; charset=utf-8"
    assert b"<title>Notes</title>" in response.body


def test_missing_file_is_404():
    assert send("GET", "/missing.txt").status == 404


def test_path_that_escapes_the_public_directory_is_403():
    for path in ("/../README.md", "/%2e%2e/README.md", "/css/../../README.md", "/a\\..\\b"):
        assert send("GET", path).status == 403, path


def test_static_files_only_accept_get():
    assert send("POST", "/index.html").status == 405


def test_notes_start_empty():
    response = send("GET", "/api/notes")
    assert response.status == 200
    assert json.loads(response.body) == []


def test_post_creates_a_note_with_201():
    store = NotesStore()
    response = send("POST", "/api/notes", "  buy milk  ", store)
    assert response.status == 201
    assert json.loads(response.body) == {"id": 1, "text": "buy milk"}
    assert store.all() == [{"id": 1, "text": "buy milk"}]


def test_put_changes_a_note_and_delete_removes_it():
    store = NotesStore()
    send("POST", "/api/notes", "first", store)
    assert send("PUT", "/api/notes/1", "changed", store).status == 200
    assert store.all() == [{"id": 1, "text": "changed"}]
    assert send("DELETE", "/api/notes/1", store=store).status == 204
    assert store.all() == []


def test_ids_are_not_reused_after_delete():
    store = NotesStore()
    send("POST", "/api/notes", "a", store)
    send("DELETE", "/api/notes/1", store=store)
    assert json.loads(send("POST", "/api/notes", "b", store).body)["id"] == 2


def test_store_full_is_507():
    store = NotesStore()
    for _ in range(MAX_NOTES):
        send("POST", "/api/notes", "x", store)
    assert send("POST", "/api/notes", "one more", store).status == 507


def test_unknown_note_is_404():
    assert send("PUT", "/api/notes/9", "x").status == 404
    assert send("DELETE", "/api/notes/9").status == 404
    assert send("PUT", "/api/notes/abc", "x").status == 404


def test_invalid_note_bodies_are_400():
    assert send("POST", "/api/notes", "   ").status == 400
    assert send("POST", "/api/notes", "").status == 400
    assert send("POST", "/api/notes", "x" * 201).status == 400
    assert send("POST", "/api/notes", "x" * 200).status == 201
    assert send("PUT", "/api/notes/1", "  ").status == 400
    bad_utf8 = Request("POST", "/api/notes", {}, b"\xff\xfe")
    assert handle(bad_utf8, PUBLIC, NotesStore()).status == 400


def test_text_is_escaped_in_the_json_answer():
    response = send("POST", "/api/notes", 'say "hi"\\ now')
    assert json.loads(response.body) == {"id": 1, "text": 'say "hi"\\ now'}


def test_wrong_method_on_api_is_405():
    assert send("DELETE", "/api/notes").status == 405
    assert send("POST", "/api/notes/1", "x").status == 405


def test_unknown_api_path_is_404():
    assert send("GET", "/api/notesx").status == 404
    assert send("GET", "/api/notes/1/2").status == 404


def test_response_bytes_have_status_line_and_length():
    raw = send("GET", "/about.json").to_bytes()
    head, _, body = raw.partition(b"\r\n\r\n")
    assert head.startswith(b"HTTP/1.1 200 OK\r\n")
    assert b"Content-Type: application/json; charset=utf-8" in head
    assert f"Content-Length: {len(body)}".encode() in head


def test_204_response_has_no_content_headers():
    store = NotesStore()
    send("POST", "/api/notes", "a", store)
    raw = send("DELETE", "/api/notes/1", store=store).to_bytes()
    assert raw == b"HTTP/1.1 204 No Content\r\nConnection: close\r\n\r\n"
