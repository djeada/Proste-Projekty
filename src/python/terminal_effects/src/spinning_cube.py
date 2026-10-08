# A cube spinning around three axes. A z-buffer keeps the nearest face on top,
# and faces turned towards the viewer are lit brighter.
from math import cos, sin

from term import H, W, pixels, show_pixels, start

FACE_COLOR = [0xFF4060, 0x40E070, 0x4080FF, 0xFFD040, 0xE050FF, 0x40E0FF]
NORMAL = [(0, 0, -1), (1, 0, 0), (-1, 0, 0), (0, 0, 1), (0, -1, 0), (0, 1, 0)]


class Cube:
    def __init__(self, A, B, C):
        self.sA, self.cA, self.sB, self.cB, self.sC, self.cC = sin(A), cos(A), sin(B), cos(B), sin(C), cos(C)
        self.zbuf = [[0.0] * W for _ in range(H)]

    def rotate(self, i, j, k):
        sA, cA, sB, cB, sC, cC = self.sA, self.cA, self.sB, self.cB, self.sC, self.cC
        return (j * sA * sB * cC - k * cA * sB * cC + j * cA * sC + k * sA * sC + i * cB * cC,
                j * cA * cC + k * sA * cC - j * sA * sB * sC + k * cA * sB * sC - i * cB * sC,
                k * cA * cB - j * sA * cB + i * sB)

    def plot(self, i, j, k, color):
        x, y, z = self.rotate(i, j, k)
        ooz = 1 / (z + 60)
        px, py = W // 2 + int(60 * ooz * x), H // 2 + int(60 * ooz * y)
        if 0 <= px < W and 0 <= py < H and ooz > self.zbuf[py][px]:
            self.zbuf[py][px] = ooz
            pixels[py][px] = color

    def lit(self, face):
        light = 0.3 + 0.7 * max(0, -self.rotate(*NORMAL[face])[2])
        c = FACE_COLOR[face]
        return sum(int((c >> s & 255) * light) << s for s in (0, 8, 16))


def main():
    A = B = C = 0.0
    start(30)
    while True:
        cube = Cube(A, B, C)
        for row in pixels:
            row[:] = [0] * W
        color = [cube.lit(f) for f in range(6)]
        for u in range(40):
            for v in range(40):
                a, b = -10 + u * 0.5, -10 + v * 0.5
                cube.plot(a, b, -10, color[0])
                cube.plot(10, b, a, color[1])
                cube.plot(-10, b, -a, color[2])
                cube.plot(-a, b, 10, color[3])
                cube.plot(a, -10, -b, color[4])
                cube.plot(a, 10, b, color[5])
        show_pixels()
        A += 0.05
        B += 0.05
        C += 0.01


if __name__ == "__main__":
    main()
