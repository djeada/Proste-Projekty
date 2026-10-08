from caesar_cipher import crack, decrypt, encrypt, shift_char

SENTENCE = "The quick brown fox jumps over the lazy dog and then runs away into the forest."


def test_shift_char_moves_letters():
    assert shift_char("a", 3) == "d"
    assert shift_char("z", 1) == "a"
    assert shift_char("Z", 1) == "A"
    assert shift_char("a", -1) == "z"


def test_encrypt_keeps_case_and_other_characters():
    assert encrypt("Hello, World!", 3) == "Khoor, Zruog!"
    assert encrypt("123 .,?", 5) == "123 .,?"
    assert encrypt("zażółć", 1) == "abżółć"
    assert encrypt("ąęśćżźłóń", 7) == "ąęśćżźłóń"


def test_negative_and_large_keys():
    assert encrypt("abc", -1) == "zab"
    assert encrypt("abc", 26) == "abc"
    assert encrypt("abc", 27) == "bcd"
    assert encrypt("abc", 52) == "abc"
    assert encrypt("abc", -27) == "zab"


def test_decrypt_reverses_encrypt():
    assert decrypt("Khoor, Zruog!", 3) == "Hello, World!"


def test_round_trip_for_many_keys():
    for key in (-30, -3, 0, 1, 7, 25, 52, 100):
        assert decrypt(encrypt(SENTENCE, key), key) == SENTENCE


def test_crack_finds_key_and_text():
    key, plain_text = crack(encrypt(SENTENCE, 7))
    assert key == 7
    assert plain_text == SENTENCE


def test_crack_without_letters_returns_text_unchanged():
    assert crack("1234 !?") == (0, "1234 !?")
