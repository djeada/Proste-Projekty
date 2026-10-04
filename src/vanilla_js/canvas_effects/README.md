# Canvas Effects (Vanilla JS)

The eleven [terminal effects](../../c/terminal_effects) ported from C to the browser: 3D graphics, fractals, simulations and algorithm visualizations drawn on a `<canvas>` with plain JavaScript. No libraries, no build step, no WebGL. Each effect is a single HTML file that you can open and read in one sitting.

Where the C programs had to make do with colored characters in a terminal, these pages draw real pixels: the Mandelbrot set and the plasma are computed pixel by pixel, the ray tracer fires one ray per pixel, and the Matrix rain uses smooth katakana glyphs.

## Effects

| Page | What it shows | Ideas inside |
|---|---|---|
| `donut.html` | A spinning 3D torus, shaded face by face in glowing green | Rotation matrices, perspective projection, surface normals, back-face culling, painter's algorithm |
| `mandelbrot_zoom.html` | A nearly 60,000× zoom into the Mandelbrot set's "seahorse valley" | Complex numbers, escape-time algorithm, smooth iteration count, color cycling |
| `game_of_life.html` | Conway's Game of Life on a wrapping grid, cells colored by age | Cellular automata, double buffering, toroidal neighbors |
| `matrix_rain.html` | The Matrix "digital rain", then "Wake up, Neo..." typed out | Per-column state, trails with color gradients, a typewriter effect |
| `doom_fire.html` | The PSX Doom fire: it burns, dies out and lights up again | Heat propagation with random cooling and wind, palettes |
| `spinning_cube.html` | A solid cube with six glowing faces | 3D rotation around three axes, back-face culling, flat shading |
| `plasma.html` | Demoscene plasma | Summed sine waves, a color lookup table, writing pixels with `ImageData` |
| `quicksort_visualizer.html` | Quicksort on 120 bars, one frame per swap, then a victory sweep | Lomuto partitioning, recursion, generator functions, Fisher-Yates shuffle |
| `maze_solver.html` | A random maze carved live, flooded with BFS, then solved | Depth-first search with an explicit stack, BFS shortest path |
| `ray_tracer.html` | A reflective sphere on a checkered floor with an orbiting light | Ray-sphere and ray-plane intersection, diffuse/specular light, shadows, reflections |
| `warp_starfield.html` | A 3D starfield accelerating to warp speed | Perspective projection, drawing far layers first, motion streaks |

`index.html` is a small gallery that links to all of them.

## How the pages work

- Every page fills the whole window and redraws itself when the window is resized.
- Animation runs on `requestAnimationFrame`, and every page moves its animation forward by the time that has passed, not by the number of frames. Effects look the same on 60 Hz and 120 Hz screens.
- The heavy per-pixel effects (Mandelbrot, plasma, ray tracer and fire) render into a smaller `ImageData`, about 60,000 to 100,000 pixels, and the browser scales the canvas up to fit the window. They stay smooth even on big screens.
- Where the C programs exit when they finish, the pages loop instead. Each effect plays out in about 10 seconds: the zoom restarts, the fire is lit again, a new maze is carved, the bars are shuffled again, and the starship sets off from home once more.

## Requirements

- A modern web browser (Chrome, Firefox, Safari or Edge)
- Node.js 18 or newer, only to run the tests

## Usage

Open `src/index.html`, or any single effect such as `src/donut.html`, directly in your browser.

You can also serve the folder over HTTP:

```sh
cd src
python3 -m http.server 8000
```

Then go to http://localhost:8000.

## Test

The smoke tests run each page's script in Node with a tiny fake browser: a stub canvas, a fake clock and a hand-cranked `requestAnimationFrame`. Each page runs through about 12 simulated seconds, including a window resize. The test checks that the page throws no errors, keeps animating and actually draws something. There are no dependencies to install:

```sh
npm test
```
