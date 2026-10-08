"""The paint logic: canvas pixels, drawing primitives and the undo history. No tkinter here."""

HISTORY_DEPTH = 20
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)


class Canvas:
    """A width x height image; each pixel is an (r, g, b) tuple, rows stored one after another."""

    def __init__(self, width, height, fill=WHITE):
        self.width = width
        self.height = height
        self.pixels = [fill] * (width * height)

    def inside(self, x, y):
        return 0 <= x < self.width and 0 <= y < self.height

    def get(self, x, y):
        """Returns black for a position outside the canvas."""
        if not self.inside(x, y):
            return BLACK
        return self.pixels[y * self.width + x]

    def set(self, x, y, color):
        """Positions outside the canvas are ignored, so drawing may run past the edges."""
        if self.inside(x, y):
            self.pixels[y * self.width + x] = color

    def clear(self, color=WHITE):
        self.pixels = [color] * (self.width * self.height)

    def copy(self):
        clone = Canvas(self.width, self.height)
        clone.pixels = list(self.pixels)
        return clone


def stamp(canvas, x, y, width, color):
    """A square brush of the given width (1, 3 or 5) centred on (x, y)."""
    half = width // 2
    for dy in range(-half, half + 1):
        for dx in range(-half, half + 1):
            canvas.set(x + dx, y + dy, color)


def draw_line(canvas, x0, y0, x1, y1, width, color):
    """Bresenham's line algorithm: step along the longer axis one pixel at a time and use an
    error counter to decide when to also step along the shorter axis."""
    dx = abs(x1 - x0)
    sx = 1 if x0 < x1 else -1
    dy = -abs(y1 - y0)
    sy = 1 if y0 < y1 else -1
    err = dx + dy
    while True:
        stamp(canvas, x0, y0, width, color)
        if x0 == x1 and y0 == y1:
            break
        e2 = 2 * err
        if e2 >= dy:
            err += dy
            x0 += sx
        if e2 <= dx:
            err += dx
            y0 += sy


def draw_rect(canvas, x0, y0, x1, y1, width, color):
    """The outline of the rectangle whose opposite corners are (x0, y0) and (x1, y1)."""
    draw_line(canvas, x0, y0, x1, y0, width, color)
    draw_line(canvas, x1, y0, x1, y1, width, color)
    draw_line(canvas, x1, y1, x0, y1, width, color)
    draw_line(canvas, x0, y1, x0, y0, width, color)


def flood_fill(canvas, x, y, color):
    """Paints the region of pixels that have the same color as (x, y), stopping at other colors.

    Instead of recursion (which can overflow the call stack on a big canvas) the pixels still to
    visit go on an explicit stack. A pixel is painted as soon as it is pushed, so it is pushed once.
    """
    if not canvas.inside(x, y):
        return
    target = canvas.get(x, y)
    if target == color:
        return
    canvas.set(x, y, color)
    stack = [(x, y)]
    while stack:
        cx, cy = stack.pop()
        for nx, ny in ((cx + 1, cy), (cx - 1, cy), (cx, cy + 1), (cx, cy - 1)):
            if canvas.inside(nx, ny) and canvas.get(nx, ny) == target:
                canvas.set(nx, ny, color)
                stack.append((nx, ny))


class History:
    """Snapshots of the canvas, oldest first. The oldest one is dropped when the history is full."""

    def __init__(self):
        self._snapshots = []

    def push(self, canvas):
        self._snapshots.append(canvas.copy())
        del self._snapshots[:-HISTORY_DEPTH]

    def undo(self):
        """Returns the previous canvas, or None when there is nothing to undo."""
        return self._snapshots.pop() if self._snapshots else None
