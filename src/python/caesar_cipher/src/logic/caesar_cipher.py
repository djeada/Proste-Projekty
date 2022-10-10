class CeasarCipher:
    """
    Class for ciphering and deciphering messages according to the rules of the Caesar cipher.
    """

    def __init__(self, message: str, key: int):
        self.message = message
        self.key = key

    def cipher(self) -> str:
        """
        Returns ciphered message.
        """
        cipher_message = ""
        for letter in self.message:
            if letter.isalpha():
                if letter.isupper():
                    cipher_message += chr(
                        (ord(letter) + self.key - ord("A")) % 26 + ord("A")
                    )
                else:
                    cipher_message += chr(
                        (ord(letter) + self.key - ord("a")) % 26 + ord("a")
                    )
            else:
                cipher_message += letter
        return cipher_message

    def decipher(self) -> str:
        """
        Returns deciphered message.
        """
        decipher_message = ""
        for letter in self.message:
            if letter.isalpha():
                if letter.isupper():
                    decipher_message += chr(
                        (ord(letter) - self.key - ord("A")) % 26 + ord("A")
                    )
                else:
                    decipher_message += chr(
                        (ord(letter) - self.key - ord("a")) % 26 + ord("a")
                    )
            else:
                decipher_message += letter
        return decipher_message
