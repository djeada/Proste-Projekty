const diceElement = document.getElementById('dice');
const rollsLeftElement = document.getElementById('rolls-left');
const rollBtn = document.getElementById('roll');
const newGameBtn = document.getElementById('new-game');
const totalScoreElement = document.getElementById('total-score');
const upperBonusElement = document.getElementById('upper-bonus');

const scoreRows = document.querySelectorAll('.score-row[data-category]');

let dice = [0, 0, 0, 0, 0];
let held = [false, false, false, false, false];
let rollsLeft = 3;
let scores = {};
let gameOver = false;

const categories = [
  'ones', 'twos', 'threes', 'fours', 'fives', 'sixes',
  'threeOfKind', 'fourOfKind', 'fullHouse', 'smallStraight', 'largeStraight', 'yahtzee', 'chance'
];

function init() {
  dice = [0, 0, 0, 0, 0];
  held = [false, false, false, false, false];
  rollsLeft = 3;
  scores = {};
  gameOver = false;
  
  rollsLeftElement.textContent = '3';
  rollBtn.disabled = false;
  totalScoreElement.textContent = '0';
  upperBonusElement.textContent = '0';
  
  scoreRows.forEach(row => {
    row.classList.remove('used', 'available');
    const valueSpan = row.querySelector('.score-value');
    if (valueSpan) valueSpan.textContent = '-';
  });
  
  renderDice();
}

function rollDice() {
  if (rollsLeft <= 0 || gameOver) return;
  
  for (let i = 0; i < 5; i++) {
    if (!held[i]) {
      dice[i] = Math.floor(Math.random() * 6) + 1;
    }
  }
  
  rollsLeft--;
  rollsLeftElement.textContent = rollsLeft;
  
  if (rollsLeft === 0) {
    rollBtn.disabled = true;
  }
  
  renderDice();
  updateAvailableScores();
}

function renderDice() {
  diceElement.innerHTML = '';
  
  for (let i = 0; i < 5; i++) {
    const die = document.createElement('div');
    die.className = 'die' + (held[i] ? ' held' : '');
    die.textContent = dice[i] || '';
    die.addEventListener('click', () => toggleHold(i));
    diceElement.appendChild(die);
  }
}

function toggleHold(index) {
  if (rollsLeft === 3 || dice[index] === 0) return;
  held[index] = !held[index];
  renderDice();
}

function updateAvailableScores() {
  scoreRows.forEach(row => {
    const category = row.dataset.category;
    if (!scores.hasOwnProperty(category)) {
      row.classList.add('available');
      const potential = calculateScore(category);
      const valueSpan = row.querySelector('.score-value');
      if (valueSpan) valueSpan.textContent = potential;
    }
  });
}

function calculateScore(category) {
  const counts = [0, 0, 0, 0, 0, 0, 0];
  const sum = dice.reduce((a, b) => a + b, 0);
  
  for (const d of dice) {
    counts[d]++;
  }
  
  switch (category) {
    case 'ones': return counts[1] * 1;
    case 'twos': return counts[2] * 2;
    case 'threes': return counts[3] * 3;
    case 'fours': return counts[4] * 4;
    case 'fives': return counts[5] * 5;
    case 'sixes': return counts[6] * 6;
    
    case 'threeOfKind':
      return counts.some(c => c >= 3) ? sum : 0;
    
    case 'fourOfKind':
      return counts.some(c => c >= 4) ? sum : 0;
    
    case 'fullHouse':
      const hasThree = counts.some(c => c === 3);
      const hasTwo = counts.some(c => c === 2);
      return (hasThree && hasTwo) ? 25 : 0;
    
    case 'smallStraight':
      const sorted = [...new Set(dice)].sort();
      const str = sorted.join('');
      return (str.includes('1234') || str.includes('2345') || str.includes('3456')) ? 30 : 0;
    
    case 'largeStraight':
      const sortedL = [...dice].sort().join('');
      return (sortedL === '12345' || sortedL === '23456') ? 40 : 0;
    
    case 'yahtzee':
      return counts.some(c => c === 5) ? 50 : 0;
    
    case 'chance':
      return sum;
    
    default:
      return 0;
  }
}

function selectCategory(category) {
  if (scores.hasOwnProperty(category) || rollsLeft === 3 || gameOver) return;
  
  const score = calculateScore(category);
  scores[category] = score;
  
  const row = document.querySelector(`.score-row[data-category="${category}"]`);
  row.classList.remove('available');
  row.classList.add('used');
  
  const valueSpan = row.querySelector('.score-value');
  if (valueSpan) valueSpan.textContent = score;
  
  // Reset for next turn
  rollsLeft = 3;
  rollsLeftElement.textContent = '3';
  held = [false, false, false, false, false];
  dice = [0, 0, 0, 0, 0];
  rollBtn.disabled = false;
  
  // Clear available markers
  scoreRows.forEach(r => {
    if (!scores.hasOwnProperty(r.dataset.category)) {
      r.classList.remove('available');
      const val = r.querySelector('.score-value');
      if (val) val.textContent = '-';
    }
  });
  
  updateTotalScore();
  renderDice();
  checkGameOver();
}

function updateTotalScore() {
  const upperCategories = ['ones', 'twos', 'threes', 'fours', 'fives', 'sixes'];
  const upperSum = upperCategories.reduce((sum, cat) => sum + (scores[cat] || 0), 0);
  const upperBonus = upperSum >= 63 ? 35 : 0;
  upperBonusElement.textContent = upperBonus;
  
  const total = Object.values(scores).reduce((a, b) => a + b, 0) + upperBonus;
  totalScoreElement.textContent = total;
}

function checkGameOver() {
  if (Object.keys(scores).length === categories.length) {
    gameOver = true;
    rollBtn.disabled = true;
  }
}

scoreRows.forEach(row => {
  row.addEventListener('click', () => {
    selectCategory(row.dataset.category);
  });
});

rollBtn.addEventListener('click', rollDice);
newGameBtn.addEventListener('click', init);

init();
