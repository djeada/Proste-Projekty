// The rules of Zombie Apocalypse: no DOM or drawing here.

const WORLD_WIDTH = 60;
const WORLD_HEIGHT = 20;
const MAX_ZOMBIES = 64;
const MAX_BULLETS = 64;
const MAX_PICKUPS = 2;

const PLAYER_RADIUS = 0.5;
const MAX_HEALTH = 100;
const PLAYER_SPEED = 7.0;
const TOUCH_DISTANCE = 1.0; // player and zombies have radius 0.5
const ZOMBIE_DAMAGE = 10;
const KILL_SCORE = 10;
const BULLET_SPEED = 18.0;
const BULLET_HIT_DISTANCE = 0.6;
const SHOT_COOLDOWN = 0.25;
const PICKUP_HEAL = 25;
const PICKUP_INTERVAL = 12.0;
const WAVE_BREAK = 3.0;

function waveSize(wave) {
  return 5 + 3 * (wave - 1);
}

function zombieSpeed(wave) {
  return Math.min(2.0 + 0.4 * (wave - 1), 5.0);
}

function spawnInterval(wave) {
  return Math.max(0.5, 1.5 - 0.15 * (wave - 1));
}

// A number in [0, 1). A 32-bit linear congruential generator: the same seed gives the same game.
function nextRandom(state) {
  state.rng = (Math.imul(state.rng, 1664525) + 1013904223) >>> 0;
  return state.rng / 4294967296;
}

function newGame(seed) {
  return {
    player: { x: WORLD_WIDTH / 2, y: WORLD_HEIGHT / 2 },
    facing: { x: 1, y: 0 }, // direction of the last movement, used when the aim is zero
    health: MAX_HEALTH,
    wave: 1,
    toSpawn: waveSize(1), // zombies of this wave that are not on the field yet
    score: 0,
    gameOver: false,
    zombies: [], // {x, y}
    bullets: [], // {pos: {x, y}, vel: {x, y}}
    pickups: [], // {x, y}
    spawnTimer: 0,
    waveDelay: 0, // seconds left before the next wave; 0 while a wave is running
    pickupTimer: PICKUP_INTERVAL,
    shotCooldown: 0,
    rng: seed >>> 0,
  };
}

function add(a, b) {
  return { x: a.x + b.x, y: a.y + b.y };
}

function scale(v, k) {
  return { x: v.x * k, y: v.y * k };
}

function normalize(v) {
  const length = Math.hypot(v.x, v.y);
  return length < 1e-9 ? { x: 0, y: 0 } : scale(v, 1 / length);
}

function distance(a, b) {
  return Math.hypot(a.x - b.x, a.y - b.y);
}

function clamp(value, low, high) {
  return Math.max(low, Math.min(high, value));
}

// input: {moveX, moveY, aimX, aimY, fire}. Move and aim are directions; (0, 0) means none.
function update(state, input, dt) {
  if (state.gameOver) {
    return;
  }
  movePlayer(state, input, dt);
  tryFire(state, input, dt);
  moveBullets(state, dt);
  spawnZombies(state, dt);
  moveZombies(state, dt);
  resolveBulletHits(state);
  resolveZombieTouches(state);
  if (state.gameOver) {
    return;
  }
  resolvePickups(state);
  spawnPickups(state, dt);
  updateWaves(state, dt);
}

function movePlayer(state, input, dt) {
  const direction = normalize({ x: input.moveX, y: input.moveY });
  if (direction.x === 0 && direction.y === 0) {
    return;
  }
  state.facing = direction;
  state.player = {
    x: clamp(state.player.x + direction.x * PLAYER_SPEED * dt, PLAYER_RADIUS, WORLD_WIDTH - PLAYER_RADIUS),
    y: clamp(state.player.y + direction.y * PLAYER_SPEED * dt, PLAYER_RADIUS, WORLD_HEIGHT - PLAYER_RADIUS),
  };
}

function tryFire(state, input, dt) {
  state.shotCooldown = Math.max(0, state.shotCooldown - dt);
  if (!input.fire || state.shotCooldown > 0 || state.bullets.length >= MAX_BULLETS) {
    return;
  }
  let direction = normalize({ x: input.aimX, y: input.aimY });
  if (direction.x === 0 && direction.y === 0) {
    direction = state.facing;
  }
  state.bullets.push({ pos: { ...state.player }, vel: scale(direction, BULLET_SPEED) });
  state.shotCooldown = SHOT_COOLDOWN;
}

