// Page logic: draws the display, handles buttons and keys, and downloads the laps file.
const display = document.getElementById('display');
const message = document.getElementById('message');
const startButton = document.getElementById('start');
const lapButton = document.getElementById('lap');
const resetButton = document.getElementById('reset');
const saveButton = document.getElementById('save');
const countdownBox = document.getElementById('countdown');
const minutesInput = document.getElementById('minutes');
const secondsInput = document.getElementById('seconds');
const settings = document.getElementById('countdown-settings');
const lapList = document.getElementById('laps');

let timer = new Timer();
let started = false;
let alerted = false;

// performance.now() never jumps backwards, unlike Date.now().
const now = () => Math.floor(performance.now());

function limitFromInputs() {
  if (!countdownBox.checked) return 0;
  try {
    return countdownMs(Number(minutesInput.value), Number(secondsInput.value));
  } catch (error) {
    message.textContent = 'Seconds must be 0 to 59.';
    return 0;
  }
}

function refresh(time = now()) {
  display.textContent = formatTime(timer.display(time));
  display.classList.toggle('running', timer.running);
  display.classList.toggle('finished', timer.finished(time));
  startButton.textContent = timer.running ? 'Stop' : 'Start';
  lapButton.disabled = !timer.running;
  saveButton.disabled = timer.laps.length === 0;
}

function configChanged() {
  if (started) return;
  timer = new Timer(limitFromInputs());
  refresh();
}

function toggle() {
  const time = now();
  if (timer.running) {
    timer.stop(time);
  } else {
    started = true;
    timer.start(time);
    settings.disabled = true;
  }
  refresh(time);
}

function lap() {
  const recorded = timer.lap(now());
  if (!recorded) return;
  const item = document.createElement('li');
  item.textContent = formatLap(timer.laps.length, recorded);
  lapList.appendChild(item);
  refresh();
}

function reset() {
  started = false;
  alerted = false;
  timer.reset();
  lapList.replaceChildren();
  message.textContent = '';
  settings.disabled = false;
  refresh();
}

function saveLaps() {
  const blob = new Blob([lapsText(timer.laps)], { type: 'text/plain' });
  const url = URL.createObjectURL(blob);
  const link = document.createElement('a');
  link.href = url;
  link.download = 'laps.txt';
  link.click();
  setTimeout(() => URL.revokeObjectURL(url), 1000);
}

function tick() {
  const time = now();
  timer.tick(time);
  if (timer.finished(time) && !alerted) {
    alerted = true;
    message.textContent = 'Time is up!';
  }
  refresh(time);
}

startButton.addEventListener('click', toggle);
lapButton.addEventListener('click', lap);
resetButton.addEventListener('click', reset);
saveButton.addEventListener('click', saveLaps);
countdownBox.addEventListener('change', configChanged);
minutesInput.addEventListener('change', configChanged);
secondsInput.addEventListener('change', configChanged);

document.addEventListener('keydown', (event) => {
  if (event.repeat || event.target.tagName === 'INPUT') return;
  switch (event.key.toLowerCase()) {
    case 's': toggle(); break;
    case 'l': lap(); break;
    case 'r': reset(); break;
  }
});

refresh();
setInterval(tick, 20);
