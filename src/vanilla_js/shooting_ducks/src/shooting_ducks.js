// Rules of Shooting Ducks: spawning, movement, hits, escapes and waves. No DOM here.
const FIELD_WIDTH = 100;
const FIELD_HEIGHT = 40;
const DUCK_HALF_WIDTH = 5;
const DUCK_HALF_HEIGHT = 3;
const START_LIVES = 3;
const POINTS_PER_HIT = 10;
const WAVE_BREAK_SECONDS = 2;
const MAX_DUCKS = 64;
const BOB_AMPLITUDE = 1.5;
const BOB_SPEED = 2;

// Linear congruential generator: gives the same numbers as the C and Python versions.
function createRng(seed) {
  return { state: (seed >>> 0) || 1 };
}

function nextRandom(rng) {
  rng.state = (Math.imul(rng.state, 1664525) + 1013904223) >>> 0;
  return rng.state / 4294967296;
}

function ducksInWave(wave) {
  return 3 + 2 * wave;
}

function spawnInterval(wave) {
  return Math.max(0.5, 2 - 0.15 * wave);
}

function hasEscaped(duck) {
  return (duck.speed > 0 && duck.x - DUCK_HALF_WIDTH > FIELD_WIDTH) ||
    (duck.speed < 0 && duck.x + DUCK_HALF_WIDTH < 0);
}

function isHitAt(duck, x, y) {
  return Math.abs(x - duck.x) <= DUCK_HALF_WIDTH && Math.abs(y - duck.y) <= DUCK_HALF_HEIGHT;
}

function createGame(seed) {
  const game = {
    rng: createRng(seed),
    ducks: [],
    wave: 0,
    score: 0,
    lives: START_LIVES,
    ducksToSpawn: 0,
    spawnTimer: 0,
    breakTimer: 0, // above zero while the "wave cleared" pause runs
    gameOver: false,
  };
  startWave(game, 1);
  return game;
}

function startWave(game, wave) {
  game.wave = wave;
  game.ducksToSpawn = ducksInWave(wave);
  game.spawnTimer = 0;
  game.breakTimer = 0;
}

function spawnDuck(game) {
  const fliesRight = nextRandom(game.rng) < 0.5;
  const speed = (5 + 1.5 * game.wave) * (0.8 + 0.4 * nextRandom(game.rng));
  const baseY = 5 + 22 * nextRandom(game.rng);
  const phase = 2 * Math.PI * nextRandom(game.rng);
  game.ducks.push({
    x: fliesRight ? -DUCK_HALF_WIDTH : FIELD_WIDTH + DUCK_HALF_WIDTH,
    y: baseY,
    baseY,
    speed: fliesRight ? speed : -speed,
    age: 0,
    phase,
  });
}

function updateGame(game, dt) {
  if (game.gameOver) return;

  if (game.breakTimer > 0) {
    game.breakTimer -= dt;
    if (game.breakTimer <= 0) startWave(game, game.wave + 1);
    return;
  }

  for (const duck of game.ducks) {
    duck.x += duck.speed * dt;
    duck.age += dt;
    duck.y = duck.baseY + BOB_AMPLITUDE * Math.sin(BOB_SPEED * duck.age + duck.phase);
  }

  const escaped = game.ducks.filter(hasEscaped).length;
  game.ducks = game.ducks.filter((duck) => !hasEscaped(duck));
  game.lives -= escaped;
  if (game.lives <= 0) {
    game.lives = 0;
    game.gameOver = true;
    return;
  }

  if (game.ducksToSpawn > 0) {
    game.spawnTimer -= dt;
    if (game.spawnTimer <= 0 && game.ducks.length < MAX_DUCKS) {
      spawnDuck(game);
      game.ducksToSpawn -= 1;
      game.spawnTimer = spawnInterval(game.wave);
    }
  }

  if (game.ducksToSpawn === 0 && game.ducks.length === 0) {
    game.breakTimer = WAVE_BREAK_SECONDS;
  }
}

// Removes the first duck at (x, y) and scores it. Returns true on a hit.
function shootAt(game, x, y) {
  if (game.gameOver) return false;
  const index = game.ducks.findIndex((duck) => isHitAt(duck, x, y));
  if (index === -1) return false;
  game.ducks.splice(index, 1);
  game.score += POINTS_PER_HIT;
  return true;
}

if (typeof module !== 'undefined') {
  module.exports = {
    FIELD_WIDTH, FIELD_HEIGHT, DUCK_HALF_WIDTH, DUCK_HALF_HEIGHT, START_LIVES, WAVE_BREAK_SECONDS,
    createRng, nextRandom, ducksInWave, createGame, updateGame, shootAt,
  };
}
