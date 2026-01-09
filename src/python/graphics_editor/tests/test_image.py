import unittest
import sys
import os
import tempfile

sys.path.insert(
    0, os.path.abspath(os.path.join(os.path.dirname(__file__), "../src/logic"))
)
from image import Image, Pixel


class TestPixel(unittest.TestCase):
    def test_pixel_creation(self):
        pixel = Pixel(255, 128, 0)
        self.assertEqual(pixel.r, 255)
        self.assertEqual(pixel.g, 128)
        self.assertEqual(pixel.b, 0)

    def test_pixel_to_hex(self):
        pixel = Pixel(255, 0, 128)
        self.assertEqual(pixel.to_hex(), "#ff0080")

    def test_pixel_from_hex(self):
        pixel = Pixel.from_hex("#ff0080")
        self.assertEqual(pixel.r, 255)
        self.assertEqual(pixel.g, 0)
        self.assertEqual(pixel.b, 128)

    def test_pixel_equality(self):
        p1 = Pixel(100, 100, 100)
        p2 = Pixel(100, 100, 100)
        self.assertEqual(p1, p2)


class TestImage(unittest.TestCase):
    def test_image_creation(self):
        img = Image(100, 100)
        self.assertEqual(img.width, 100)
        self.assertEqual(img.height, 100)

    def test_get_pixel(self):
        img = Image(100, 100)
        pixel = img.get_pixel(50, 50)
        self.assertIsNotNone(pixel)
        # Default is white
        self.assertEqual(pixel.r, 255)
        self.assertEqual(pixel.g, 255)
        self.assertEqual(pixel.b, 255)

    def test_set_pixel(self):
        img = Image(100, 100)
        result = img.set_pixel(50, 50, Pixel(255, 0, 0))
        self.assertTrue(result)
        pixel = img.get_pixel(50, 50)
        self.assertEqual(pixel.r, 255)
        self.assertEqual(pixel.g, 0)
        self.assertEqual(pixel.b, 0)

    def test_set_pixel_out_of_bounds(self):
        img = Image(100, 100)
        result = img.set_pixel(150, 150, Pixel(255, 0, 0))
        self.assertFalse(result)

    def test_fill(self):
        img = Image(10, 10)
        img.fill(Pixel(255, 0, 0))
        for y in range(10):
            for x in range(10):
                pixel = img.get_pixel(x, y)
                self.assertEqual(pixel.r, 255)
                self.assertEqual(pixel.g, 0)

    def test_clear(self):
        img = Image(10, 10)
        img.fill(Pixel(255, 0, 0))
        img.clear()
        pixel = img.get_pixel(5, 5)
        self.assertEqual(pixel.r, 255)
        self.assertEqual(pixel.g, 255)
        self.assertEqual(pixel.b, 255)

    def test_draw_line(self):
        img = Image(100, 100)
        img.draw_line(0, 0, 10, 10, Pixel(255, 0, 0))
        # Check that some pixels on the diagonal are set
        pixel = img.get_pixel(5, 5)
        self.assertEqual(pixel.r, 255)
        self.assertEqual(pixel.g, 0)

    def test_save_and_load_ppm(self):
        img = Image(10, 10)
        img.set_pixel(5, 5, Pixel(255, 0, 0))

        with tempfile.NamedTemporaryFile(suffix=".ppm", delete=False) as f:
            temp_path = f.name

        try:
            self.assertTrue(img.save_ppm(temp_path))

            img2 = Image(10, 10)
            self.assertTrue(img2.load_ppm(temp_path))
            pixel = img2.get_pixel(5, 5)
            self.assertEqual(pixel.r, 255)
            self.assertEqual(pixel.g, 0)
        finally:
            os.unlink(temp_path)


if __name__ == "__main__":
    unittest.main()
