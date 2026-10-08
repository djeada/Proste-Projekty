"""Caesar cipher: shifts the letters a-z and A-Z and leaves other characters unchanged."""

# Relative frequency of a..z in English text, in percent.
ENGLISH_FREQUENCY = (
    8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015, 6.094, 6.966, 0.153, 0.772, 4.025, 2.406,
    6.749, 7.507, 1.929, 0.095, 5.987, 6.327, 9.056, 2.758, 0.978, 2.360, 0.150, 1.974, 0.074,
)


def shift_char(char: str, key: int) -> str:
    """Shifts one ASCII letter by key, keeping its case. Other characters are returned as they are."""
    if not (char.isascii() and char.isalpha()):
        return char
    base = ord("A") if char.isupper() else ord("a")
    # Python's % never returns a negative result for a positive divisor, so no extra care is needed.
    return chr((ord(char) - base + key) % 26 + base)


def encrypt(text: str, key: int) -> str:
    """Encrypts text. The key may be negative or larger than 25."""
    return "".join(shift_char(char, key) for char in text)


def decrypt(text: str, key: int) -> str:
    """Decrypts text that was encrypted with key."""
    return encrypt(text, -key)


def _letter_score(text: str, key: int) -> float:
    """Sum of English frequencies of the letters that appear when text is decrypted with key."""
    return sum(
        ENGLISH_FREQUENCY[(ord(char.lower()) - ord("a") - key) % 26]
        for char in text
        if char.isascii() and char.isalpha()
    )


def crack(text: str) -> tuple:
    """Tries all 26 keys and returns (most likely key, decrypted text)."""
    key = max(range(26), key=lambda candidate: _letter_score(text, candidate))
    return key, decrypt(text, key)
