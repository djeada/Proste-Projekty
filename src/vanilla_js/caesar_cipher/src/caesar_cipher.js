// Caesar cipher: shifts the letters a-z and A-Z and leaves other characters unchanged.

// Relative frequency of a..z in English text, in percent.
const ENGLISH_FREQUENCY = [
  8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015, 6.094, 6.966, 0.153, 0.772, 4.025, 2.406,
  6.749, 7.507, 1.929, 0.095, 5.987, 6.327, 9.056, 2.758, 0.978, 2.360, 0.150, 1.974, 0.074,
];

// In JavaScript, % keeps the sign of the left operand (-1 % 26 is -1), so this helper is needed.
function mod(n, m) {
  return ((n % m) + m) % m;
}

// Index 0..25 of an ASCII letter (case ignored), or -1 for any other character.
function letterIndex(char) {
  const code = char.charCodeAt(0);
  if (code >= 65 && code <= 90) return code - 65;
  if (code >= 97 && code <= 122) return code - 97;
  return -1;
}

function shiftChar(char, key) {
  const index = letterIndex(char);
  if (index === -1) return char;
  const base = char === char.toUpperCase() ? 65 : 97;
  return String.fromCharCode(base + mod(index + key, 26));
}

function encrypt(text, key) {
  return [...text].map((char) => shiftChar(char, key)).join('');
}

function decrypt(text, key) {
  return encrypt(text, -key);
}

// Sum of English frequencies of the letters that appear when text is decrypted with key.
function letterScore(text, key) {
  let total = 0;
  for (const char of text) {
    const index = letterIndex(char);
    if (index !== -1) total += ENGLISH_FREQUENCY[mod(index - key, 26)];
  }
  return total;
}

// Tries all 26 keys and returns the most likely one together with the decrypted text.
function crack(text) {
  let bestKey = 0;
  let bestScore = -1;
  for (let key = 0; key < 26; key++) {
    const score = letterScore(text, key);
    if (score > bestScore) {
      bestScore = score;
      bestKey = key;
    }
  }
  return { key: bestKey, text: decrypt(text, bestKey) };
}

if (typeof module !== 'undefined') module.exports = { shiftChar, encrypt, decrypt, crack };
