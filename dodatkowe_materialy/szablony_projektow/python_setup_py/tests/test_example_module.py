# test_example_module.py
import unittest
from example_package.example_module import say_hello

class TestExampleModule(unittest.TestCase):

    def test_say_hello(self):
        self.assertEqual(say_hello("World"), "Hello, World!")

if __name__ == '__main__':
    unittest.main()
