# Quicksort, one frame per swap. White bars are being swapped, magenta is the pivot.
from term import H, W, pixels, rnd, seed, show_pixels, start

KEYS = [0xFF3030, 0xFFD030, 0x30E060, 0x30C0FF, 0xA040FF]


def rainbow(v):
    p = (v - 1) * 1024 // H
    f = p % 256
    c1, c2 = KEYS[p // 256], KEYS[p // 256 + 1]
    return sum(((c1 >> s & 255) * (256 - f) + (c2 >> s & 255) * f) // 256 << s for s in (0, 8, 16))


class Sorter:
    def __init__(self, a):
        self.a = a
        self.compares = self.swaps = 0

    def draw(self, i1=-1, i2=-1, pivot=-1, done=0):
        for x, v in enumerate(self.a):
            c = (0x40FF70 if x < done else 0xFF40FF if x == pivot
                 else 0xFFFFFF if x in (i1, i2) else rainbow(v))
            for y in range(H):
                pixels[y][x] = c if y >= H - v else 0
        show_pixels(f" quicksort: {self.compares} comparisons, {self.swaps} swaps")

    def swap(self, i, j, pivot):
        self.a[i], self.a[j] = self.a[j], self.a[i]
        self.swaps += 1
        self.draw(i, j, pivot)

    def quicksort(self, lo, hi):
        if lo >= hi:
            return
        pivot, i = self.a[hi], lo
        for j in range(lo, hi):
            self.compares += 1
            if self.a[j] < pivot:
                self.swap(i, j, hi)
                i += 1
        self.swap(i, hi, i)
        self.quicksort(lo, i - 1)
        self.quicksort(i + 1, hi)


def main():
    seed(7)
    a = [1 + i * H // W for i in range(W)]
    start(30)
    while True:
        for i in range(W - 1, 0, -1):
            j = rnd(i + 1)
            a[i], a[j] = a[j], a[i]
        sorter = Sorter(a)
        for _ in range(20):
            sorter.draw()
        sorter.quicksort(0, W - 1)
        for done in range(1, W + 41):
            sorter.draw(done=done)


if __name__ == "__main__":
    main()
