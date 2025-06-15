"""
Funkcje szyfrujące i deszyfrujące dla szyfru Cezara.
"""

def caesar_encrypt(text: str, key: int) -> str:
    """
    Szyfruje tekst szyfrem Cezara.
    Obsługuje duże/małe litery, znaki niealfabetyczne pozostawia bez zmian.
    """
    result = []
    for char in text:
        if char.isupper():
            offset = ord('A')
            result.append(chr((ord(char) - offset + key) % 26 + offset))
        elif char.islower():
            offset = ord('a')
            result.append(chr((ord(char) - offset + key) % 26 + offset))
        else:
            result.append(char)
    return ''.join(result)

def caesar_decrypt(text: str, key: int) -> str:
    """
    Deszyfruje tekst szyfrem Cezara.
    """
    return caesar_encrypt(text, -key)
