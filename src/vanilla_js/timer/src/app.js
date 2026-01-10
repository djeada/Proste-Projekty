const display = document.getElementById('display');
const startBtn = document.getElementById('start');
const stopBtn = document.getElementById('stop');
const resetBtn = document.getElementById('reset');
const lapBtn = document.getElementById('lap');
const lapList = document.getElementById('lap-list');

let startTime = 0;
let elapsedTime = 0;
let timerInterval = null;
let running = false;
let lapCount = 0;

function formatTime(ms) {
  const hours = Math.floor(ms / 3600000);
  const minutes = Math.floor((ms % 3600000) / 60000);
  const seconds = Math.floor((ms % 60000) / 1000);
  const milliseconds = ms % 1000;
  
  return `${String(hours).padStart(2, '0')}:${String(minutes).padStart(2, '0')}:${String(seconds).padStart(2, '0')}.${String(milliseconds).padStart(3, '0')}`;
}

function updateDisplay() {
  const currentTime = Date.now();
  elapsedTime = currentTime - startTime;
  display.textContent = formatTime(elapsedTime);
}

function start() {
  if (running) return;
  
  running = true;
  startTime = Date.now() - elapsedTime;
  timerInterval = setInterval(updateDisplay, 10);
  
  display.classList.add('running');
  startBtn.disabled = true;
  stopBtn.disabled = false;
  lapBtn.disabled = false;
}

function stop() {
  if (!running) return;
  
  running = false;
  clearInterval(timerInterval);
  
  display.classList.remove('running');
  startBtn.disabled = false;
  stopBtn.disabled = true;
  lapBtn.disabled = true;
}

function reset() {
  stop();
  elapsedTime = 0;
  lapCount = 0;
  display.textContent = formatTime(0);
  lapList.innerHTML = '';
}

function lap() {
  if (!running) return;
  
  lapCount++;
  const lapItem = document.createElement('li');
  lapItem.innerHTML = `<span>Lap ${lapCount}</span><span>${formatTime(elapsedTime)}</span>`;
  lapList.insertBefore(lapItem, lapList.firstChild);
}

startBtn.addEventListener('click', start);
stopBtn.addEventListener('click', stop);
resetBtn.addEventListener('click', reset);
lapBtn.addEventListener('click', lap);

document.addEventListener('keydown', (e) => {
  switch(e.key.toLowerCase()) {
    case ' ':
      e.preventDefault();
      if (running) stop();
      else start();
      break;
    case 'r':
      reset();
      break;
    case 'l':
      if (running) lap();
      break;
  }
});
