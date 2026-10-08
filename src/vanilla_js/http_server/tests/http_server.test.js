const { test } = require('node:test');
const assert = require('node:assert/strict');
const path = require('node:path');
const { createServer } = require('../src/main.js');
const { MAX_NOTE_BYTES, MAX_NOTES, mimeType, noteText, NotesStore, handleRequest } = require('../src/http_server.js');

const PUBLIC = path.join(__dirname, '..', 'public');

// Handles a request whose body is plain text.
function send(method, url, text = '', store = new NotesStore()) {
  return handleRequest({ method, url, body: Buffer.from(text) }, PUBLIC, store);
}

const json = (response) => JSON.parse(response.body.toString('utf8'));

test('MIME types are chosen by extension', () => {
  assert.equal(mimeType('index.html'), 'text/html; charset=utf-8');
  assert.equal(mimeType('STYLE.CSS'), 'text/css; charset=utf-8');
  assert.equal(mimeType('logo.png'), 'image/png');
  assert.equal(mimeType('archive.bin'), 'application/octet-stream');
});

test('noteText trims plain text and rejects empty, too long or non-UTF-8 bodies', () => {
  assert.equal(noteText(Buffer.from('  buy milk \n')), 'buy milk');
  assert.equal(noteText(Buffer.from('   ')), null);
  assert.equal(noteText(Buffer.from('x'.repeat(MAX_NOTE_BYTES))), 'x'.repeat(MAX_NOTE_BYTES));
  assert.equal(noteText(Buffer.from('x'.repeat(MAX_NOTE_BYTES + 1))), null);
  assert.equal(noteText(Buffer.from([0xff, 0xfe])), null);
});

test('the root path serves index.html', () => {
  const response = send('GET', '/');
  assert.equal(response.status, 200);
  assert.equal(response.contentType, 'text/html; charset=utf-8');
  assert.match(response.body.toString('utf8'), /<title>Notes<\/title>/);
});

test('a missing file is 404 and static files only accept GET', () => {
  assert.equal(send('GET', '/missing.txt').status, 404);
  assert.equal(send('GET', '/public').status, 404);
  assert.equal(send('POST', '/index.html').status, 405);
});

test('paths that escape the public directory are 403', () => {
  for (const url of ['/../package.json', '/%2e%2e/package.json', '/css/../../package.json', '/a%00b']) {
    assert.equal(send('GET', url).status, 403, url);
  }
});

test('notes can be created, listed, changed and deleted', () => {
  const store = new NotesStore();
  assert.deepEqual(json(send('GET', '/api/notes', '', store)), []);

  const created = send('POST', '/api/notes', ' first ', store);
  assert.equal(created.status, 201);
  assert.deepEqual(json(created), { id: 1, text: 'first' });

  assert.equal(send('PUT', '/api/notes/1', 'changed', store).status, 200);
  assert.deepEqual(json(send('GET', '/api/notes', '', store)), [{ id: 1, text: 'changed' }]);

  assert.equal(send('DELETE', '/api/notes/1', '', store).status, 204);
  assert.deepEqual(json(send('GET', '/api/notes', '', store)), []);
});

test('the answer escapes quotes, backslashes and control characters', () => {
  const created = send('POST', '/api/notes', 'say "hi"\\ now');
  assert.deepEqual(json(created), { id: 1, text: 'say "hi"\\ now' });
});

test('ids are not reused after a delete', () => {
  const store = new NotesStore();
  send('POST', '/api/notes', 'a', store);
  send('DELETE', '/api/notes/1', '', store);
  assert.equal(json(send('POST', '/api/notes', 'b', store)).id, 2);
});

test('a full store answers 507', () => {
  const store = new NotesStore();
  for (let i = 0; i < MAX_NOTES; i++) {
    send('POST', '/api/notes', 'x', store);
  }
  assert.equal(send('POST', '/api/notes', 'one more', store).status, 507);
});

test('API errors use 400, 404 and 405', () => {
  const store = new NotesStore();
  assert.equal(send('POST', '/api/notes', '   ', store).status, 400);
  assert.equal(send('PUT', '/api/notes/9', 'x', store).status, 404);
  assert.equal(send('DELETE', '/api/notes/9', '', store).status, 404);
  assert.equal(send('DELETE', '/api/notes/abc', '', store).status, 404);
  assert.equal(send('GET', '/api/notes/1/2', '', store).status, 404);
  assert.equal(send('PATCH', '/api/notes', '', store).status, 405);
  assert.equal(send('POST', '/api/notes/1', 'x', store).status, 405);
});

test('the HTTP server answers real requests', async () => {
  const server = createServer(PUBLIC, new NotesStore());
  await new Promise((resolve) => server.listen(0, '127.0.0.1', resolve));
  const base = `http://127.0.0.1:${server.address().port}`;
  try {
    const created = await fetch(`${base}/api/notes`, {
      method: 'POST',
      headers: { 'Content-Type': 'text/plain' },
      body: 'over HTTP',
    });
    assert.equal(created.status, 201);
    assert.deepEqual(await created.json(), { id: 1, text: 'over HTTP' });

    const page = await fetch(`${base}/`);
    assert.equal(page.status, 200);
    assert.equal(page.headers.get('content-type'), 'text/html; charset=utf-8');
    assert.equal(page.headers.get('content-length'), String((await page.text()).length));
  } finally {
    server.closeAllConnections();
    await new Promise((resolve) => server.close(resolve));
  }
});
