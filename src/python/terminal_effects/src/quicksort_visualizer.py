# Quicksort, visualized: every swap is a frame.
import random
import sys
import time

N, H = 48, 24
RAINBOW = [196, 202, 208, 214, 220, 226, 190, 154, 118, 82, 46, 47,
           48, 49, 50, 51, 45, 39, 33, 27, 21, 57, 93, 129]


class Visualizer:
    def __init__(self, a):
        self.a, self.compares, self.swaps = a, 0, 0

    def draw(self, x1=-1, x2=-1, pivot=-1, done=0):
        out = [f"\033[H\033[0m  quicksort | compares {self.compares:4d}"
               f" | swaps {self.swaps:3d}\n\n"]
        for row in range(H, 0, -1):
            last = -1
            for i, v in enumerate(self.a):
                if v < row:
                    out.append(" ")
                    continue
                c = (46 if i < done else 201 if i == pivot
                     else 231 if i in (x1, x2) else RAINBOW[v - 1])
                if c != last:
                    out.append(f"\033[38;5;{c}m")
                    last = c
                out.append("█")
            out.append("\033[0m\n")
        sys.stdout.write("".join(out))
        sys.stdout.flush()

    def swap(self, i, j, pivot):
        self.a[i], self.a[j] = self.a[j], self.a[i]
        self.swaps += 1
        self.draw(i, j, pivot)
        time.sleep(0.045)

    def quicksort(self, lo, hi):
        if lo >= hi:
            return
        p, i = self.a[hi], lo
        for j in range(lo, hi):
            self.compares += 1
            if self.a[j] < p:
                self.swap(i, j, hi)
                i += 1
        self.swap(i, hi, i)
        self.quicksort(lo, i - 1)
        self.quicksort(i + 1, hi)


def main():
    random.seed(7)
    a = [1 + i * H // N for i in range(N)]
    random.shuffle(a)  # Fisher-Yates shuffle
    vis = Visualizer(a)
    sys.stdout.write("\033[2J")
    vis.draw()
    time.sleep(0.6)
    vis.quicksort(0, N - 1)
    for done in range(1, N + 1):  # victory sweep
        vis.draw(done=done)
        time.sleep(0.015)


if __name__ == "__main__":
    main()
