// A tiny ray tracer: a reflective sphere, a checkered floor, soft
// lighting and hard shadows, with the light orbiting the scene.
#include <math.h>
#include <stdio.h>
#include <unistd.h>

#define W 48
#define H 26
#define FRAMES 150

typedef struct { float x, y, z; } vec;

static vec v(float x, float y, float z) { return (vec){x, y, z}; }
static vec add(vec a, vec b) { return v(a.x + b.x, a.y + b.y, a.z + b.z); }
static vec sub(vec a, vec b) { return v(a.x - b.x, a.y - b.y, a.z - b.z); }
static vec mul(vec a, float s) { return v(a.x * s, a.y * s, a.z * s); }
static float dot(vec a, vec b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
static vec norm(vec a) { return mul(a, 1 / sqrtf(dot(a, a))); }

static const vec center = {0, 0, 4};
static vec light;

static float hit_sphere(vec o, vec d) {
    vec oc = sub(o, center);
    float b = dot(oc, d), c = dot(oc, oc) - 1;
    float disc = b * b - c;
    if (disc < 0) return -1;
    float t = -b - sqrtf(disc);
    return t > 1e-3f ? t : -1;
}

static vec trace(vec o, vec d, int depth) {
    float ts = hit_sphere(o, d);
    float tp = d.y < 0 ? (-1 - o.y) / d.y : -1;
    if (ts < 0 && tp < 0) return v(20, 30 + 40 * d.y, 60 + 120 * d.y);  // sky
    if (ts > 0 && (tp < 0 || ts < tp)) {
        vec p = add(o, mul(d, ts)), n = norm(sub(p, center));
        vec l = norm(sub(light, p));
        float diff = fmaxf(0, dot(n, l));
        vec r = sub(d, mul(n, 2 * dot(d, n)));
        float spec = powf(fmaxf(0, dot(r, l)), 30);
        vec base = mul(v(255, 60, 100), 0.15f + 0.85f * diff);
        if (depth < 2) base = add(mul(base, 0.7f), mul(trace(p, r, depth + 1), 0.3f));
        return add(base, v(255 * spec, 255 * spec, 255 * spec));
    }
    vec p = add(o, mul(d, tp));
    int check = ((int)floorf(p.x) + (int)floorf(p.z)) & 1;
    vec l = norm(sub(light, p));
    float shade = 0.25f + 0.75f * fmaxf(0, l.y);
    if (hit_sphere(p, l) > 0) shade *= 0.3f;  // shadow
    float fog = expf(-0.06f * tp);
    vec base = check ? v(230, 230, 230) : v(40, 40, 60);
    return add(mul(base, shade * fog), mul(v(20, 30, 60), 1 - fog));
}

int main(void) {
    printf("\033[2J");
    for (int frame = 0; frame < FRAMES; frame++) {
        float a = frame * 0.06f;
        light = v(5 * cosf(a), 3 + sinf(a * 0.7f), 1.5f + 2.5f * sinf(a));  // in front
        printf("\033[H");
        for (int y = 0; y < H; y++) {
            for (int x = 0; x < W; x++) {
                float u = (x - W / 2.0f) / (W / 2.0f);
                float w = (H / 2.0f - y) * 2.3f / (W / 2.0f);
                vec c = trace(v(0, 0.3f, 0), norm(v(u, w - 0.15f, 1.4f)), 0);
                printf("\033[38;2;%d;%d;%dm█", (int)fminf(255, c.x),
                       (int)fminf(255, c.y), (int)fminf(255, c.z));
            }
            putchar('\n');
        }
        printf("\033[0m");
        fflush(stdout);
        usleep(30000);
    }
    printf("\033[1;36m  [+] %d frames ray traced on the CPU\033[0m\n", FRAMES);
    return 0;
}
