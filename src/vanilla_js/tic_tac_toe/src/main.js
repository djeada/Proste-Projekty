// Browser UI for tic-tac-toe: nine cells, a score and a mode switch.

const cellButtons = [];
const state = {
  board: newBoard(),
  turn: 'X',
  vsComputer: false,
  scores: { X: 0, O: 0, draw: 0 },
};

const boardElement = document.getElementById('board');
const statusElement = document.getElementById('status');
const scoreElement = document.getElementById('score');
const modeButton = document.getElementById('mode');

for (let cell = 0; cell < 9; cell++) {
  const button = document.createElement('button');
  button.className = 'cell';
  button.addEventListener('click', () => onCellClick(cell));
  boardElement.appendChild(button);
  cellButtons.push(button);
}

document.getElementById('new-round').addEventListener('click', newRound);
modeButton.addEventListener('click', () => {
  state.vsComputer = !state.vsComputer;
  newRound();
});

function newRound() {
  state.board = newBoard();
  state.turn = 'X';
  render();
}

// Places the mark of the player whose turn it is and counts the result when the round ends.
function place(cell) {
  state.board = play(state.board, cell, state.turn);
  if (isOver(state.board)) {
    state.scores[winner(state.board) || 'draw'] += 1;
  } else {
    state.turn = otherMark(state.turn);
  }
}

function onCellClick(cell) {
  if (isOver(state.board) || !legalMoves(state.board).includes(cell)) return;
  place(cell);
  if (state.vsComputer && !isOver(state.board)) {
    place(bestMove(state.board, state.turn));
  }
  render();
}

function render() {
  const line = winningLine(state.board) || [];
  cellButtons.forEach((button, cell) => {
    const mark = state.board[cell];
    button.textContent = mark || String(cell + 1);
    button.className = 'cell';
    if (mark) button.classList.add(mark === 'X' ? 'mark-x' : 'mark-o');
    if (line.includes(cell)) button.classList.add('win');
  });

  const won = winner(state.board);
  if (won) {
    statusElement.textContent = `${won} wins! Press New round to play again.`;
  } else if (isDraw(state.board)) {
    statusElement.textContent = 'It is a draw! Press New round to play again.';
  } else {
    statusElement.textContent = `Turn: ${state.turn}`;
  }

  modeButton.textContent = state.vsComputer ? 'Mode: vs computer' : 'Mode: two players';
  const { X, O, draw } = state.scores;
  scoreElement.textContent = `X wins: ${X}    O wins: ${O}    Draws: ${draw}`;
}

render();
