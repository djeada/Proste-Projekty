// Browser interface: draws the two boards, handles clicks and runs the turns.
const COLUMN_LETTERS = 'ABCDEFGHIJ';

function coordinate(x, y) {
  return `${COLUMN_LETTERS[x]}${y + 1}`;
}

function createGame() {
  const game = {
    player: new Board(),
    enemy: new Board(),
    computer: new Computer(),
    horizontal: true,
    phase: 'placing', // 'placing', 'battle' or 'over'
    playerWon: false,
    preview: null,
    message: `Place your ships. Next ship: length ${FLEET[0]}. Click a cell on your board.`,
  };
  game.enemy.placeRandomly(Math.random);
  return game;
}

function nextShipIndex(board) {
  return board.ships.findIndex((ship) => !ship.placed);
}

function placeShip(game, x, y) {
  if (game.phase !== 'placing') return;
  const index = nextShipIndex(game.player);
  if (!game.player.place(index, x, y, game.horizontal)) {
    game.message = 'Cannot place a ship there: it leaves the board or overlaps another ship.';
  } else if (game.player.fleetPlaced()) {
    game.phase = 'battle';
    game.preview = null;
    game.message = 'All ships placed. Battle! Click a cell on the computer\'s board to fire.';
  } else {
    game.message = `Ship placed. Next ship: length ${game.player.ships[nextShipIndex(game.player)].length}.`;
  }
}

function randomFleet(game) {
  if (game.phase !== 'placing') return;
  game.player.placeRandomly(Math.random);
  game.phase = 'battle';
  game.preview = null;
  game.message = 'Random fleet placed. Battle! Click a cell on the computer\'s board to fire.';
}

function outcome(board, x, y, result) {
  if (result === SUNK) return `sunk a ship of length ${board.ships[board.shipAt[y][x]].length}`;
  return result === HIT ? 'hit' : 'miss';
}

function fire(game, x, y) {
  if (game.phase !== 'battle') return;
  const result = game.enemy.fire(x, y);
  if (result === REPEAT || result === INVALID) {
    game.message = 'You already fired at that cell. Choose another one.';
    return;
  }
  let text = `You fire at ${coordinate(x, y)}: ${outcome(game.enemy, x, y, result)}.`;
  if (game.enemy.allSunk()) {
    game.phase = 'over';
    game.playerWon = true;
    game.message = `${text} You won! All the computer's ships are sunk.`;
    return;
  }
  const target = game.computer.choose(game.player, Math.random);
  const reply = game.player.fire(target.x, target.y);
  game.computer.report(game.player, target.x, target.y, reply);
  text += ` Computer fires at ${coordinate(target.x, target.y)}: ${outcome(game.player, target.x, target.y, reply)}.`;
  if (game.player.allSunk()) {
    game.phase = 'over';
    game.playerWon = false;
    text += ' The computer sank your whole fleet. You lost.';
  }
  game.message = text;
}

function buildBoard(container, handlers) {
  const cells = Array.from({ length: BOARD_SIZE }, () => []);
  container.append(document.createElement('span'));
  for (const letter of COLUMN_LETTERS) {
    const label = document.createElement('span');
    label.textContent = letter;
    container.append(label);
  }
  for (let y = 0; y < BOARD_SIZE; y++) {
    const row = document.createElement('span');
    row.textContent = y + 1;
    container.append(row);
    for (let x = 0; x < BOARD_SIZE; x++) {
      const cell = document.createElement('button');
      cell.type = 'button';
      cell.className = 'cell';
      cell.setAttribute('aria-label', coordinate(x, y));
      cell.addEventListener('click', () => handlers.click(x, y));
      if (handlers.hover) cell.addEventListener('mouseenter', () => handlers.hover(x, y));
      container.append(cell);
      cells[y][x] = cell;
    }
  }
  if (handlers.hover) container.addEventListener('mouseleave', () => handlers.leave());
  return cells;
}

function paintBoard(cells, board, showShips) {
  for (let y = 0; y < BOARD_SIZE; y++) {
    for (let x = 0; x < BOARD_SIZE; x++) {
      const index = board.shipAt[y][x];
      let state = 'water';
      if (board.shot[y][x]) {
        if (index === null) state = 'miss';
        else state = isSunk(board.ships[index]) ? 'sunk' : 'hit';
      }
      else if (showShips && index !== null) state = 'ship';
      cells[y][x].className = `cell ${state}`;
      cells[y][x].textContent = { miss: '•', hit: 'X', sunk: '#' }[state] || '';
    }
  }
}

function showPreview(cells, game) {
  if (!game.preview) return;
  const index = nextShipIndex(game.player);
  const { x, y } = game.preview;
  const valid = game.player.canPlace(index, x, y, game.horizontal);
  for (const c of shipCells(game.player.ships[index].length, x, y, game.horizontal)) {
    if (inBounds(c.x, c.y)) cells[c.y][c.x].classList.add(valid ? 'preview-ok' : 'preview-bad');
  }
}

function render(game) {
  paintBoard(ownCells, game.player, true);
  if (game.phase === 'placing') showPreview(ownCells, game);
  paintBoard(enemyCells, game.enemy, game.phase === 'over');
  messageEl.textContent = game.message;
  resultEl.textContent = game.phase === 'over' ? (game.playerWon ? 'You won!' : 'You lost.') : '';
  rotateButton.textContent = `Orientation: ${game.horizontal ? 'horizontal' : 'vertical'}`;
}

const ownBoardEl = document.getElementById('own-board');
const enemyBoardEl = document.getElementById('enemy-board');
const messageEl = document.getElementById('message');
const resultEl = document.getElementById('result');
const rotateButton = document.getElementById('rotate');

let game = createGame();

// Runs an action on the game, then redraws the page.
function refresh(action) {
  action();
  render(game);
}

const ownCells = buildBoard(ownBoardEl, {
  click: (x, y) => refresh(() => placeShip(game, x, y)),
  hover: (x, y) => refresh(() => { game.preview = { x, y }; }),
  leave: () => refresh(() => { game.preview = null; }),
});
const enemyCells = buildBoard(enemyBoardEl, { click: (x, y) => refresh(() => fire(game, x, y)) });

rotateButton.addEventListener('click', () => refresh(() => { game.horizontal = !game.horizontal; }));
document.getElementById('random').addEventListener('click', () => refresh(() => randomFleet(game)));
document.getElementById('new-game').addEventListener('click', () => refresh(() => { game = createGame(); }));

render(game);
