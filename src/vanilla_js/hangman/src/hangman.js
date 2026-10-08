// Rules of hangman: choosing a word, applying guesses and the game state.
const MAX_MISSES = 6;

const WORD_LIST = [
  { category: 'Animal', word: 'elephant' }, { category: 'Animal', word: 'giraffe' },
  { category: 'Animal', word: 'penguin' }, { category: 'Animal', word: 'kangaroo' },
  { category: 'Animal', word: 'dolphin' }, { category: 'Fruit', word: 'banana' },
  { category: 'Fruit', word: 'cherry' }, { category: 'Fruit', word: 'orange' },
  { category: 'Fruit', word: 'mango' }, { category: 'Fruit', word: 'lemon' },
  { category: 'Country', word: 'canada' }, { category: 'Country', word: 'norway' },
  { category: 'Country', word: 'brazil' }, { category: 'Country', word: 'japan' },
  { category: 'Country', word: 'egypt' }, { category: 'Coding', word: 'python' },
  { category: 'Coding', word: 'compiler' }, { category: 'Coding', word: 'function' },
  { category: 'Coding', word: 'keyboard' }, { category: 'Coding', word: 'variable' },
];

const GUESS = { HIT: 'hit', MISS: 'miss', REPEAT: 'repeat', INVALID: 'invalid' };
const STATUS = { PLAYING: 'playing', WON: 'won', LOST: 'lost' };

// random returns a number in [0, 1); pass a fixed function to make the choice repeatable.
function chooseEntry(list, random = Math.random) {
  return list[Math.floor(random() * list.length)];
}

function createGame(entry) {
  return { category: entry.category, word: entry.word, guessed: new Set(), misses: 0 };
}

function applyGuess(game, letter) {
  const key = String(letter).toLowerCase();
  if (!/^[a-z]$/.test(key)) return GUESS.INVALID;
  if (game.guessed.has(key)) return GUESS.REPEAT;
  game.guessed.add(key);
  if (game.word.includes(key)) return GUESS.HIT;
  game.misses += 1;
  return GUESS.MISS;
}

// The word with unknown letters as underscores, e.g. "_ a _ a _ a".
function maskedWord(game) {
  return [...game.word].map((letter) => (game.guessed.has(letter) ? letter : '_')).join(' ');
}

function gameStatus(game) {
  if (game.misses >= MAX_MISSES) return STATUS.LOST;
  if ([...game.word].every((letter) => game.guessed.has(letter))) return STATUS.WON;
  return STATUS.PLAYING;
}

if (typeof module !== 'undefined') {
  module.exports = {
    MAX_MISSES, WORD_LIST, GUESS, STATUS, chooseEntry, createGame, applyGuess, maskedWord, gameStatus,
  };
}
