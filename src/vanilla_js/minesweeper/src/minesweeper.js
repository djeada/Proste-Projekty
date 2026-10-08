// Minesweeper rules: mines, numbers, reveal with flood fill, flags. No DOM here.

// Seeded random numbers (mulberry32): the same seed always gives the same mine layout.
function makeRandom(seed) {
  let state = seed >>> 0;
  return () => {
    state = (state + 0x6d2b79f5) >>> 0;
    let t = state;
    t = Math.imul(t ^ (t >>> 15), t | 1);
    t ^= t + Math.imul(t ^ (t >>> 7), t | 61);
    return ((t ^ (t >>> 14)) >>> 0) / 4294967296;
  };
}

function makeGrid(rows, cols, value) {
  return Array.from({ length: rows }, () => Array(cols).fill(value));
}

class Game {
  constructor(rows, cols, mines, random = Math.random) {
    this.rows = rows;
    this.cols = cols;
    this.mineTotal = mines;
    this.random = random;
    this.minesPlaced = false;
    this.mine = makeGrid(rows, cols, false);
    this.neighbors = makeGrid(rows, cols, 0);
    this.revealed = makeGrid(rows, cols, false);
    this.flagged = makeGrid(rows, cols, false);
    this.revealedCount = 0;
    this.flagCount = 0;
    this.state = 'playing'; // 'playing', 'won' or 'lost'
  }

  get minesLeft() {
    return this.mineTotal - this.flagCount;
  }

  reveal(row, col) {
    if (this.state !== 'playing' || !this.inBoard(row, col)) return;
    if (this.revealed[row][col] || this.flagged[row][col]) return;
    if (!this.minesPlaced) this.placeMines(row, col);
    if (this.mine[row][col]) {
      for (let r = 0; r < this.rows; r++) {
        for (let c = 0; c < this.cols; c++) {
          if (this.mine[r][c]) this.revealed[r][c] = true;
        }
      }
      this.state = 'lost';
      return;
    }
    this.revealCell(row, col);
    if (this.revealedCount === this.rows * this.cols - this.mineTotal) this.state = 'won';
  }

  toggleFlag(row, col) {
    if (this.state !== 'playing' || !this.inBoard(row, col) || this.revealed[row][col]) return;
    this.flagged[row][col] = !this.flagged[row][col];
    this.flagCount += this.flagged[row][col] ? 1 : -1;
  }

  inBoard(row, col) {
    return row >= 0 && row < this.rows && col >= 0 && col < this.cols;
  }

  // Places random mines, skipping the first clicked cell and its neighbors.
  placeMines(safeRow, safeCol) {
    let placed = 0;
    while (placed < this.mineTotal) {
      const row = Math.floor(this.random() * this.rows);
      const col = Math.floor(this.random() * this.cols);
      if (this.mine[row][col] || (Math.abs(row - safeRow) <= 1 && Math.abs(col - safeCol) <= 1)) {
        continue;
      }
      this.mine[row][col] = true;
      placed++;
    }
    for (let r = 0; r < this.rows; r++) {
      for (let c = 0; c < this.cols; c++) {
        this.neighbors[r][c] = this.countMinesAround(r, c);
      }
    }
    this.minesPlaced = true;
  }

  countMinesAround(row, col) {
    let count = 0;
    for (let r = row - 1; r <= row + 1; r++) {
      for (let c = col - 1; c <= col + 1; c++) {
        if ((r !== row || c !== col) && this.inBoard(r, c) && this.mine[r][c]) count++;
      }
    }
    return count;
  }

  // Opens a cell. An empty cell (no mines around) also opens its neighbors: flood fill.
  revealCell(row, col) {
    if (!this.inBoard(row, col) || this.revealed[row][col] || this.flagged[row][col]) return;
    this.revealed[row][col] = true;
    this.revealedCount++;
    if (this.neighbors[row][col] === 0) {
      for (let r = row - 1; r <= row + 1; r++) {
        for (let c = col - 1; c <= col + 1; c++) {
          this.revealCell(r, c);
        }
      }
    }
  }
}

if (typeof module !== 'undefined') module.exports = { Game, makeRandom };
