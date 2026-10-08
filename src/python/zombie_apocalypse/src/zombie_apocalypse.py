"""The rules of Zombie Apocalypse: no input, output or drawing here."""

import math
from dataclasses import dataclass, field
from typing import List, Tuple

Point = Tuple[float, float]

WORLD_WIDTH = 60
WORLD_HEIGHT = 20
MAX_ZOMBIES = 64
MAX_BULLETS = 64
MAX_PICKUPS = 2

PLAYER_RADIUS = 0.5
MAX_HEALTH = 100
PLAYER_SPEED = 7.0
TOUCH_DISTANCE = 1.0  # player and zombies have radius 0.5
ZOMBIE_DAMAGE = 10
KILL_SCORE = 10
BULLET_SPEED = 18.0
BULLET_HIT_DISTANCE = 0.6
SHOT_COOLDOWN = 0.25
PICKUP_HEAL = 25
PICKUP_INTERVAL = 12.0
WAVE_BREAK = 3.0


@dataclass
class Bullet:
    pos: Point
    vel: Point


@dataclass
class Input:
    """What the player asks for in one step. Move and aim are directions; (0, 0) means none."""

    move_x: float = 0.0
    move_y: float = 0.0
    aim_x: float = 0.0
    aim_y: float = 0.0
    fire: bool = False


@dataclass
class Game:
    player: Point
    facing: Point  # direction of the last movement, used when the aim is (0, 0)
    health: int
    wave: int
    to_spawn: int  # zombies of this wave that are not on the field yet
    rng: int
    zombies: List[Point] = field(default_factory=list)
    bullets: List[Bullet] = field(default_factory=list)
    pickups: List[Point] = field(default_factory=list)
    score: int = 0
    game_over: bool = False
    spawn_timer: float = 0.0
    wave_delay: float = 0.0  # seconds left before the next wave; 0 while a wave is running
    pickup_timer: float = PICKUP_INTERVAL
    shot_cooldown: float = 0.0


def new_game(seed: int) -> Game:
    return Game(
        player=(WORLD_WIDTH / 2, WORLD_HEIGHT / 2),
        facing=(1.0, 0.0),
        health=MAX_HEALTH,
        wave=1,
        to_spawn=wave_size(1),
        rng=seed & 0xFFFFFFFF,
    )


def wave_size(wave: int) -> int:
    return 5 + 3 * (wave - 1)


def zombie_speed(wave: int) -> float:
    return min(2.0 + 0.4 * (wave - 1), 5.0)


def _spawn_interval(wave: int) -> float:
    return max(0.5, 1.5 - 0.15 * (wave - 1))


def next_random(game: Game) -> float:
    """A number in [0, 1). A 32-bit linear congruential generator: the same seed gives the same game."""
    game.rng = (game.rng * 1664525 + 1013904223) & 0xFFFFFFFF
    return game.rng / 2**32


def _add(a: Point, b: Point) -> Point:
    return (a[0] + b[0], a[1] + b[1])


def _sub(a: Point, b: Point) -> Point:
    return (a[0] - b[0], a[1] - b[1])


def _scale(v: Point, k: float) -> Point:
    return (v[0] * k, v[1] * k)


def _normalize(v: Point) -> Point:
    length = math.hypot(v[0], v[1])
    if length < 1e-9:
        return (0.0, 0.0)
    return _scale(v, 1.0 / length)


def _distance(a: Point, b: Point) -> float:
    return math.hypot(*_sub(a, b))


def _clamp(value: float, low: float, high: float) -> float:
    return max(low, min(high, value))


def update(game: Game, input: Input, dt: float) -> None:
    if game.game_over:
        return
    _move_player(game, input, dt)
    _try_fire(game, input, dt)
    _move_bullets(game, dt)
    _spawn_zombies(game, dt)
    _move_zombies(game, dt)
    _resolve_bullet_hits(game)
    _resolve_zombie_touches(game)
    if game.game_over:
        return
    _resolve_pickups(game)
    _spawn_pickups(game, dt)
    _update_waves(game, dt)


def _move_player(game: Game, input: Input, dt: float) -> None:
    direction = _normalize((input.move_x, input.move_y))
    if direction == (0.0, 0.0):
        return
    game.facing = direction
    x = _clamp(game.player[0] + direction[0] * PLAYER_SPEED * dt, PLAYER_RADIUS, WORLD_WIDTH - PLAYER_RADIUS)
    y = _clamp(game.player[1] + direction[1] * PLAYER_SPEED * dt, PLAYER_RADIUS, WORLD_HEIGHT - PLAYER_RADIUS)
    game.player = (x, y)


