import unittest
import sys
import os
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '../src/logic')))
import config

class TestHangmanConfig(unittest.TestCase):
    def test_word_list(self):
        self.assertTrue(hasattr(config, 'WORDS'))
        self.assertIsInstance(config.WORDS, list)
        self.assertTrue(len(config.WORDS) > 0)

if __name__ == '__main__':
    unittest.main()
