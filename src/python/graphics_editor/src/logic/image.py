"""
Simple graphics editor image logic.
"""
from typing import List, Tuple, Optional
from dataclasses import dataclass


@dataclass
class Pixel:
    """Represents a pixel color."""

    r: int
    g: int
    b: int

    def __eq__(self, other: object) -> bool:
        if not isinstance(other, Pixel):
            return False
        return self.r == other.r and self.g == other.g and self.b == other.b

    def to_hex(self) -> str:
        """Convert to hex color string."""
        return f"#{self.r:02x}{self.g:02x}{self.b:02x}"

    @classmethod
    def from_hex(cls, hex_color: str) -> "Pixel":
        """Create from hex color string."""
        hex_color = hex_color.lstrip("#")
        return cls(
            r=int(hex_color[0:2], 16),
            g=int(hex_color[2:4], 16),
            b=int(hex_color[4:6], 16),
        )


class Image:
    """
    Represents an image as a 2D array of pixels.
    """

    MAX_WIDTH = 1024
    MAX_HEIGHT = 1024

    def __init__(self, width: int, height: int) -> None:
        self.width = min(width, self.MAX_WIDTH)
        self.height = min(height, self.MAX_HEIGHT)
        self.data: List[List[Pixel]] = [
            [Pixel(255, 255, 255) for _ in range(self.width)]
            for _ in range(self.height)
        ]
        self.modified = False

    def get_pixel(self, x: int, y: int) -> Optional[Pixel]:
        """Get pixel at coordinates."""
        if 0 <= x < self.width and 0 <= y < self.height:
            return self.data[y][x]
        return None

    def set_pixel(self, x: int, y: int, color: Pixel) -> bool:
        """Set pixel at coordinates."""
        if 0 <= x < self.width and 0 <= y < self.height:
            self.data[y][x] = color
            self.modified = True
            return True
        return False

    def fill(self, color: Pixel) -> None:
        """Fill entire image with a color."""
        for y in range(self.height):
            for x in range(self.width):
                self.data[y][x] = Pixel(color.r, color.g, color.b)
        self.modified = True

    def draw_line(
        self, x1: int, y1: int, x2: int, y2: int, color: Pixel
    ) -> None:
        """Draw a line using Bresenham's algorithm."""
        dx = abs(x2 - x1)
        dy = abs(y2 - y1)
        sx = 1 if x1 < x2 else -1
        sy = 1 if y1 < y2 else -1
        err = dx - dy

        while True:
            self.set_pixel(x1, y1, color)
            if x1 == x2 and y1 == y2:
                break
            e2 = 2 * err
            if e2 > -dy:
                err -= dy
                x1 += sx
            if e2 < dx:
                err += dx
                y1 += sy

    def draw_rect(
        self, x: int, y: int, width: int, height: int, color: Pixel
    ) -> None:
        """Draw a rectangle outline."""
        self.draw_line(x, y, x + width, y, color)
        self.draw_line(x + width, y, x + width, y + height, color)
        self.draw_line(x + width, y + height, x, y + height, color)
        self.draw_line(x, y + height, x, y, color)

    def fill_rect(
        self, x: int, y: int, width: int, height: int, color: Pixel
    ) -> None:
        """Draw a filled rectangle."""
        for py in range(y, min(y + height, self.height)):
            for px in range(x, min(x + width, self.width)):
                self.set_pixel(px, py, color)

    def draw_circle(self, cx: int, cy: int, radius: int, color: Pixel) -> None:
        """Draw a circle outline using midpoint algorithm."""
        x = radius
        y = 0
        err = 0

        while x >= y:
            self.set_pixel(cx + x, cy + y, color)
            self.set_pixel(cx + y, cy + x, color)
            self.set_pixel(cx - y, cy + x, color)
            self.set_pixel(cx - x, cy + y, color)
            self.set_pixel(cx - x, cy - y, color)
            self.set_pixel(cx - y, cy - x, color)
            self.set_pixel(cx + y, cy - x, color)
            self.set_pixel(cx + x, cy - y, color)

            y += 1
            if err <= 0:
                err += 2 * y + 1
            if err > 0:
                x -= 1
                err -= 2 * x + 1

    def bucket_fill(self, x: int, y: int, new_color: Pixel) -> None:
        """Flood fill starting from a point."""
        if not (0 <= x < self.width and 0 <= y < self.height):
            return

        original_color = self.get_pixel(x, y)
        if original_color == new_color:
            return

        stack = [(x, y)]
        while stack:
            px, py = stack.pop()
            if not (0 <= px < self.width and 0 <= py < self.height):
                continue
            if self.get_pixel(px, py) != original_color:
                continue

            self.set_pixel(px, py, new_color)
            stack.extend([(px + 1, py), (px - 1, py), (px, py + 1), (px, py - 1)])

    def clear(self) -> None:
        """Clear the image to white."""
        self.fill(Pixel(255, 255, 255))

    def save_ppm(self, filename: str) -> bool:
        """Save image to PPM format."""
        try:
            with open(filename, "w") as f:
                f.write("P3\n")
                f.write(f"{self.width} {self.height}\n")
                f.write("255\n")
                for row in self.data:
                    for pixel in row:
                        f.write(f"{pixel.r} {pixel.g} {pixel.b} ")
                    f.write("\n")
            self.modified = False
            return True
        except IOError:
            return False

    def load_ppm(self, filename: str) -> bool:
        """Load image from PPM format."""
        try:
            with open(filename, "r") as f:
                magic = f.readline().strip()
                if magic != "P3":
                    return False

                # Skip comments
                line = f.readline()
                while line.startswith("#"):
                    line = f.readline()

                dims = line.strip().split()
                width, height = int(dims[0]), int(dims[1])
                max_val = int(f.readline().strip())

                self.width = min(width, self.MAX_WIDTH)
                self.height = min(height, self.MAX_HEIGHT)
                self.data = []

                values = []
                for line in f:
                    values.extend(line.strip().split())

                idx = 0
                for y in range(self.height):
                    row = []
                    for x in range(self.width):
                        r = int(values[idx])
                        g = int(values[idx + 1])
                        b = int(values[idx + 2])
                        row.append(Pixel(r, g, b))
                        idx += 3
                    self.data.append(row)

                self.modified = False
                return True
        except (IOError, IndexError, ValueError):
            return False