def _try_fire(game: Game, input: Input, dt: float) -> None:
    game.shot_cooldown = max(0.0, game.shot_cooldown - dt)
    if not input.fire or game.shot_cooldown > 0 or len(game.bullets) >= MAX_BULLETS:
        return
    direction = _normalize((input.aim_x, input.aim_y))
    if direction == (0.0, 0.0):
        direction = game.facing
    game.bullets.append(Bullet(pos=game.player, vel=_scale(direction, BULLET_SPEED)))
    game.shot_cooldown = SHOT_COOLDOWN


def _move_bullets(game: Game, dt: float) -> None:
    flying = []
    for bullet in game.bullets:
        pos = _add(bullet.pos, _scale(bullet.vel, dt))
        if 0 <= pos[0] <= WORLD_WIDTH and 0 <= pos[1] <= WORLD_HEIGHT:
            flying.append(Bullet(pos=pos, vel=bullet.vel))
    game.bullets = flying


def _spawn_zombies(game: Game, dt: float) -> None:
    if game.to_spawn == 0 or game.wave_delay > 0:
        return
    game.spawn_timer -= dt
    if game.spawn_timer > 0:
        return
    _spawn_zombie(game)
    game.to_spawn -= 1
    game.spawn_timer = _spawn_interval(game.wave)


def _spawn_zombie(game: Game) -> None:
    """A zombie appears on a random point of a random edge of the world."""
    side = next_random(game) * 4.0
    t = next_random(game)
    if side < 1:
        pos = (0.0, t * WORLD_HEIGHT)
    elif side < 2:
        pos = (float(WORLD_WIDTH), t * WORLD_HEIGHT)
    elif side < 3:
        pos = (t * WORLD_WIDTH, 0.0)
    else:
        pos = (t * WORLD_WIDTH, float(WORLD_HEIGHT))
    if len(game.zombies) < MAX_ZOMBIES:
        game.zombies.append(pos)


def _move_zombies(game: Game, dt: float) -> None:
    step = zombie_speed(game.wave) * dt
    game.zombies = [
        _add(z, _scale(_normalize(_sub(game.player, z)), step)) for z in game.zombies
    ]


def _resolve_bullet_hits(game: Game) -> None:
    remaining = []
    for bullet in game.bullets:
        target = next(
            (z for z in game.zombies if _distance(z, bullet.pos) < BULLET_HIT_DISTANCE), None
        )
        if target is None:
            remaining.append(bullet)
        else:
            game.zombies.remove(target)
            game.score += KILL_SCORE
    game.bullets = remaining


def _resolve_zombie_touches(game: Game) -> None:
    survivors = []
    for zombie in game.zombies:
        if _distance(zombie, game.player) < TOUCH_DISTANCE:
            game.health -= ZOMBIE_DAMAGE
        else:
            survivors.append(zombie)
    game.zombies = survivors
    if game.health <= 0:
        game.health = 0
        game.game_over = True


def _resolve_pickups(game: Game) -> None:
    remaining = []
    for pickup in game.pickups:
        if _distance(pickup, game.player) < TOUCH_DISTANCE:
            game.health = min(MAX_HEALTH, game.health + PICKUP_HEAL)
        else:
            remaining.append(pickup)
    game.pickups = remaining


def _spawn_pickups(game: Game, dt: float) -> None:
    game.pickup_timer -= dt
    if game.pickup_timer > 0:
        return
    game.pickup_timer = PICKUP_INTERVAL
    if len(game.pickups) < MAX_PICKUPS:
        x = next_random(game) * WORLD_WIDTH
        y = next_random(game) * WORLD_HEIGHT
        game.pickups.append((x, y))


def _update_waves(game: Game, dt: float) -> None:
    if game.wave_delay > 0:
        game.wave_delay -= dt
        if game.wave_delay <= 0:
            game.wave_delay = 0.0
            game.wave += 1
            game.to_spawn = wave_size(game.wave)
            game.spawn_timer = 0.0
    elif game.to_spawn == 0 and not game.zombies:
        game.wave_delay = WAVE_BREAK
