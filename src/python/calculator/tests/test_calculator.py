import unittest
import sys
import os
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '../src/logic')))
from calculator import Calculator

class TestCalculator(unittest.TestCase):
    def test_basic_operations(self):
        self.assertEqual(Calculator('2+2').eval_calculation_string(), 4)
        self.assertEqual(Calculator('5-3').eval_calculation_string(), 2)
        self.assertEqual(Calculator('4*3').eval_calculation_string(), 12)
        self.assertEqual(Calculator('8/2').eval_calculation_string(), 4)

    def test_square(self):
        calc = Calculator('3')
        calc.square()
        self.assertEqual(calc.eval_calculation_string(), 9)

    def test_square_root(self):
        calc = Calculator('16')
        calc.square_root()
        self.assertEqual(calc.eval_calculation_string(), 4)

    def test_invert(self):
        calc = Calculator('4')
        calc.invert()
        self.assertEqual(calc.eval_calculation_string(), 0.25)

    def test_invalid(self):
        self.assertFalse(Calculator('2++2').validate_calculation_string())
        self.assertFalse(Calculator('abc').validate_calculation_string())

    def test_valid(self):
        self.assertTrue(Calculator('2+2').validate_calculation_string())
        self.assertTrue(Calculator('3*4').validate_calculation_string())

if __name__ == '__main__':
    unittest.main()
