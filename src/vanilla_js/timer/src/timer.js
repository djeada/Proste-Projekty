// Stopwatch and countdown rules. Time is given in milliseconds by the caller.
const MAX_LAPS = 100;

class Timer {
  constructor(limitMs = 0) {
    this.limitMs = limitMs; // countdown length, 0 for a stopwatch
    this.laps = [];
    this.running = false;
    this.accumulatedMs = 0;
    this.startedAtMs = 0;
  }

  rawElapsed(now) {
    return this.running ? this.accumulatedMs + now - this.startedAtMs : this.accumulatedMs;
  }

  elapsed(now) {
    const elapsed = this.rawElapsed(now);
    return this.limitMs > 0 ? Math.min(elapsed, this.limitMs) : elapsed;
  }

  // Elapsed time for a stopwatch, remaining time for a countdown.
  display(now) {
    return this.limitMs > 0 ? this.limitMs - this.elapsed(now) : this.elapsed(now);
  }

  finished(now) {
    return this.limitMs > 0 && this.elapsed(now) >= this.limitMs;
  }

  start(now) {
    if (this.running || this.finished(now)) return;
    this.running = true;
    this.startedAtMs = now;
  }

  stop(now) {
    if (!this.running) return;
    this.accumulatedMs = this.rawElapsed(now);
    this.running = false;
  }

  // Stops a countdown that has reached zero. Call it regularly.
  tick(now) {
    if (this.running && this.finished(now)) this.stop(now);
  }

  lap(now) {
    if (!this.running || this.laps.length >= MAX_LAPS) return null;
    const total = this.elapsed(now);
    const previous = this.laps.length > 0 ? this.laps[this.laps.length - 1].total : 0;
    const lap = { split: total - previous, total };
    this.laps.push(lap);
    return lap;
  }

  reset() {
    this.accumulatedMs = 0;
    this.startedAtMs = 0;
    this.running = false;
    this.laps = [];
  }
}

function countdownMs(minutes, seconds) {
  if (!Number.isInteger(minutes) || !Number.isInteger(seconds) || minutes < 0 || seconds < 0 || seconds > 59) {
    throw new RangeError('minutes must be 0 or more and seconds 0 to 59');
  }
  return (minutes * 60 + seconds) * 1000;
}

// Mm:ss.mmm, with hours in front when needed: 1:02:03.004.
function formatTime(ms) {
  const hours = Math.floor(ms / 3600000);
  const minutes = Math.floor(ms / 60000) % 60;
  const seconds = Math.floor(ms / 1000) % 60;
  const millis = Math.floor(ms) % 1000;
  const clock = `${String(minutes).padStart(2, '0')}:${String(seconds).padStart(2, '0')}.${String(millis).padStart(3, '0')}`;
  return hours > 0 ? `${hours}:${clock}` : clock;
}

function formatLap(number, lap) {
  return `Lap ${number}: split ${formatTime(lap.split)}  total ${formatTime(lap.total)}`;
}

function lapsText(laps) {
  return laps.map((lap, index) => `${formatLap(index + 1, lap)}\n`).join('');
}

if (typeof module !== 'undefined') module.exports = { Timer, countdownMs, formatTime, formatLap, lapsText };
