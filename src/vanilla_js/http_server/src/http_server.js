// HTTP logic: MIME types, safe paths, the notes store and routing. No sockets here.
const fs = require('fs');
const path = require('path');

const MAX_NOTE_BYTES = 200;
const MAX_NOTES = 100;
const NOTES_PATH = '/api/notes';
const JSON_TYPE = 'application/json; charset=utf-8';
const BAD_NOTE = 'body must be 1 to 200 bytes of plain text';

const MIME_TYPES = {
  '.html': 'text/html; charset=utf-8',
  '.css': 'text/css; charset=utf-8',
  '.js': 'text/javascript; charset=utf-8',
  '.json': 'application/json; charset=utf-8',
  '.txt': 'text/plain; charset=utf-8',
  '.png': 'image/png',
  '.jpg': 'image/jpeg',
  '.gif': 'image/gif',
  '.svg': 'image/svg+xml',
};

// A response is { status, contentType, body } where body is a Buffer.
function jsonResponse(status, data) {
  return { status, contentType: JSON_TYPE, body: Buffer.from(JSON.stringify(data)) };
}

function errorResponse(status, message) {
  return jsonResponse(status, { error: message });
}

function mimeType(fileName) {
  return MIME_TYPES[path.extname(fileName).toLowerCase()] || 'application/octet-stream';
}

// Notes kept in memory, numbered 1, 2, 3, ... and never reused.
class NotesStore {
  constructor() {
    this.texts = new Map();
    this.nextId = 1;
  }

  all() {
    return [...this.texts].map(([id, text]) => ({ id, text }));
  }

  isFull() {
    return this.texts.size >= MAX_NOTES;
  }

  add(text) {
    const id = this.nextId++;
    this.texts.set(id, text);
    return { id, text };
  }

  update(id, text) {
    if (!this.texts.has(id)) {
      return null;
    }
    this.texts.set(id, text);
    return { id, text };
  }

  delete(id) {
    return this.texts.delete(id);
  }
}

// The body as a note: plain UTF-8 text, trimmed, 1 to 200 bytes. null if it is not a valid note.
function noteText(body) {
  let text;
  try {
    text = new TextDecoder('utf-8', { fatal: true }).decode(body).trim();
  } catch {
    return null;
  }
  if (text === '' || Buffer.byteLength(text, 'utf8') > MAX_NOTE_BYTES) {
    return null;
  }
  return text;
}

function handleStatic(method, urlPath, publicDir) {
  if (method !== 'GET') {
    return errorResponse(405, 'method not allowed');
  }
  // Percent escapes are refused, so "%2e%2e" cannot hide a "..".
  if (urlPath.includes('%') || urlPath.includes('\\') || urlPath.split('/').includes('..')) {
    return errorResponse(403, 'forbidden');
  }

  const file = path.join(publicDir, urlPath.replace(/^\/+/, '') || 'index.html');
  let isFile = false;
  try {
    isFile = fs.statSync(file).isFile();
  } catch {
    isFile = false;
  }
  if (!isFile) {
    return errorResponse(404, 'not found');
  }
  return { status: 200, contentType: mimeType(file), body: fs.readFileSync(file) };
}

function handleNotes(method, urlPath, body, store) {
  const rest = urlPath.slice(NOTES_PATH.length);

  if (rest === '') {
    if (method === 'GET') {
      return jsonResponse(200, store.all());
    }
    if (method === 'POST') {
      const text = noteText(body);
      if (text === null) {
        return errorResponse(400, BAD_NOTE);
      }
      return store.isFull() ? errorResponse(507, 'too many notes') : jsonResponse(201, store.add(text));
    }
    return errorResponse(405, 'method not allowed');
  }

  // The path is "/api/notes/<id>" here.
  const idText = rest.slice(1);
  if (!/^\d+$/.test(idText)) {
    return errorResponse(404, 'not found');
  }
  const id = Number(idText);

  if (method === 'PUT') {
    const text = noteText(body);
    if (text === null) {
      return errorResponse(400, BAD_NOTE);
    }
    const note = store.update(id, text);
    return note ? jsonResponse(200, note) : errorResponse(404, 'not found');
  }
  if (method === 'DELETE') {
    return store.delete(id) ? { status: 204, contentType: null, body: Buffer.alloc(0) } : errorResponse(404, 'not found');
  }
  return errorResponse(405, 'method not allowed');
}

// request is { method, url, body } where body is a Buffer. Node's http module has already parsed the request.
function handleRequest(request, publicDir, store) {
  const urlPath = request.url.split('?')[0];
  if (urlPath === NOTES_PATH || urlPath.startsWith(NOTES_PATH + '/')) {
    return handleNotes(request.method, urlPath, request.body, store);
  }
  return handleStatic(request.method, urlPath, publicDir);
}

module.exports = { MAX_NOTE_BYTES, MAX_NOTES, mimeType, noteText, NotesStore, handleRequest };