function moveBullets(state, dt) {
  state.bullets = state.bullets
    .map((bullet) => ({ pos: add(bullet.pos, scale(bullet.vel, dt)), vel: bullet.vel }))
    .filter(({ pos }) => pos.x >= 0 && pos.x <= WORLD_WIDTH && pos.y >= 0 && pos.y <= WORLD_HEIGHT);
}

function spawnZombies(state, dt) {
  if (state.toSpawn === 0 || state.waveDelay > 0) {
    return;
  }
  state.spawnTimer -= dt;
  if (state.spawnTimer > 0) {
    return;
  }
  spawnZombie(state);
  state.toSpawn -= 1;
  state.spawnTimer = spawnInterval(state.wave);
}

// A zombie appears on a random point of a random edge of the world.
function spawnZombie(state) {
  const side = nextRandom(state) * 4;
  const t = nextRandom(state);
  let pos;
  if (side < 1) {
    pos = { x: 0, y: t * WORLD_HEIGHT };
  } else if (side < 2) {
    pos = { x: WORLD_WIDTH, y: t * WORLD_HEIGHT };
  } else if (side < 3) {
    pos = { x: t * WORLD_WIDTH, y: 0 };
  } else {
    pos = { x: t * WORLD_WIDTH, y: WORLD_HEIGHT };
  }
  if (state.zombies.length < MAX_ZOMBIES) {
    state.zombies.push(pos);
  }
}

function moveZombies(state, dt) {
  const step = zombieSpeed(state.wave) * dt;
  state.zombies = state.zombies.map((zombie) =>
    add(zombie, scale(normalize({ x: state.player.x - zombie.x, y: state.player.y - zombie.y }), step)),
  );
}

function resolveBulletHits(state) {
  state.bullets = state.bullets.filter((bullet) => {
    const index = state.zombies.findIndex((z) => distance(z, bullet.pos) < BULLET_HIT_DISTANCE);
    if (index < 0) {
      return true;
    }
    state.zombies.splice(index, 1);
    state.score += KILL_SCORE;
    return false;
  });
}

function resolveZombieTouches(state) {
  state.zombies = state.zombies.filter((zombie) => {
    if (distance(zombie, state.player) >= TOUCH_DISTANCE) {
      return true;
    }
    state.health -= ZOMBIE_DAMAGE;
    return false;
  });
  if (state.health <= 0) {
    state.health = 0;
    state.gameOver = true;
  }
}

function resolvePickups(state) {
  state.pickups = state.pickups.filter((pickup) => {
    if (distance(pickup, state.player) >= TOUCH_DISTANCE) {
      return true;
    }
    state.health = Math.min(MAX_HEALTH, state.health + PICKUP_HEAL);
    return false;
  });
}

function spawnPickups(state, dt) {
  state.pickupTimer -= dt;
  if (state.pickupTimer > 0) {
    return;
  }
  state.pickupTimer = PICKUP_INTERVAL;
  if (state.pickups.length < MAX_PICKUPS) {
    const x = nextRandom(state) * WORLD_WIDTH;
    const y = nextRandom(state) * WORLD_HEIGHT;
    state.pickups.push({ x, y });
  }
}

function updateWaves(state, dt) {
  if (state.waveDelay > 0) {
    state.waveDelay -= dt;
    if (state.waveDelay <= 0) {
      state.waveDelay = 0;
      state.wave += 1;
      state.toSpawn = waveSize(state.wave);
      state.spawnTimer = 0;
    }
  } else if (state.toSpawn === 0 && state.zombies.length === 0) {
    state.waveDelay = WAVE_BREAK;
  }
}

if (typeof module !== 'undefined') {
  module.exports = {
    WORLD_WIDTH,
    WORLD_HEIGHT,
    MAX_HEALTH,
    KILL_SCORE,
    ZOMBIE_DAMAGE,
    BULLET_HIT_DISTANCE,
    newGame,
    update,
    nextRandom,
    waveSize,
    zombieSpeed,
    distance,
  };
}
