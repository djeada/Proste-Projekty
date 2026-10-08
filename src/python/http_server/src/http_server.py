"""HTTP logic: request parsing, MIME types, safe paths, the notes store and routing. No sockets here."""
import json
from dataclasses import dataclass
from pathlib import Path
from typing import Dict, List, Optional

MAX_REQUEST_BYTES = 65536
MAX_NOTE_BYTES = 200
MAX_NOTES = 100
NOTES_PATH = "/api/notes"
BAD_TEXT = "body must be 1 to 200 bytes of plain text"

REASONS = {
    200: "OK",
    201: "Created",
    204: "No Content",
    400: "Bad Request",
    403: "Forbidden",
    404: "Not Found",
    405: "Method Not Allowed",
    507: "Insufficient Storage",
}

MIME_TYPES = {
    ".html": "text/html; charset=utf-8",
    ".css": "text/css; charset=utf-8",
    ".js": "text/javascript; charset=utf-8",
    ".json": "application/json; charset=utf-8",
    ".txt": "text/plain; charset=utf-8",
    ".png": "image/png",
    ".jpg": "image/jpeg",
    ".gif": "image/gif",
    ".svg": "image/svg+xml",
}
JSON_TYPE = "application/json; charset=utf-8"


class ParseError(Exception):
    """The bytes received are not a valid HTTP request."""


@dataclass
class Request:
    method: str
    path: str  # the request target without the query string
    headers: Dict[str, str]  # header names in lower case
    body: bytes


@dataclass
class Response:
    status: int
    body: bytes = b""
    content_type: Optional[str] = None

    def to_bytes(self) -> bytes:
        lines = [f"HTTP/1.1 {self.status} {REASONS[self.status]}", "Connection: close"]
        if self.status != 204:  # 204 No Content has no body and no Content-Type
            lines += [f"Content-Type: {self.content_type}", f"Content-Length: {len(self.body)}"]
        return "\r\n".join(lines).encode("latin-1") + b"\r\n\r\n" + self.body


def parse_request(raw: bytes) -> Optional[Request]:
    """Return the request, None if more bytes are needed, or raise ParseError."""
    if len(raw) > MAX_REQUEST_BYTES:
        raise ParseError("request too large")
    end = raw.find(b"\r\n\r\n")
    if end < 0:
        return None

    lines = raw[:end].decode("latin-1").split("\r\n")
    parts = lines[0].split(" ")
    if len(parts) != 3 or not parts[2].startswith("HTTP/"):
        raise ParseError(f"bad request line: {lines[0]!r}")
    method, target, _version = parts

    headers = {}
    for line in lines[1:]:
        name, colon, value = line.partition(":")
        if not colon or not name.strip():
            raise ParseError(f"bad header: {line!r}")
        headers[name.strip().lower()] = value.strip()

    try:
        length = int(headers.get("content-length", "0"))
    except ValueError:
        raise ParseError("bad Content-Length") from None
    if length < 0:
        raise ParseError("bad Content-Length")

    body_start = end + 4
    if len(raw) < body_start + length:
        return None
    body = raw[body_start:body_start + length]
    return Request(method, target.split("?", 1)[0], headers, body)


def mime_type(file_name: str) -> str:
    suffix = Path(file_name).suffix.lower()
    return MIME_TYPES.get(suffix, "application/octet-stream")


def json_response(status: int, data: object) -> Response:
    return Response(status, json.dumps(data).encode("utf-8"), JSON_TYPE)


def error_response(status: int, message: str) -> Response:
    return json_response(status, {"error": message})


class NotesStore:
    """Notes kept in memory, numbered 1, 2, 3, ... and never reused."""

    def __init__(self) -> None:
        self._texts: Dict[int, str] = {}
        self._next_id = 1

    def is_full(self) -> bool:
        return len(self._texts) >= MAX_NOTES

    def all(self) -> List[dict]:
        return [{"id": note_id, "text": text} for note_id, text in self._texts.items()]

    def add(self, text: str) -> dict:
        note_id = self._next_id
        self._next_id += 1
        self._texts[note_id] = text
        return {"id": note_id, "text": text}

    def update(self, note_id: int, text: str) -> Optional[dict]:
        if note_id not in self._texts:
            return None
        self._texts[note_id] = text
        return {"id": note_id, "text": text}

    def delete(self, note_id: int) -> bool:
        return self._texts.pop(note_id, None) is not None


def handle(request: Request, public_dir: Path, store: NotesStore) -> Response:
    if request.path == NOTES_PATH or request.path.startswith(NOTES_PATH + "/"):
        return handle_api(request, store)
    return handle_static(request, public_dir)


def handle_static(request: Request, public_dir: Path) -> Response:
    if request.method != "GET":
        return error_response(405, "method not allowed")

    # Percent escapes are refused, so "%2e%2e" cannot hide a "..".
    path = request.path
    if "%" in path or "\\" in path or ".." in path.split("/"):
        return error_response(403, "forbidden")

    file_path = public_dir / (path.lstrip("/") or "index.html")
    if not file_path.is_file():
        return error_response(404, "not found")
    return Response(200, file_path.read_bytes(), mime_type(file_path.name))


def note_text(body: bytes) -> Optional[str]:
    """The body as a note: plain UTF-8 text, trimmed, 1 to 200 bytes. None if it is not a valid note."""
    try:
        text = body.decode("utf-8").strip()
    except UnicodeDecodeError:
        return None
    if not text or len(text.encode("utf-8")) > MAX_NOTE_BYTES:
        return None
    return text


def handle_api(request: Request, store: NotesStore) -> Response:
    rest = request.path[len(NOTES_PATH):]

    if rest == "":
        if request.method == "GET":
            return json_response(200, store.all())
        if request.method == "POST":
            text = note_text(request.body)
            if text is None:
                return error_response(400, BAD_TEXT)
            if store.is_full():
                return error_response(507, "too many notes")
            return json_response(201, store.add(text))
        return error_response(405, "method not allowed")

    note_id_text = rest[1:]  # the path is "/api/notes/<id>" here
    if not (note_id_text.isascii() and note_id_text.isdigit()):
        return error_response(404, "not found")
    note_id = int(note_id_text)

    if request.method == "PUT":
        text = note_text(request.body)
        if text is None:
            return error_response(400, BAD_TEXT)
        note = store.update(note_id, text)
        return json_response(200, note) if note else error_response(404, "not found")
    if request.method == "DELETE":
        if store.delete(note_id):
            return Response(204)
        return error_response(404, "not found")
    return error_response(405, "method not allowed")
