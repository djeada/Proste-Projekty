import unittest
import sys
import os
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '../src/logic')))
from caesar_cipher import caesar_encrypt, caesar_decrypt

class TestCaesarCipher(unittest.TestCase):
    def test_encrypt_basic(self):
        self.assertEqual(caesar_encrypt('abc', 1), 'bcd')
        self.assertEqual(caesar_encrypt('xyz', 2), 'zab')
        self.assertEqual(caesar_encrypt('ABC', 3), 'DEF')

    def test_decrypt_basic(self):
        self.assertEqual(caesar_decrypt('bcd', 1), 'abc')
        self.assertEqual(caesar_decrypt('zab', 2), 'xyz')
        self.assertEqual(caesar_decrypt('DEF', 3), 'ABC')

    def test_non_alpha(self):
        self.assertEqual(caesar_encrypt('a! b? c.', 1), 'b! c? d.')
        self.assertEqual(caesar_decrypt('b! c? d.', 1), 'a! b? c.')

    def test_mixed_case(self):
        self.assertEqual(caesar_encrypt('AbC xYz', 2), 'CdE zAb')
        self.assertEqual(caesar_decrypt('CdE zAb', 2), 'AbC xYz')

    def test_negative_key(self):
        self.assertEqual(caesar_encrypt('abc', -1), 'zab')
        self.assertEqual(caesar_decrypt('zab', -1), 'abc')

    def test_large_key(self):
        self.assertEqual(caesar_encrypt('abc', 27), 'bcd')
        self.assertEqual(caesar_decrypt('bcd', 27), 'abc')

    def test_empty(self):
        self.assertEqual(caesar_encrypt('', 5), '')
        self.assertEqual(caesar_decrypt('', 5), '')

if __name__ == '__main__':
    unittest.main()
