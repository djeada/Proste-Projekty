// A tiny ray tracer: a shiny sphere over a checkered floor, lit by a moving light.
import { H, W, pixels, showPixels, start } from './term.js';

const vec = (x, y, z) => ({ x, y, z });
const add = (a, b) => vec(a.x + b.x, a.y + b.y, a.z + b.z);
const sub = (a, b) => vec(a.x - b.x, a.y - b.y, a.z - b.z);
const mul = (a, s) => vec(a.x * s, a.y * s, a.z * s);
const dot = (a, b) => a.x * b.x + a.y * b.y + a.z * b.z;
const norm = (a) => mul(a, 1 / Math.sqrt(dot(a, a)));

const CENTER = vec(0, 0, 4);
let light;

function hitSphere(o, d) {
  const oc = sub(o, CENTER);
  const b = dot(oc, d);
  const disc = b * b - dot(oc, oc) + 1;
  if (disc < 0) return -1;
  const t = -b - Math.sqrt(disc);
  return t > 1e-6 ? t : -1;
}

function trace(o, d, depth) {
  const ts = hitSphere(o, d);
  const tp = d.y < 0 ? (-1 - o.y) / d.y : -1;
  if (ts < 0 && tp < 0) return vec(20, 30 + 60 * d.y, 70 + 140 * d.y);
  if (ts > 0 && (tp < 0 || ts < tp)) {
    const p = add(o, mul(d, ts));
    const n = norm(sub(p, CENTER));
    const l = norm(sub(light, p));
    const r = sub(d, mul(n, 2 * dot(d, n)));
    const diffuse = Math.max(0, dot(n, l));
    let spec = Math.max(0, dot(r, l));
    for (let k = 0; k < 5; k++) spec *= spec;
    let c = mul(vec(255, 60, 100), 0.15 + 0.85 * diffuse);
    if (depth < 2) c = add(mul(c, 0.7), mul(trace(p, r, depth + 1), 0.3));
    return add(c, vec(255 * spec, 255 * spec, 255 * spec));
  }
  const p = add(o, mul(d, tp));
  const l = norm(sub(light, p));
  let shade = 0.25 + 0.75 * Math.max(0, l.y);
  if (hitSphere(p, l) > 0) shade *= 0.3;
  const fog = 1 / (1 + 0.004 * tp * tp);
  const c = (Math.floor(p.x) + Math.floor(p.z)) & 1 ? vec(230, 230, 230) : vec(40, 40, 60);
  return add(mul(c, shade * fog), mul(vec(20, 30, 70), 1 - fog));
}

start(30);
for (let frame = 0; ; frame++) {
  const a = frame * 0.06;
  light = vec(5 * Math.cos(a), 3 + Math.sin(a * 0.7), 1.5 + 2.5 * Math.sin(a));
  for (let y = 0; y < H; y++) {
    for (let x = 0; x < W; x++) {
      const d = norm(vec((x - W / 2) / (W / 2), (H / 2 - y) / (W / 2) - 0.15, 1.4));
      const c = trace(vec(0, 0.3, 0), d, 0);
      const [r, g, b] = [c.x, c.y, c.z].map((v) => Math.trunc(Math.min(255, v)));
      pixels[y][x] = (r << 16) | (g << 8) | b;
    }
  }
  await showPixels();
}
