// A tiny ray tracer: a shiny sphere over a checkered floor, lit by a moving light.
#include <math.h>

#include "term.h"

typedef struct {
    double x, y, z;
} vec;

static vec v(double x, double y, double z) { return (vec){x, y, z}; }
static vec add(vec a, vec b) { return v(a.x + b.x, a.y + b.y, a.z + b.z); }
static vec sub(vec a, vec b) { return v(a.x - b.x, a.y - b.y, a.z - b.z); }
static vec mul(vec a, double s) { return v(a.x * s, a.y * s, a.z * s); }
static double dot(vec a, vec b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
static vec norm(vec a) { return mul(a, 1 / sqrt(dot(a, a))); }

static const vec center = {0, 0, 4};
static vec light;

static double hit_sphere(vec o, vec d) {
    vec oc = sub(o, center);
    double b = dot(oc, d), disc = b * b - dot(oc, oc) + 1;
    if (disc < 0) return -1;
    double t = -b - sqrt(disc);
    return t > 1e-6 ? t : -1;
}

static vec trace(vec o, vec d, int depth) {
    double ts = hit_sphere(o, d), tp = d.y < 0 ? (-1 - o.y) / d.y : -1;
    if (ts < 0 && tp < 0) return v(20, 30 + 60 * d.y, 70 + 140 * d.y);
    if (ts > 0 && (tp < 0 || ts < tp)) {
        vec p = add(o, mul(d, ts)), n = norm(sub(p, center)), l = norm(sub(light, p));
        vec r = sub(d, mul(n, 2 * dot(d, n)));
        double diffuse = fmax(0, dot(n, l)), spec = fmax(0, dot(r, l));
        for (int k = 0; k < 5; k++) spec *= spec;
        vec c = mul(v(255, 60, 100), 0.15 + 0.85 * diffuse);
        if (depth < 2) c = add(mul(c, 0.7), mul(trace(p, r, depth + 1), 0.3));
        return add(c, v(255 * spec, 255 * spec, 255 * spec));
    }
    vec p = add(o, mul(d, tp)), l = norm(sub(light, p));
    double shade = 0.25 + 0.75 * fmax(0, l.y);
    if (hit_sphere(p, l) > 0) shade *= 0.3;
    double fog = 1 / (1 + 0.004 * tp * tp);
    vec c = ((int)floor(p.x) + (int)floor(p.z)) & 1 ? v(230, 230, 230) : v(40, 40, 60);
    return add(mul(c, shade * fog), mul(v(20, 30, 70), 1 - fog));
}

int main(int argc, char **argv) {
    start(argc, argv, 30);
    for (int frame = 0;; frame++) {
        double a = frame * 0.06;
        light = v(5 * cos(a), 3 + sin(a * 0.7), 1.5 + 2.5 * sin(a));
        for (int y = 0; y < H; y++)
            for (int x = 0; x < W; x++) {
                vec d = norm(v((x - W / 2.0) / (W / 2.0), (H / 2.0 - y) / (W / 2.0) - 0.15, 1.4));
                vec c = trace(v(0, 0.3, 0), d, 0);
                int r = (int)fmin(255, c.x), g = (int)fmin(255, c.y), b = (int)fmin(255, c.z);
                pixels[y][x] = (uint32_t)(r << 16 | g << 8 | b);
            }
        show_pixels("");
    }
}
