document.addEventListener('keydown', keyDownHandler, false);

const canvas = document.getElementById('canvas');
const ctx = canvas.getContext('2d')
const size = 30;
const speed = 70;

var snake;
var food = [Math.random()*(canvas.width-2*size)+size, Math.random()*(canvas.height-2*size)+size];

function setup() {
	snake = new Snake();
}

function draw() {
	drawBackground();
	drawFood();
	snake.runSnake();
	setTimeout(draw, speed);
}

function drawBackground(){
	ctx.fillStyle = 'rgb(255,255,255)';
	ctx.fillRect(0, 0, canvas.width, canvas.height);
}

function drawFood() {
	ctx.fillStyle = 'rgb(255,0,100)';
	ctx.fillRect(food[0], food[1], size, size);
}

function Snake() {
	this.xHead = 0;
	this.yHead = 0;
	this.xDir = size;
	this.yDir = 0;
	this.score = 0;
	this.positions = [[this.xHead, this.yHead]];
	
	this.runSnake = function(){
		this.checkFood();
		this.update();
		this.show();
		this.checkBorder();
	}
	
	this.dir = function(x, y) {
		this.xDir = x;
		this.yDir = y;
	}
	
	this.checkFood = function(){
		var centerXa = this.xHead + size/2;
		var centerYa = this.yHead + size/2;
		var centerXb  = food[0] + size/2;
		var centerYb = food[1] + size/2;
		if(Math.abs(centerXa - centerXb) < size && Math.abs(centerYa - centerYb) < size){
			food = [Math.random()*(canvas.width-2*size)+size, Math.random()*(canvas.height-2*size)+size];
			this.positions.push([this.xHead+size*this.xDir, this.yHead+size*this.yDir]);
			this.score++;
		}
	}
	
	this.update = function() {
		this.xHead += this.xDir;
		this.yHead += this.yDir;
		this.updatePositions();
	}
	
	this.show = function() {
		//draw head 
		ctx.fillStyle = 'rgb(0,0,0)';
		ctx.fillRect(this.xHead, this.yHead, size, size);
		drawCircle(this.xHead + 2*size/3, this.yHead + size/3);
		
		//draw rest of the snake
		ctx.fillStyle = 'rgb(0,0,0)';
		for(var i = 1; i < this.positions.length; i++){
			ctx.fillRect(this.positions[i][0], this.positions[i][1], size, size);
		}		
	}
	this.updatePositions = function(){
		for(var i = this.positions.length - 3; i >= 0 ; i--){
			this.positions[i+2][0] = this.positions[i][0];
			this.positions[i+2][1] = this.positions[i][1];
		}
		this.positions[0] = [this.xHead, this.yHead];
	}
	
	this.checkBorder = function(){
		if(this.xHead < 0){
			end();
		}
		if(this.xHead > canvas.width-size){
			end();
		}
		if(this.yHead < 0) {
			end();
		}
		if(this.yHead > canvas.height-size){
			end();
		}
		for(var i = 1; i < this.positions.length; i++){
			if(this.positions[i][0] == this.xHead && this.positions[i][1] == this.yHead){
				end();
			}
		}
	}
}

function end(){
	ctx.fillStyle = 'rgb(0,0,0)';
	ctx.fillRect(0, 0, canvas.width, canvas.height);
	ctx.font = "50px serif";
	ctx.fillStyle = 'rgb(255,255,255)';
	ctx.fillText("Game Over", 0.7*canvas.width/2, 1.1*canvas.height/2);
	ctx.font = "30px serif";
	ctx.fillText("Your score: " + snake.score, 0.8*canvas.width/2, 0.9*canvas.height/2);
	setTimeout(end, speed);
}

function drawCircle(x, y){
	ctx.beginPath();
    ctx.arc(x, y, size/5, 0, 2 * Math.PI, false);
    ctx.fillStyle = 'green';
    ctx.fill();
    ctx.lineWidth = 1;
    ctx.strokeStyle = 'white';
    ctx.stroke();

}

function keyDownHandler(event) {
  var x = event.keyCode;
  //left arrow
  if (x == 37) {
	snake.dir(-size, 0);
  }
  //up arrow
  else if (x == 38) {
	snake.dir(0, -size);
  }
  //right arrow
  else if (x == 39) {
	snake.dir(size, 0);
  }
  //down arrow
  else if (x == 40) {
	snake.dir(0, size);
  }
}

//main
setup();
draw();