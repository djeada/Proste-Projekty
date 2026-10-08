# Fireworks: rockets fly up, burst into sparks, and gravity pulls everything down.
# Old frames are dimmed instead of cleared, which leaves glowing trails.
from term import H, W, pixels, rnd, seed, show_pixels, start

MAX = 400
COLORS = [0xFF5050, 0xFFD040, 0x50FF80, 0x50C8FF, 0xD070FF, 0xFFFFFF]


class Particle:
    def __init__(self):
        self.life = 0


particles = [Particle() for _ in range(MAX)]


def spawn(x, y, dx, dy, frames, color):
    for p in particles:
        if not p.life:
            p.x, p.y, p.vx, p.vy, p.life, p.color, p.rocket = x, y, dx, dy, frames, color, False
            return p
    return None


def burst(x, y):
    color = COLORS[rnd(6)]
    for _ in range(60):
        while True:
            dx = (rnd(201) - 100) / 100
            dy = (rnd(201) - 100) / 100
            if dx * dx + dy * dy <= 1:
                break
        spawn(x, y, dx, dy, 25 + rnd(20), color)


def dim(c, num, den):
    return (c >> 16) * num // den << 16 | (c >> 8 & 255) * num // den << 8 | (c & 255) * num // den


def main():
    seed(2025)
    start(30)
    while True:
        for row in pixels:
            row[:] = [dim(c, 3, 4) for c in row]
        if rnd(12) == 0:
            x, dx, dy = 8 + rnd(W - 16), (rnd(41) - 20) / 100, -(1.3 + rnd(40) / 100)
            p = spawn(x, H - 1, dx, dy, 100, 0xFFE0B0)
            if p:
                p.rocket = True
        for p in particles:
            if not p.life:
                continue
            p.x += p.vx
            p.y += p.vy
            p.vy += 0.05 if p.rocket else 0.02
            p.vx *= 0.97
            p.life -= 1
            if p.rocket and p.vy >= 0:
                p.life = 0
                burst(p.x, p.y)
                continue
            x, y = int(p.x), int(p.y)
            if 0 <= x < W and 0 <= y < H:
                pixels[y][x] = p.color if p.rocket else dim(p.color, p.life + 15, 60)
        show_pixels()


if __name__ == "__main__":
    main()
