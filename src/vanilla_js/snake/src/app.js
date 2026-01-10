const canvas = document.getElementById('canvas');
const ctx = canvas.getContext('2d');
const scoreElement = document.getElementById('score');
const messageElement = document.getElementById('message');
const newGameBtn = document.getElementById('new-game');

const size = 20;
const speed = 100;

let snake;
let food;
let gameInterval;
let gameOver = false;

function setup() {
  snake = new Snake();
  spawnFood();
  gameOver = false;
  scoreElement.textContent = '0';
  messageElement.textContent = '';
  messageElement.className = 'message';
}

function spawnFood() {
  const cols = Math.floor(canvas.width / size);
  const rows = Math.floor(canvas.height / size);
  food = [
    Math.floor(Math.random() * cols) * size,
    Math.floor(Math.random() * rows) * size
  ];
}

function draw() {
  if (gameOver) return;
  
  drawBackground();
  drawFood();
  snake.runSnake();
}

function drawBackground() {
  ctx.fillStyle = '#2d2d2d';
  ctx.fillRect(0, 0, canvas.width, canvas.height);
  
  // Draw grid
  ctx.strokeStyle = '#3d3d3d';
  ctx.lineWidth = 0.5;
  for (let x = 0; x <= canvas.width; x += size) {
    ctx.beginPath();
    ctx.moveTo(x, 0);
    ctx.lineTo(x, canvas.height);
    ctx.stroke();
  }
  for (let y = 0; y <= canvas.height; y += size) {
    ctx.beginPath();
    ctx.moveTo(0, y);
    ctx.lineTo(canvas.width, y);
    ctx.stroke();
  }
}

function drawFood() {
  ctx.fillStyle = '#f44336';
  ctx.beginPath();
  ctx.arc(food[0] + size / 2, food[1] + size / 2, size / 2 - 2, 0, 2 * Math.PI);
  ctx.fill();
}

function Snake() {
  this.xHead = size * 5;
  this.yHead = size * 5;
  this.xDir = size;
  this.yDir = 0;
  this.score = 0;
  this.positions = [[this.xHead, this.yHead]];
  
  this.runSnake = function() {
    this.checkFood();
    this.update();
    this.show();
    this.checkCollision();
  };
  
  this.dir = function(x, y) {
    // Prevent 180-degree turns
    if (this.positions.length > 1) {
      if (x !== 0 && this.xDir !== 0) return;
      if (y !== 0 && this.yDir !== 0) return;
    }
    this.xDir = x;
    this.yDir = y;
  };
  
  this.checkFood = function() {
    if (this.xHead === food[0] && this.yHead === food[1]) {
      spawnFood();
      this.positions.push([...this.positions[this.positions.length - 1]]);
      this.score++;
      scoreElement.textContent = this.score;
    }
  };
  
  this.update = function() {
    this.xHead += this.xDir;
    this.yHead += this.yDir;
    this.updatePositions();
  };
  
  this.show = function() {
    // Draw body
    ctx.fillStyle = 'rgb(0, 157, 193)';
    for (let i = 1; i < this.positions.length; i++) {
      ctx.fillRect(this.positions[i][0] + 1, this.positions[i][1] + 1, size - 2, size - 2);
    }
    
    // Draw head
    ctx.fillStyle = '#00b8d4';
    ctx.fillRect(this.xHead + 1, this.yHead + 1, size - 2, size - 2);
    
    // Draw eyes
    ctx.fillStyle = '#1c1c1c';
    const eyeSize = 3;
    if (this.xDir > 0) {
      ctx.beginPath();
      ctx.arc(this.xHead + size - 6, this.yHead + 6, eyeSize, 0, 2 * Math.PI);
      ctx.fill();
      ctx.beginPath();
      ctx.arc(this.xHead + size - 6, this.yHead + size - 6, eyeSize, 0, 2 * Math.PI);
      ctx.fill();
    } else if (this.xDir < 0) {
      ctx.beginPath();
      ctx.arc(this.xHead + 6, this.yHead + 6, eyeSize, 0, 2 * Math.PI);
      ctx.fill();
      ctx.beginPath();
      ctx.arc(this.xHead + 6, this.yHead + size - 6, eyeSize, 0, 2 * Math.PI);
      ctx.fill();
    } else if (this.yDir > 0) {
      ctx.beginPath();
      ctx.arc(this.xHead + 6, this.yHead + size - 6, eyeSize, 0, 2 * Math.PI);
      ctx.fill();
      ctx.beginPath();
      ctx.arc(this.xHead + size - 6, this.yHead + size - 6, eyeSize, 0, 2 * Math.PI);
      ctx.fill();
    } else {
      ctx.beginPath();
      ctx.arc(this.xHead + 6, this.yHead + 6, eyeSize, 0, 2 * Math.PI);
      ctx.fill();
      ctx.beginPath();
      ctx.arc(this.xHead + size - 6, this.yHead + 6, eyeSize, 0, 2 * Math.PI);
      ctx.fill();
    }
  };
  
  this.updatePositions = function() {
    for (let i = this.positions.length - 1; i > 0; i--) {
      this.positions[i][0] = this.positions[i - 1][0];
      this.positions[i][1] = this.positions[i - 1][1];
    }
    this.positions[0] = [this.xHead, this.yHead];
  };
  
  this.checkCollision = function() {
    // Wall collision
    if (this.xHead < 0 || this.xHead >= canvas.width ||
        this.yHead < 0 || this.yHead >= canvas.height) {
      end();
      return;
    }
    
    // Self collision
    for (let i = 1; i < this.positions.length; i++) {
      if (this.positions[i][0] === this.xHead && this.positions[i][1] === this.yHead) {
        end();
        return;
      }
    }
  };
}

function end() {
  gameOver = true;
  clearInterval(gameInterval);
  messageElement.textContent = `Game Over! Final Score: ${snake.score}`;
  messageElement.className = 'message lose';
}

function startGame() {
  if (gameInterval) {
    clearInterval(gameInterval);
  }
  setup();
  gameInterval = setInterval(draw, speed);
}

document.addEventListener('keydown', function(event) {
  if (gameOver) return;
  
  switch (event.keyCode) {
    case 37: // Left
      snake.dir(-size, 0);
      break;
    case 38: // Up
      snake.dir(0, -size);
      break;
    case 39: // Right
      snake.dir(size, 0);
      break;
    case 40: // Down
      snake.dir(0, size);
      break;
  }
});

newGameBtn.addEventListener('click', startGame);

// Start the game
startGame();