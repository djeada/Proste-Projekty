// Page logic: draws the dice and the score card and handles clicks.
const statusText = document.getElementById('status');
const messageText = document.getElementById('message');
const rollsLeftText = document.getElementById('rolls-left');
const rollButton = document.getElementById('roll');
const dieButtons = Array.from(document.querySelectorAll('.die'));
const cardHead = document.getElementById('card-head');
const cardBody = document.getElementById('card-body');
const playersSelect = document.getElementById('players');

let game = null;

function rollFace() {
  return Math.floor(Math.random() * 6) + 1;
}

function say(text) {
  messageText.textContent = text;
}

function cellText(player, category) {
  const score = game.cards[player][category];
  if (score !== null) {
    return String(score);
  }
  if (player === game.current && game.rolls > 0) {
    return String(scoreFor(game.dice, category));
  }
  return '-';
}

function addRow(label, cells, className = '') {
  const row = document.createElement('tr');
  row.className = className;
  row.innerHTML = `<td>${label}</td>`;
  cells.forEach((cell) => row.appendChild(cell));
  cardBody.appendChild(row);
}

function cell(text) {
  const td = document.createElement('td');
  td.textContent = text;
  return td;
}

function renderCard() {
  cardHead.innerHTML = '';
  const headRow = document.createElement('tr');
  headRow.innerHTML = '<th>Category</th>';
  game.cards.forEach((_, player) => {
    const th = document.createElement('th');
    th.textContent = `Player ${player + 1}`;
    headRow.appendChild(th);
  });
  cardHead.appendChild(headRow);

  cardBody.innerHTML = '';
  const canScore = game.rolls > 0 && !isOver(game);
  CATEGORY_NAMES.forEach((name, category) => {
    const cells = game.cards.map((card, player) => {
      const td = cell(cellText(player, category));
      if (canScore && player === game.current && card[category] === null) {
        td.classList.add('possible');
        td.title = 'Score this category';
        td.addEventListener('click', () => scoreCategory(category));
      }
      return td;
    });
    addRow(name, cells);
  });
  addRow(`Upper bonus (${UPPER_BONUS_LIMIT}+)`, game.cards.map((card) => cell(String(upperBonus(card)))), 'summary');
  addRow('Total', game.cards.map((card) => cell(String(cardTotal(card)))), 'summary');
}

function renderDice() {
  dieButtons.forEach((button, die) => {
    button.textContent = game.rolls === 0 ? '-' : String(game.dice[die]);
    button.classList.toggle('held', game.held[die]);
    button.disabled = isOver(game);
  });
}

function render() {
  if (isOver(game)) {
    const best = winner(game);
    statusText.textContent = `Game over! Player ${best + 1} wins with ${cardTotal(game.cards[best])} points.`;
    rollsLeftText.textContent = '';
  } else {
    statusText.textContent = `Round ${game.round} of ${NUM_ROUNDS} - Player ${game.current + 1} to play`;
    rollsLeftText.textContent = `Rolls left: ${MAX_ROLLS - game.rolls}`;
  }
  rollButton.disabled = isOver(game) || game.rolls >= MAX_ROLLS;
  renderDice();
  renderCard();
}

function scoreCategory(category) {
  if (chooseCategory(game, category)) {
    say('');
  } else {
    say('That category is already used.');
  }
  render();
}

function startNewGame() {
  game = newGame(Number(playersSelect.value));
  say('Click a die to hold it, roll again, then click a green score to use it.');
  render();
}

rollButton.addEventListener('click', () => {
  say(rollDice(game, rollFace) ? '' : 'No rolls left. Click a score to finish the turn.');
  render();
});

dieButtons.forEach((button, die) => {
  button.addEventListener('click', () => {
    say(toggleHold(game, die) ? '' : 'Roll the dice first.');
    render();
  });
});

document.getElementById('new-game').addEventListener('click', startNewGame);

startNewGame();
