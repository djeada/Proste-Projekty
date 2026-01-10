## About the Project

A Caesar Cipher application that allows users to encrypt and decrypt text using one of the oldest known cryptographic methods. The cipher shifts each letter in the text by a specified number of positions in the alphabet.

## Screenshots

![caesar cipher](https://user-images.githubusercontent.com/37275728/194821911-e403023e-c5e5-4b19-b8bb-5cfedae8f164.gif)

## Requirements

* A modern web browser, such as the most recent versions of Chrome, Firefox, or Safari.
* There are no other libraries or frameworks required.

## Installation

1. Clone or download the repository.
2. Open `src/index.html` in your web browser.

## Usage

1. Enter the text to be encrypted or decrypted in the message input field.
2. Enter the shift value (key) using the number input.
3. Select whether the algorithm should encrypt or decrypt the message.
4. Press the Run button.
5. The processed message appears in the output area.

## Features

* Encrypt and decrypt text with any integer shift value.
* Handles both uppercase and lowercase letters.
* Preserves non-alphabetic characters (numbers, punctuation, spaces).
* Copy result to clipboard functionality.

## Possible improvements

Some of the ideas include:

* Implement other cipher algorithms (Vigenère, ROT13, etc.).
* Add brute force decryption to try all possible shifts.
* Add frequency analysis for cryptanalysis.
