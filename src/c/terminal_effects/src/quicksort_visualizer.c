// Quicksort, visualized: every swap is a frame.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 48
#define H 24

static const int rainbow[H] = {196, 202, 208, 214, 220, 226, 190, 154, 118, 82, 46, 47,
                               48, 49, 50, 51, 45, 39, 33, 27, 21, 57, 93, 129};
static int a[N], compares, swaps;

static void draw(int x1, int x2, int pivot, int done) {
    printf("\033[H\033[0m  quicksort | compares %4d | swaps %3d\n\n", compares, swaps);
    for (int row = H; row >= 1; row--) {
        int last = -1;
        for (int i = 0; i < N; i++) {
            if (a[i] < row) {
                putchar(' ');
                continue;
            }
            int c = i < done ? 46 : i == pivot ? 201 : (i == x1 || i == x2) ? 231
                                                                              : rainbow[a[i] - 1];
            if (c != last) {
                printf("\033[38;5;%dm", c);
                last = c;
            }
            fputs("█", stdout);
        }
        printf("\033[0m\n");
    }
    fflush(stdout);
}

static void swap(int i, int j, int pivot) {
    int t = a[i];
    a[i] = a[j];
    a[j] = t;
    swaps++;
    draw(i, j, pivot, 0);
    usleep(45000);
}

static void quicksort(int lo, int hi) {
    if (lo >= hi) return;
    int p = a[hi], i = lo;
    for (int j = lo; j < hi; j++) {
        compares++;
        if (a[j] < p) swap(i++, j, hi);
    }
    swap(i, hi, i);
    quicksort(lo, i - 1);
    quicksort(i + 1, hi);
}

int main(void) {
    srand(7);
    for (int i = 0; i < N; i++) a[i] = 1 + i * H / N;
    for (int i = N - 1; i > 0; i--) {  // Fisher-Yates shuffle
        int j = rand() % (i + 1), t = a[i];
        a[i] = a[j];
        a[j] = t;
    }
    printf("\033[2J");
    draw(-1, -1, -1, 0);
    usleep(600000);
    quicksort(0, N - 1);
    for (int done = 1; done <= N; done++) {  // victory sweep
        draw(-1, -1, -1, done);
        usleep(15000);
    }
    return 0;
}
