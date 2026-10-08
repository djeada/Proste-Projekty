# A tiny ray tracer: a shiny sphere over a checkered floor, lit by a moving light.
from math import cos, floor, sin, sqrt

from term import H, W, pixels, show_pixels, start


class Vec:
    __slots__ = ("x", "y", "z")

    def __init__(self, x, y, z):
        self.x, self.y, self.z = x, y, z

    def __add__(self, o):
        return Vec(self.x + o.x, self.y + o.y, self.z + o.z)

    def __sub__(self, o):
        return Vec(self.x - o.x, self.y - o.y, self.z - o.z)

    def __mul__(self, s):
        return Vec(self.x * s, self.y * s, self.z * s)

    def dot(self, o):
        return self.x * o.x + self.y * o.y + self.z * o.z

    def norm(self):
        return self * (1 / sqrt(self.dot(self)))


CENTER = Vec(0, 0, 4)


def hit_sphere(o, d):
    oc = o - CENTER
    b = oc.dot(d)
    disc = b * b - oc.dot(oc) + 1
    if disc < 0:
        return -1
    t = -b - sqrt(disc)
    return t if t > 1e-6 else -1


def trace(o, d, light, depth=0):
    ts = hit_sphere(o, d)
    tp = (-1 - o.y) / d.y if d.y < 0 else -1
    if ts < 0 and tp < 0:
        return Vec(20, 30 + 60 * d.y, 70 + 140 * d.y)
    if ts > 0 and (tp < 0 or ts < tp):
        p = o + d * ts
        n = (p - CENTER).norm()
        to_light = (light - p).norm()
        r = d - n * (2 * d.dot(n))
        diffuse = max(0, n.dot(to_light))
        spec = max(0, r.dot(to_light))
        for _ in range(5):
            spec *= spec
        c = Vec(255, 60, 100) * (0.15 + 0.85 * diffuse)
        if depth < 2:
            c = c * 0.7 + trace(p, r, light, depth + 1) * 0.3
        return c + Vec(255 * spec, 255 * spec, 255 * spec)
    p = o + d * tp
    to_light = (light - p).norm()
    shade = 0.25 + 0.75 * max(0, to_light.y)
    if hit_sphere(p, to_light) > 0:
        shade *= 0.3
    fog = 1 / (1 + 0.004 * tp * tp)
    c = Vec(230, 230, 230) if (floor(p.x) + floor(p.z)) & 1 else Vec(40, 40, 60)
    return c * (shade * fog) + Vec(20, 30, 70) * (1 - fog)


def main():
    eye = Vec(0, 0.3, 0)
    start(30)
    frame = 0
    while True:
        a = frame * 0.06
        light = Vec(5 * cos(a), 3 + sin(a * 0.7), 1.5 + 2.5 * sin(a))
        for y in range(H):
            for x in range(W):
                d = Vec((x - W / 2) / (W / 2), (H / 2 - y) / (W / 2) - 0.15, 1.4).norm()
                c = trace(eye, d, light)
                r, g, b = (int(min(255, v)) for v in (c.x, c.y, c.z))
                pixels[y][x] = r << 16 | g << 8 | b
        show_pixels()
        frame += 1


if __name__ == "__main__":
    main()
