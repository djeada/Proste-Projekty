# Demoscene fire: every pixel is a little less than the average of the pixels below it.
# The two hidden rows under the screen are random embers that feed the flames.
from term import H, W, pixels, rnd, seed, show_pixels, start

PALETTE = [
    0x000000, 0x1F0707, 0x2F0F07, 0x470F07, 0x571707, 0x671F07, 0x771F07, 0x8F2707,
    0x9F2F07, 0xAF3F07, 0xBF4707, 0xC74707, 0xDF4F07, 0xDF5707, 0xDF5707, 0xD75F07,
    0xD75F07, 0xD7670F, 0xCF6F0F, 0xCF770F, 0xCF7F0F, 0xCF8717, 0xC78717, 0xC78F17,
    0xC7971F, 0xBF9F1F, 0xBF9F1F, 0xBFA727, 0xBFA727, 0xBFAF2F, 0xB7AF2F, 0xB7B72F,
    0xB7B737, 0xCFCF6F, 0xDFDF9F, 0xEFEFC7, 0xFFFFFF,
]
MAX_HEAT = len(PALETTE) - 1


def main():
    seed(1993)
    heat = [[0] * (W + 2) for _ in range(H + 2)]
    start(30)
    while True:
        for x in range(1, W + 1):
            heat[H][x] = MAX_HEAT if rnd(3) else 0
            heat[H + 1][x] = MAX_HEAT if rnd(3) else 0
        for y in range(H):
            below, below2 = heat[y + 1], heat[y + 2]
            for x in range(1, W + 1):
                heat[y][x] = (below[x - 1] + below[x] + below[x + 1] + below2[x]) * 31 // 129
        for y in range(H):
            for x in range(W):
                pixels[y][x] = PALETTE[heat[y][x + 1]]
        show_pixels()


if __name__ == "__main__":
    main()
