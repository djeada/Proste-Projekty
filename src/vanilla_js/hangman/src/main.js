// DOM, events and drawing of hangman. The rules are in hangman.js.
const MESSAGES = {
  [GUESS.HIT]: 'Good guess!',
  [GUESS.MISS]: 'Not in the word.',
  [GUESS.REPEAT]: 'You already guessed that letter.',
  [GUESS.INVALID]: 'Please enter a letter from a to z.',
};

const keyboard = document.getElementById('keyboard');
const messageEl = document.getElementById('message');
let game;
let message = '';

for (const letter of 'abcdefghijklmnopqrstuvwxyz') {
  const button = document.createElement('button');
  button.type = 'button';
  button.textContent = letter.toUpperCase();
  button.dataset.letter = letter;
  button.addEventListener('click', () => handleGuess(letter));
  keyboard.appendChild(button);
}

function render() {
  document.getElementById('category').textContent = `Category: ${game.category}`;
  document.getElementById('word').textContent = maskedWord(game);
  document.getElementById('misses').textContent = `Wrong guesses left: ${MAX_MISSES - game.misses}`;
  const guessed = [...game.guessed].sort().join(', ') || '-';
  document.getElementById('guessed').textContent = `Guessed: ${guessed}`;

  const status = gameStatus(game);
  if (status === STATUS.WON) message = `You won! The word was '${game.word}'.`;
  if (status === STATUS.LOST) message = `You lost. The word was '${game.word}'.`;
  messageEl.textContent = message;

  document.querySelectorAll('[data-stage]').forEach((part) => {
    part.classList.toggle('hidden', Number(part.dataset.stage) > game.misses);
  });
  keyboard.querySelectorAll('button').forEach((button) => {
    const letter = button.dataset.letter;
    button.disabled = status !== STATUS.PLAYING || game.guessed.has(letter);
  });
}

function newGame() {
  game = createGame(chooseEntry(WORD_LIST));
  message = '';
  render();
}

function handleGuess(letter) {
  if (gameStatus(game) !== STATUS.PLAYING) return;
  message = MESSAGES[applyGuess(game, letter)];
  render();
}

document.addEventListener('keydown', (event) => {
  if (event.ctrlKey || event.metaKey || event.altKey) return;
  if (/^[a-zA-Z]$/.test(event.key)) handleGuess(event.key.toLowerCase());
});
document.getElementById('new-game').addEventListener('click', newGame);

newGame();
