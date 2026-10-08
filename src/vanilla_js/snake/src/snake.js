// The rules of Snake: no DOM here.
const WIDTH = 20;
const HEIGHT = 15;

const UP = { x: 0, y: -1 };
const DOWN = { x: 0, y: 1 };
const LEFT = { x: -1, y: 0 };
const RIGHT = { x: 1, y: 0 };

function isOpposite(a, b) {
  return a.x === -b.x && a.y === -b.y;
}

class SnakeGame {
  // randomBelow(n) returns a random integer in [0, n); tests pass a fake one.
  constructor(randomBelow = (n) => Math.floor(Math.random() * n)) {
    this.randomBelow = randomBelow;
    this.reset();
  }

  reset() {
    this.body = [{ x: Math.floor(WIDTH / 2), y: Math.floor(HEIGHT / 2) }]; // body[0] is the head
    this.direction = RIGHT; // the direction of the last step
    this.nextDirection = RIGHT; // the direction of the next step
    this.score = 0;
    this.gameOver = false;
    this.food = null;
    this.placeFood();
  }

  // A turn that would reverse the last step is ignored, so the snake never runs into its neck.
  turn(direction) {
    if (!isOpposite(direction, this.direction)) {
      this.nextDirection = direction;
    }
  }

  step() {
    if (this.gameOver) {
      return;
    }
    this.direction = this.nextDirection;
    const head = { x: this.body[0].x + this.direction.x, y: this.body[0].y + this.direction.y };

    if (!this.isInside(head) || this.isOnSnake(head)) {
      this.gameOver = true;
      return;
    }

    const eats = this.food !== null && head.x === this.food.x && head.y === this.food.y;
    this.body.unshift(head);
    if (eats) {
      this.score += 10;
      this.placeFood();
    } else {
      this.body.pop(); // without eating, the tail leaves its cell
    }
  }

  // Puts the food on a random free cell.
  placeFood() {
    const free = [];
    for (let y = 0; y < HEIGHT; y++) {
      for (let x = 0; x < WIDTH; x++) {
        if (!this.isOnSnake({ x, y })) {
          free.push({ x, y });
        }
      }
    }
    if (free.length === 0) {
      this.gameOver = true; // the board is full
      this.food = null;
      return;
    }
    this.food = free[this.randomBelow(free.length)];
  }

  isInside(cell) {
    return cell.x >= 0 && cell.x < WIDTH && cell.y >= 0 && cell.y < HEIGHT;
  }

  isOnSnake(cell) {
    return this.body.some((part) => part.x === cell.x && part.y === cell.y);
  }

  // The time between two steps: the snake gets faster as it grows, but not faster than 60 ms.
  delayMs() {
    return Math.max(60, 150 - 5 * (this.body.length - 1));
  }
}

if (typeof module !== 'undefined') module.exports = { WIDTH, HEIGHT, UP, DOWN, LEFT, RIGHT, SnakeGame };
