import unittest
import sys
import os
import tempfile

sys.path.insert(
    0, os.path.abspath(os.path.join(os.path.dirname(__file__), "../src/logic"))
)
from buffer import TextBuffer


class TestTextBuffer(unittest.TestCase):
    def test_initial_state(self):
        buffer = TextBuffer()
        self.assertEqual(buffer.line_count(), 0)
        self.assertFalse(buffer.is_modified())
        self.assertEqual(buffer.filename, "")

    def test_append_line(self):
        buffer = TextBuffer()
        buffer.append_line("Hello")
        self.assertEqual(buffer.line_count(), 1)
        self.assertEqual(buffer.get_line(0), "Hello")
        self.assertTrue(buffer.is_modified())

    def test_insert_line(self):
        buffer = TextBuffer()
        buffer.append_line("Line 1")
        buffer.append_line("Line 3")
        buffer.insert_line(1, "Line 2")
        self.assertEqual(buffer.get_line(1), "Line 2")
        self.assertEqual(buffer.line_count(), 3)

    def test_delete_line(self):
        buffer = TextBuffer()
        buffer.append_line("Line 1")
        buffer.append_line("Line 2")
        buffer.delete_line(0)
        self.assertEqual(buffer.line_count(), 1)
        self.assertEqual(buffer.get_line(0), "Line 2")

    def test_set_line(self):
        buffer = TextBuffer()
        buffer.append_line("Original")
        buffer.set_line(0, "Modified")
        self.assertEqual(buffer.get_line(0), "Modified")

    def test_get_content(self):
        buffer = TextBuffer()
        buffer.append_line("Line 1")
        buffer.append_line("Line 2")
        self.assertEqual(buffer.get_content(), "Line 1\nLine 2")

    def test_set_content(self):
        buffer = TextBuffer()
        buffer.set_content("Line 1\nLine 2\nLine 3")
        self.assertEqual(buffer.line_count(), 3)
        self.assertEqual(buffer.get_line(1), "Line 2")

    def test_clear(self):
        buffer = TextBuffer()
        buffer.append_line("Test")
        buffer.filename = "test.txt"
        buffer.clear()
        self.assertEqual(buffer.line_count(), 0)
        self.assertEqual(buffer.filename, "")
        self.assertFalse(buffer.is_modified())

    def test_save_and_load(self):
        buffer = TextBuffer()
        buffer.append_line("Line 1")
        buffer.append_line("Line 2")

        with tempfile.NamedTemporaryFile(mode="w", suffix=".txt", delete=False) as f:
            temp_path = f.name

        try:
            self.assertTrue(buffer.save(temp_path))
            self.assertFalse(buffer.is_modified())

            new_buffer = TextBuffer()
            self.assertTrue(new_buffer.load(temp_path))
            self.assertEqual(new_buffer.line_count(), 2)
            self.assertEqual(new_buffer.get_line(0), "Line 1")
        finally:
            os.unlink(temp_path)


if __name__ == "__main__":
    unittest.main()
