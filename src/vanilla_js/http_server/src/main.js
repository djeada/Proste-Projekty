// The program: a server on http://127.0.0.1:8000/. Usage: node src/main.js [port] [public_dir]
const http = require('http');
const { NotesStore, handleRequest } = require('./http_server.js');

const DEFAULT_PORT = 8000;

function createServer(publicDir, store) {
  return http.createServer((req, res) => {
    const chunks = [];
    req.on('data', (chunk) => chunks.push(chunk));
    req.on('end', () => {
      const response = handleRequest({ method: req.method, url: req.url, body: Buffer.concat(chunks) }, publicDir, store);
      const headers = { Connection: 'close' };
      if (response.contentType) {
        headers['Content-Type'] = response.contentType;
        headers['Content-Length'] = response.body.length;
      }
      console.log(`${req.method} ${req.url.split('?')[0]} ${response.status}`);
      res.writeHead(response.status, headers);
      res.end(response.body);
    });
  });
}

module.exports = { createServer };

if (require.main === module) {
  const portArg = process.argv[2] ?? String(DEFAULT_PORT);
  const port = Number(portArg);
  if (!/^\d+$/.test(portArg) || port > 65535) {
    console.error(`Invalid port: ${portArg}`);
    process.exit(1);
  }
  const publicDir = process.argv[3] || 'public';
  const server = createServer(publicDir, new NotesStore());
  server.listen(port, '127.0.0.1', () => {
    console.log(`Serving ${publicDir} on http://127.0.0.1:${server.address().port}/`);
  });
}
