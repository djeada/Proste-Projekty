"""The tkinter window: a 320x240 canvas shown at 2x, a toolbar and keyboard shortcuts."""
import sys
import tkinter as tk
from tkinter import messagebox

from graphics_editor import BLACK, WHITE, Canvas, History, draw_line, draw_rect, flood_fill

CANVAS_W = 320
CANVAS_H = 240
SCALE = 2
DEFAULT_FILE = "drawing.png"
TOOLS = [("brush", "Brush"), ("eraser", "Eraser"), ("line", "Line"),
         ("rect", "Rect"), ("fill", "Fill"), ("picker", "Pick")]
WIDTHS = (1, 3, 5)
PALETTE = [
    (0, 0, 0), (255, 255, 255), (128, 128, 128), (160, 0, 0),
    (255, 0, 0), (255, 128, 0), (255, 230, 0), (0, 160, 0),
    (0, 220, 120), (0, 120, 255), (120, 0, 255), (255, 0, 200),
]


def to_hex(color):
    return "#%02x%02x%02x" % color


def to_photo(canvas):
    photo = tk.PhotoImage(width=canvas.width, height=canvas.height)
    rows = []
    for y in range(canvas.height):
        start = y * canvas.width
        rows.append([to_hex(c) for c in canvas.pixels[start:start + canvas.width]])
    photo.put(rows)
    return photo


