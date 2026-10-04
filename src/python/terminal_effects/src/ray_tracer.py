# A tiny ray tracer: a reflective sphere, a checkered floor, soft
# lighting and hard shadows, with the light orbiting the scene.
import math
import sys
import time

W, H = 48, 26
FRAMES = 150


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
        return self * (1 / math.sqrt(self.dot(self)))


CENTER = Vec(0, 0, 4)


def hit_sphere(o, d):
    oc = o - CENTER
    b, c = oc.dot(d), oc.dot(oc) - 1
    disc = b * b - c
    if disc < 0:
        return -1
    t = -b - math.sqrt(disc)
    return t if t > 1e-3 else -1


def trace(o, d, light, depth=0):
    ts = hit_sphere(o, d)
    tp = (-1 - o.y) / d.y if d.y < 0 else -1
    if ts < 0 and tp < 0:
        return Vec(20, 30 + 40 * d.y, 60 + 120 * d.y)  # sky
    if ts > 0 and (tp < 0 or ts < tp):
        p = o + d * ts
        n = (p - CENTER).norm()
        to_light = (light - p).norm()
        diff = max(0, n.dot(to_light))
        r = d - n * (2 * d.dot(n))
        spec = max(0, r.dot(to_light)) ** 30
        base = Vec(255, 60, 100) * (0.15 + 0.85 * diff)
        if depth < 2:
            base = base * 0.7 + trace(p, r, light, depth + 1) * 0.3
        return base + Vec(255 * spec, 255 * spec, 255 * spec)
    p = o + d * tp
    check = (math.floor(p.x) + math.floor(p.z)) & 1
    to_light = (light - p).norm()
    shade = 0.25 + 0.75 * max(0, to_light.y)
    if hit_sphere(p, to_light) > 0:
        shade *= 0.3  # shadow
    fog = math.exp(-0.06 * tp)
    base = Vec(230, 230, 230) if check else Vec(40, 40, 60)
    return base * (shade * fog) + Vec(20, 30, 60) * (1 - fog)


def main(frames=FRAMES):
    eye = Vec(0, 0.3, 0)
    sys.stdout.write("\033[2J")
    for frame in range(frames):
        a = frame * 0.06
        light = Vec(5 * math.cos(a), 3 + math.sin(a * 0.7), 1.5 + 2.5 * math.sin(a))
        out = ["\033[H"]
        for y in range(H):
            for x in range(W):
                u = (x - W / 2) / (W / 2)
                w = (H / 2 - y) * 2.3 / (W / 2)
                c = trace(eye, Vec(u, w - 0.15, 1.4).norm(), light)
                r, g, b = (int(min(255, v)) for v in (c.x, c.y, c.z))
                out.append(f"\033[38;2;{r};{g};{b}m█")
            out.append("\n")
        out.append("\033[0m")
        sys.stdout.write("".join(out))
        sys.stdout.flush()
        time.sleep(0.03)
    print(f"\033[1;36m  [+] {frames} frames ray traced on the CPU\033[0m")


if __name__ == "__main__":
    main()
