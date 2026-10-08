# HTTP Server with a Notes API (JavaScript)

A small HTTP/1.1 server written for Node.js with the built-in `http` module. It serves the files in `public/` (with the right `Content-Type`) and a notes API whose notes are kept in memory. Open `http://127.0.0.1:8000/` in a browser to add notes on a small page.

The same server is also written in [C](../../c/http_server) and [Python](../../python/http_server). All three answer the same requests with the same status codes.

![Screenshot](screenshot.png)

## Features

- Serves static files from `public/` with the correct `Content-Type` (HTML, CSS, JSON, images, ...)
- `404 Not Found` for missing files and `403 Forbidden` for paths that try to leave `public/` (for example `/../package.json`)
- A notes API: `GET /api/notes`, `POST /api/notes`, `PUT /api/notes/<id>`, `DELETE /api/notes/<id>`
- The note is the plain-text request body, and the answers are JSON
- Status codes `200`, `201`, `204`, `400`, `403`, `404`, `405` and `507` (store full, at most 100 notes)
- One log line per request, for example `POST /api/notes 201`
- No npm packages at all: only Node's built-in modules

## How to use

Start the server from the project directory. The port and the folder are optional:

```sh
node src/main.js          # port 8000, serves ./public
node src/main.js 9000     # another port
```

Example session (the server log is printed by the server, the rest is what `curl` shows):

```sh
curl -X POST -H 'Content-Type: text/plain' -d 'buy milk' http://127.0.0.1:8000/api/notes
# HTTP 201  {"id":1,"text":"buy milk"}

curl http://127.0.0.1:8000/api/notes
# HTTP 200  [{"id":1,"text":"buy milk"}]

curl -X PUT -H 'Content-Type: text/plain' -d 'buy oat milk' http://127.0.0.1:8000/api/notes/1
# HTTP 200  {"id":1,"text":"buy oat milk"}

curl -X DELETE http://127.0.0.1:8000/api/notes/1
# HTTP 204  (no body)
```

| Request | Success | Errors |
|---|---|---|
| `GET /api/notes` | `200` with a JSON array | |
| `POST /api/notes` with the note as text | `201` with the new note | `400` empty or over 200 bytes, `507` store full |
| `PUT /api/notes/<id>` with the note as text | `200` with the changed note | `400` empty or over 200 bytes, `404` no such note |
| `DELETE /api/notes/<id>` | `204` with no body | `404` no such note |
| any other method on the API | | `405` |
| `GET /<file>` | `200` with the file | `404` missing, `403` outside `public/` |

The body is trimmed of spaces and line breaks first. The note must then be 1 to 200 bytes of UTF-8.

## How it works

### Who reads the request

Node's `http` module reads the request line, the headers and the body for you. `src/main.js` only collects the body chunks and calls the logic with the method, the URL and the body. This is the big difference from the C and Python versions, which parse the bytes themselves.

### The user interface

`createServer()` in `src/main.js` returns an HTTP server. For each request it waits for the `end` event (the whole body has arrived), calls `handleRequest()`, logs one line, and writes the response with `writeHead()` and `end()`. The file runs the server only when it is started directly (`require.main === module`), so the tests can create a server on port 0 without starting one on port 8000.

### Routing

`handleRequest()` in `src/http_server.js` removes the query string from the URL. `/api/notes` and `/api/notes/...` go to `handleNotes()`. Everything else is a file request for `handleStatic()`.

For a file, `handleStatic()` refuses any path with a `%` escape, a backslash or a `..` part, so the file must be inside `public/`. A path of `/` means `index.html`. `mimeType()` looks up the extension in `MIME_TYPES`.

### The notes store

`NotesStore` keeps a `Map` from id to text, and refuses new notes after 100. Ids start at 1 and are never reused. The store is created in `main.js` and passed to the logic, so there is no global state.

### Text in, JSON out

`noteText()` decodes the body as UTF-8 (a `TextDecoder` with `fatal: true` rejects invalid bytes), trims it and checks the length. A response is a plain object `{ status, contentType, body }`, where `body` is a `Buffer`. The JSON answers are made with `JSON.stringify()`, which also escapes quotes and backslashes. For a `204` the content type is `null`, and `main.js` then sends no `Content-Type` or `Content-Length` header.

## Project layout

```
package.json               the test and start scripts (no dependencies)
public/                    the files the server serves
public/index.html          a page that lists and adds notes with fetch()
public/style.css           the page style
public/about.json          a small JSON file (demo content)
src/http_server.js         the logic: MIME types, safe paths, notes store, routing
src/main.js                the program: the HTTP server and the command-line arguments
tests/http_server.test.js  tests of the logic, and one test of the real HTTP server
```

## Requirements

- Node.js 18 or newer (for `node:test` and the global `fetch` used by the tests)

## Run

```sh
npm start
```

or, without npm:

```sh
node src/main.js
```

Then open `http://127.0.0.1:8000/` in a browser.

## Test

```sh
npm test
```

Most tests call the logic functions directly with request objects. One test starts the real server on a free port and sends it requests with `fetch()`.

## Comparison with the other versions

- [C](../../c/http_server)
- [Python](../../python/http_server)

| | C | Python | JavaScript |
|---|---|---|---|
| Interface | POSIX sockets, one connection at a time | `socket` module, one connection at a time | Node `http` module |
| Lines of logic | 424 | 159 | 133 |
| Lines of interface | 119 | 62 | 35 |
| Tests | 13 | 22 | 11 |

Node's `http` module does the HTTP parsing, so this version has no request parser and its interface is the shortest. The logic works on already parsed requests and uses `TextDecoder`, `JSON.stringify` and `Map`, so it is short too. The C version parses the bytes itself and manages its own arrays and strings, which is why its logic is the longest. Python sits in the middle: the parsing is visible, but JSON and string handling come from the standard library.

## Ideas for extensions

- Store the notes in a file, so they survive a restart
- Add a `GET /api/notes/<id>` route
- Limit the request body size, so a client cannot send gigabytes
- Decode `%XX` escapes in paths instead of refusing them
- Send `ETag` or `Last-Modified` headers for the static files
