const words = [
  'python', 'hangman', 'programming', 'challenge', 'javascript',
  'computer', 'algorithm', 'developer', 'keyboard', 'function',
  'variable', 'constant', 'browser', 'database', 'network'
];

const maxAttempts = 10;
let currentWord = '';
let guessedLetters = [];
let attemptsLeft = maxAttempts;
let gameOver = false;

const wordDisplay = document.getElementById('word-display');
const keyboard = document.getElementById('keyboard');
const message = document.getElementById('message');
const attemptsCount = document.getElementById('attempts-count');
const newGameBtn = document.getElementById('new-game');

function init() {
  currentWord = words[Math.floor(Math.random() * words.length)].toUpperCase();
  guessedLetters = [];
  attemptsLeft = maxAttempts;
  gameOver = false;
  message.textContent = '';
  message.className = 'message';
  attemptsCount.textContent = attemptsLeft;
  renderWord();
  renderKeyboard();
}

function renderWord() {
  wordDisplay.innerHTML = '';
  for (const letter of currentWord) {
    const letterBox = document.createElement('div');
    letterBox.className = 'letter-box';
    letterBox.textContent = guessedLetters.includes(letter) ? letter : '';
    wordDisplay.appendChild(letterBox);
  }
}

function renderKeyboard() {
  keyboard.innerHTML = '';
  for (let i = 65; i <= 90; i++) {
    const letter = String.fromCharCode(i);
    const btn = document.createElement('button');
    btn.className = 'key-btn';
    btn.textContent = letter;
    btn.disabled = guessedLetters.includes(letter) || gameOver;
    
    if (guessedLetters.includes(letter)) {
      btn.classList.add(currentWord.includes(letter) ? 'correct' : 'wrong');
    }
    
    btn.addEventListener('click', () => guessLetter(letter));
    keyboard.appendChild(btn);
  }
}

function guessLetter(letter) {
  if (gameOver || guessedLetters.includes(letter)) return;
  
  guessedLetters.push(letter);
  
  if (!currentWord.includes(letter)) {
    attemptsLeft--;
    attemptsCount.textContent = attemptsLeft;
  }
  
  renderWord();
  renderKeyboard();
  checkGameState();
}

function checkGameState() {
  const wordGuessed = currentWord.split('').every(letter => guessedLetters.includes(letter));
  
  if (wordGuessed) {
    gameOver = true;
    message.textContent = 'Congratulations! You won!';
    message.className = 'message win';
    renderKeyboard();
  } else if (attemptsLeft <= 0) {
    gameOver = true;
    message.textContent = `Game Over! The word was: ${currentWord}`;
    message.className = 'message lose';
    renderKeyboard();
  }
}

document.addEventListener('keydown', (e) => {
  const key = e.key.toUpperCase();
  if (key.length === 1 && key >= 'A' && key <= 'Z') {
    guessLetter(key);
  }
});

newGameBtn.addEventListener('click', init);

init();
