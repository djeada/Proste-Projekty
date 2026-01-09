"""
GUI for Graphics Editor using tkinter.
"""
import tkinter as tk
from tkinter import ttk, colorchooser, filedialog
from enum import Enum, auto

import sys
import os

sys.path.insert(
    0, os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
)
from logic.image import Image, Pixel


class Tool(Enum):
    """Drawing tools."""

    PENCIL = auto()
    LINE = auto()
    RECTANGLE = auto()
    CIRCLE = auto()
    FILL = auto()


class Gui:
    """
    Main window for the Graphics Editor.
    """

    def __init__(self, image: Image) -> None:
        self.root = tk.Tk()
        self.root.title("Graphics Editor")
        self.image = image
        self.current_color = Pixel(0, 0, 0)
        self.current_tool = Tool.PENCIL
        self.start_x = 0
        self.start_y = 0
        self.setup()

    def run(self) -> None:
        """Start the main loop of the GUI."""
        self.root.mainloop()

    def setup(self) -> None:
        """Create the frames and internal widgets."""
        # Menu
        menubar = tk.Menu(self.root)
        self.root.config(menu=menubar)

        file_menu = tk.Menu(menubar, tearoff=0)
        menubar.add_cascade(label="File", menu=file_menu)
        file_menu.add_command(label="New", command=self.new_image)
        file_menu.add_command(label="Open...", command=self.open_image)
        file_menu.add_command(label="Save As...", command=self.save_image)
        file_menu.add_separator()
        file_menu.add_command(label="Exit", command=self.root.quit)

        # Toolbar
        toolbar = ttk.Frame(self.root)
        toolbar.pack(fill=tk.X, padx=5, pady=5)

        ttk.Button(toolbar, text="Pencil", command=lambda: self.set_tool(Tool.PENCIL)).pack(side=tk.LEFT, padx=2)
        ttk.Button(toolbar, text="Line", command=lambda: self.set_tool(Tool.LINE)).pack(side=tk.LEFT, padx=2)
        ttk.Button(toolbar, text="Rectangle", command=lambda: self.set_tool(Tool.RECTANGLE)).pack(side=tk.LEFT, padx=2)
        ttk.Button(toolbar, text="Circle", command=lambda: self.set_tool(Tool.CIRCLE)).pack(side=tk.LEFT, padx=2)
        ttk.Button(toolbar, text="Fill", command=lambda: self.set_tool(Tool.FILL)).pack(side=tk.LEFT, padx=2)

        ttk.Separator(toolbar, orient=tk.VERTICAL).pack(side=tk.LEFT, fill=tk.Y, padx=5)

        self.color_btn = tk.Button(
            toolbar, text="Color", bg="black", fg="white",
            command=self.choose_color, width=8
        )
        self.color_btn.pack(side=tk.LEFT, padx=2)

        ttk.Button(toolbar, text="Clear", command=self.clear_canvas).pack(side=tk.LEFT, padx=2)

        # Tool indicator
        self.tool_label = ttk.Label(toolbar, text="Tool: Pencil")
        self.tool_label.pack(side=tk.RIGHT, padx=10)

        # Canvas
        self.canvas = tk.Canvas(
            self.root,
            width=self.image.width,
            height=self.image.height,
            bg="white",
        )
        self.canvas.pack(padx=10, pady=10)

        # Bindings
        self.canvas.bind("<Button-1>", self.on_mouse_down)
        self.canvas.bind("<B1-Motion>", self.on_mouse_drag)
        self.canvas.bind("<ButtonRelease-1>", self.on_mouse_up)

    def set_tool(self, tool: Tool) -> None:
        """Set the current drawing tool."""
        self.current_tool = tool
        self.tool_label.config(text=f"Tool: {tool.name.title()}")

    def choose_color(self) -> None:
        """Open color chooser dialog."""
        color = colorchooser.askcolor(color=self.current_color.to_hex())
        if color[0]:
            r, g, b = int(color[0][0]), int(color[0][1]), int(color[0][2])
            self.current_color = Pixel(r, g, b)
            self.color_btn.config(bg=self.current_color.to_hex())

    def on_mouse_down(self, event: tk.Event) -> None:
        """Handle mouse button press."""
        self.start_x = event.x
        self.start_y = event.y

        if self.current_tool == Tool.PENCIL:
            self.image.set_pixel(event.x, event.y, self.current_color)
            self.canvas.create_oval(
                event.x - 1, event.y - 1, event.x + 1, event.y + 1,
                fill=self.current_color.to_hex(), outline=""
            )
        elif self.current_tool == Tool.FILL:
            self.image.bucket_fill(event.x, event.y, self.current_color)
            self.redraw_canvas()

    def on_mouse_drag(self, event: tk.Event) -> None:
        """Handle mouse drag."""
        if self.current_tool == Tool.PENCIL:
            self.image.draw_line(
                self.start_x, self.start_y, event.x, event.y, self.current_color
            )
            self.canvas.create_line(
                self.start_x, self.start_y, event.x, event.y,
                fill=self.current_color.to_hex(), width=2
            )
            self.start_x = event.x
            self.start_y = event.y

    def on_mouse_up(self, event: tk.Event) -> None:
        """Handle mouse button release."""
        if self.current_tool == Tool.LINE:
            self.image.draw_line(
                self.start_x, self.start_y, event.x, event.y, self.current_color
            )
            self.canvas.create_line(
                self.start_x, self.start_y, event.x, event.y,
                fill=self.current_color.to_hex(), width=2
            )
        elif self.current_tool == Tool.RECTANGLE:
            width = event.x - self.start_x
            height = event.y - self.start_y
            self.image.draw_rect(
                self.start_x, self.start_y, width, height, self.current_color
            )
            self.canvas.create_rectangle(
                self.start_x, self.start_y, event.x, event.y,
                outline=self.current_color.to_hex(), width=2
            )
        elif self.current_tool == Tool.CIRCLE:
            radius = int(
                ((event.x - self.start_x) ** 2 + (event.y - self.start_y) ** 2) ** 0.5
            )
            self.image.draw_circle(
                self.start_x, self.start_y, radius, self.current_color
            )
            self.canvas.create_oval(
                self.start_x - radius, self.start_y - radius,
                self.start_x + radius, self.start_y + radius,
                outline=self.current_color.to_hex(), width=2
            )

    def redraw_canvas(self) -> None:
        """Redraw the entire canvas from image data."""
        self.canvas.delete("all")
        for y in range(self.image.height):
            for x in range(self.image.width):
                pixel = self.image.get_pixel(x, y)
                if pixel and not (pixel.r == 255 and pixel.g == 255 and pixel.b == 255):
                    self.canvas.create_rectangle(
                        x, y, x + 1, y + 1,
                        fill=pixel.to_hex(), outline=""
                    )

    def clear_canvas(self) -> None:
        """Clear the canvas."""
        self.image.clear()
        self.canvas.delete("all")

    def new_image(self) -> None:
        """Create a new image."""
        self.image.clear()
        self.canvas.delete("all")

    def open_image(self) -> None:
        """Open an image file."""
        filename = filedialog.askopenfilename(
            filetypes=[("PPM Files", "*.ppm"), ("All Files", "*.*")]
        )
        if filename and self.image.load_ppm(filename):
            self.redraw_canvas()

    def save_image(self) -> None:
        """Save the image to a file."""
        filename = filedialog.asksaveasfilename(
            defaultextension=".ppm",
            filetypes=[("PPM Files", "*.ppm"), ("All Files", "*.*")]
        )
        if filename:
            self.image.save_ppm(filename)