class App:
    def __init__(self, root, path):
        self.root = root
        self.path = path
        self.canvas = Canvas(CANVAS_W, CANVAS_H)
        self.history = History()
        self.tool = "brush"
        self.width = WIDTHS[0]
        self.color = BLACK
        self.start = None
        self.last = None
        self.preview = None
        self.shown = None  # the image currently drawn on the canvas
        root.title("Graphics editor")
        self.build_toolbar()
        self.area = tk.Canvas(root, width=CANVAS_W * SCALE, height=CANVAS_H * SCALE,
                              bg="#c8c8c8", highlightthickness=0)
        self.area.pack(side="left")
        self.area.bind("<ButtonPress-1>", self.press)
        self.area.bind("<B1-Motion>", self.drag)
        self.area.bind("<ButtonRelease-1>", self.release)
        self.bind_keys()
        self.update_buttons()
        self.redraw()

    def build_toolbar(self):
        bar = tk.Frame(self.root, padx=6, pady=6)
        bar.pack(side="left", fill="y")
        self.tool_buttons = {}
        for name, label in TOOLS:
            button = tk.Button(bar, text=label, width=8, command=lambda n=name: self.select_tool(n))
            button.pack(fill="x", pady=1)
            self.tool_buttons[name] = button
        sizes = tk.Frame(bar)
        sizes.pack(fill="x", pady=(8, 0))
        self.size_buttons = {}
        for width in WIDTHS:
            button = tk.Button(sizes, text="%dpx" % width, width=3,
                               command=lambda w=width: self.select_width(w))
            button.pack(side="left", expand=True, fill="x")
            self.size_buttons[width] = button
        palette = tk.Frame(bar, pady=8)
        palette.pack(fill="x")
        for index, color in enumerate(PALETTE):
            swatch = tk.Button(palette, bg=to_hex(color), width=2, height=1,
                               command=lambda c=color: self.select_color(c))
            swatch.grid(row=index // 3, column=index % 3, padx=1, pady=1)
        self.current = tk.Label(bar, text="Color", bg=to_hex(self.color), width=8)
        self.current.pack(fill="x", pady=(0, 8))
        for text, command in [("Undo", self.undo), ("Clear", self.clear),
                              ("Open", self.open_png), ("Save", self.save_png)]:
            tk.Button(bar, text=text, width=8, command=command).pack(fill="x", pady=1)

    def bind_keys(self):
        for key, name in [("b", "brush"), ("e", "eraser"), ("l", "line"),
                          ("r", "rect"), ("f", "fill"), ("p", "picker")]:
            self.root.bind("<Key-%s>" % key, lambda event, n=name: self.select_tool(n))
        for key, width in zip("123", WIDTHS):
            self.root.bind("<Key-%s>" % key, lambda event, w=width: self.select_width(w))
        self.root.bind("<Key-n>", lambda event: self.clear())
        self.root.bind("<Control-z>", lambda event: self.undo())
        self.root.bind("<Control-s>", lambda event: self.save_png())
        self.root.bind("<Control-o>", lambda event: self.open_png())

    def update_buttons(self):
        for name, button in self.tool_buttons.items():
            button.config(relief="sunken" if name == self.tool else "raised")
        for width, button in self.size_buttons.items():
            button.config(relief="sunken" if width == self.width else "raised")
        self.current.config(bg=to_hex(self.color))

    def select_tool(self, name):
        self.tool = name
        self.update_buttons()

    def select_width(self, width):
        self.width = width
        self.update_buttons()

    def select_color(self, color):
        self.color = color
        self.update_buttons()

    def stroke_color(self):
        return WHITE if self.tool == "eraser" else self.color

    def redraw(self):
        """Shows the canvas, or the line or rectangle being dragged, at twice its size."""
        photo = to_photo(self.preview if self.preview is not None else self.canvas)
        self.shown = photo.zoom(SCALE, SCALE)  # kept on self, or Tk would drop the image
        self.area.delete("all")
        self.area.create_image(0, 0, anchor="nw", image=self.shown)

    def canvas_position(self, event):
        return event.x // SCALE, event.y // SCALE

    def press(self, event):
        x, y = self.canvas_position(event)
        if self.tool == "picker":
            if self.canvas.inside(x, y):
                self.color = self.canvas.get(x, y)
                self.update_buttons()
            return
        self.history.push(self.canvas)
        self.start = self.last = (x, y)
        if self.tool == "fill":
            flood_fill(self.canvas, x, y, self.color)
            self.start = None
        elif self.tool in ("brush", "eraser"):
            draw_line(self.canvas, x, y, x, y, self.width, self.stroke_color())
        self.redraw()

    def drag(self, event):
        if self.start is None:
            return
        x, y = self.canvas_position(event)
        if self.tool in ("brush", "eraser"):
            draw_line(self.canvas, self.last[0], self.last[1], x, y, self.width, self.stroke_color())
        else:
            self.preview = self.canvas.copy()
            self.draw_shape(self.preview, x, y)
        self.last = (x, y)
        self.redraw()

    def release(self, event):
        if self.start is None:
            return
        if self.tool in ("line", "rect"):
            self.draw_shape(self.canvas, *self.last)
        self.start = None
        self.preview = None
        self.redraw()

    def draw_shape(self, canvas, x, y):
        x0, y0 = self.start
        if self.tool == "line":
            draw_line(canvas, x0, y0, x, y, self.width, self.color)
        else:
            draw_rect(canvas, x0, y0, x, y, self.width, self.color)

    def undo(self):
        previous = self.history.undo()
        if previous is not None:
            self.canvas = previous
            self.redraw()

    def clear(self):
        self.history.push(self.canvas)
        self.canvas.clear()
        self.redraw()

    def save_png(self):
        try:
            to_photo(self.canvas).write(self.path, format="png")
        except tk.TclError as error:
            messagebox.showerror("Save", str(error))

    def open_png(self):
        try:
            photo = tk.PhotoImage(file=self.path)
        except tk.TclError as error:
            messagebox.showerror("Open", str(error))
            return
        self.history.push(self.canvas)
        self.canvas.clear()
        for y in range(min(photo.height(), CANVAS_H)):
            for x in range(min(photo.width(), CANVAS_W)):
                self.canvas.set(x, y, photo.get(x, y))
        self.redraw()


def main():
    root = tk.Tk()
    app = App(root, sys.argv[1] if len(sys.argv) > 1 else DEFAULT_FILE)
    if len(sys.argv) > 1:
        app.open_png()
    root.mainloop()


if __name__ == "__main__":
    main()
