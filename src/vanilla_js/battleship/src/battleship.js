// Battleship rules: boards, ship placement, shooting and the computer player.
const BOARD_SIZE = 10;
const FLEET = [5, 4, 3, 3, 2];

const MISS = 'miss';
const HIT = 'hit';
const SUNK = 'sunk';
const REPEAT = 'repeat'; // this cell was already shot
const INVALID = 'invalid'; // outside the board

function inBounds(x, y) {
  return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE;
}

// random() returns a number from 0 (inclusive) to 1 (exclusive); Math.random works.
function randomInt(random, n) {
  return Math.floor(random() * n);
}

function shipCells(length, x, y, horizontal) {
  const cells = [];
  for (let i = 0; i < length; i++) {
    cells.push({ x: horizontal ? x + i : x, y: horizontal ? y : y + i });
  }
  return cells;
}

function gridOf(value) {
  return Array.from({ length: BOARD_SIZE }, () => Array(BOARD_SIZE).fill(value));
}

class Board {
  constructor() {
    this.reset();
  }

  reset() {
    this.ships = FLEET.map((length) => ({ length, hits: 0, placed: false }));
    this.shipAt = gridOf(null); // index of the ship on a cell, or null for water
    this.shot = gridOf(false);
  }

  canPlace(index, x, y, horizontal) {
    const ship = this.ships[index];
    if (!ship || ship.placed) return false;
    return shipCells(ship.length, x, y, horizontal).every(
      (c) => inBounds(c.x, c.y) && this.shipAt[c.y][c.x] === null,
    );
  }

  place(index, x, y, horizontal) {
    if (!this.canPlace(index, x, y, horizontal)) return false;
    for (const c of shipCells(this.ships[index].length, x, y, horizontal)) {
      this.shipAt[c.y][c.x] = index;
    }
    this.ships[index].placed = true;
    return true;
  }

  // Replaces the whole fleet with a random valid one. Ships may touch but not overlap.
  placeRandomly(random) {
    let placed = false;
    while (!placed) {
      this.reset();
      placed = this.ships.every((_, index) => this.placeOneRandomly(index, random));
    }
  }

  placeOneRandomly(index, random) {
    for (let tries = 0; tries < 1000; tries++) {
      const x = randomInt(random, BOARD_SIZE);
      const y = randomInt(random, BOARD_SIZE);
      if (this.place(index, x, y, random() < 0.5)) return true;
    }
    return false;
  }

  fleetPlaced() {
    return this.ships.every((ship) => ship.placed);
  }

  fire(x, y) {
    if (!inBounds(x, y)) return INVALID;
    if (this.shot[y][x]) return REPEAT;
    this.shot[y][x] = true;
    const index = this.shipAt[y][x];
    if (index === null) return MISS;
    const ship = this.ships[index];
    ship.hits++;
    return isSunk(ship) ? SUNK : HIT;
  }

  allSunk() {
    return this.ships.every(isSunk);
  }
}

function isSunk(ship) {
  return ship.placed && ship.hits === ship.length;
}

// Hunt and target: random shots until a hit, then the neighbours of the hit.
class Computer {
  constructor() {
    this.targets = [];
  }

  choose(board, random) {
    while (this.targets.length > 0) {
      const target = this.targets.pop();
      if (!board.shot[target.y][target.x]) return target;
    }
    const unshot = [];
    for (let y = 0; y < BOARD_SIZE; y++) {
      for (let x = 0; x < BOARD_SIZE; x++) {
        if (!board.shot[y][x]) unshot.push({ x, y });
      }
    }
    return unshot[randomInt(random, unshot.length)];
  }

  report(board, x, y, result) {
    if (result === SUNK) {
      this.targets = [];
    } else if (result === HIT) {
      for (const [nx, ny] of [[x + 1, y], [x - 1, y], [x, y + 1], [x, y - 1]]) {
        if (inBounds(nx, ny) && !board.shot[ny][nx]) this.targets.push({ x: nx, y: ny });
      }
    }
  }
}

if (typeof module !== 'undefined') {
  module.exports = {
    BOARD_SIZE, FLEET, MISS, HIT, SUNK, REPEAT, INVALID,
    Board, Computer, shipCells, randomInt,
  };
}
