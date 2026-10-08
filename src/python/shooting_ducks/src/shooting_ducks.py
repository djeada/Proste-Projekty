"""Rules of Shooting Ducks: spawning, movement, hits, escapes and waves. No drawing or input here."""

import math
from dataclasses import dataclass
from typing import List

FIELD_WIDTH = 100.0
FIELD_HEIGHT = 40.0
DUCK_HALF_WIDTH = 5.0
DUCK_HALF_HEIGHT = 3.0
START_LIVES = 3
POINTS_PER_HIT = 10
WAVE_BREAK_SECONDS = 2.0
MAX_DUCKS = 64
BOB_AMPLITUDE = 1.5
BOB_SPEED = 2.0


class Rng:
    """Linear congruential generator; gives the same numbers as the C and JavaScript versions."""

    def __init__(self, seed: int):
        self.state = (seed & 0xFFFFFFFF) or 1

    def next(self) -> float:
        """Return a number in [0, 1)."""
        self.state = (self.state * 1664525 + 1013904223) & 0xFFFFFFFF
        return self.state / 4294967296.0


@dataclass
class Duck:
    x: float
    y: float
    base_y: float
    speed: float  # positive flies right, negative flies left
    age: float
    phase: float

    def has_escaped(self) -> bool:
        return (self.speed > 0 and self.x - DUCK_HALF_WIDTH > FIELD_WIDTH) or (
            self.speed < 0 and self.x + DUCK_HALF_WIDTH < 0
        )

    def is_hit_at(self, x: float, y: float) -> bool:
        return abs(x - self.x) <= DUCK_HALF_WIDTH and abs(y - self.y) <= DUCK_HALF_HEIGHT


def ducks_in_wave(wave: int) -> int:
    return 3 + 2 * wave


def spawn_interval(wave: int) -> float:
    return max(0.5, 2.0 - 0.15 * wave)


class Game:
    def __init__(self, seed: int = 1):
        self.rng = Rng(seed)
        self.ducks: List[Duck] = []
        self.wave = 0
        self.score = 0
        self.lives = START_LIVES
        self.ducks_to_spawn = 0
        self.spawn_timer = 0.0
        self.break_timer = 0.0  # above zero while the "wave cleared" pause runs
        self.game_over = False
        self._start_wave(1)

    def _start_wave(self, wave: int) -> None:
        self.wave = wave
        self.ducks_to_spawn = ducks_in_wave(wave)
        self.spawn_timer = 0.0
        self.break_timer = 0.0

    def _spawn_duck(self) -> None:
        flies_right = self.rng.next() < 0.5
        speed = (5.0 + 1.5 * self.wave) * (0.8 + 0.4 * self.rng.next())
        base_y = 5.0 + 22.0 * self.rng.next()
        phase = 2 * math.pi * self.rng.next()
        x = -DUCK_HALF_WIDTH if flies_right else FIELD_WIDTH + DUCK_HALF_WIDTH
        self.ducks.append(
            Duck(x=x, y=base_y, base_y=base_y, speed=speed if flies_right else -speed, age=0.0, phase=phase)
        )

    def update(self, dt: float) -> None:
        if self.game_over:
            return

        if self.break_timer > 0:
            self.break_timer -= dt
            if self.break_timer <= 0:
                self._start_wave(self.wave + 1)
            return

        for duck in self.ducks:
            duck.x += duck.speed * dt
            duck.age += dt
            duck.y = duck.base_y + BOB_AMPLITUDE * math.sin(BOB_SPEED * duck.age + duck.phase)

        escaped = sum(1 for duck in self.ducks if duck.has_escaped())
        self.ducks = [duck for duck in self.ducks if not duck.has_escaped()]
        self.lives -= escaped
        if self.lives <= 0:
            self.lives = 0
            self.game_over = True
            return

        if self.ducks_to_spawn > 0:
            self.spawn_timer -= dt
            if self.spawn_timer <= 0 and len(self.ducks) < MAX_DUCKS:
                self._spawn_duck()
                self.ducks_to_spawn -= 1
                self.spawn_timer = spawn_interval(self.wave)

        if self.ducks_to_spawn == 0 and not self.ducks:
            self.break_timer = WAVE_BREAK_SECONDS

    def shoot(self, x: float, y: float) -> bool:
        """Remove the first duck at (x, y) and score it. Returns True on a hit."""
        if self.game_over:
            return False
        for index, duck in enumerate(self.ducks):
            if duck.is_hit_at(x, y):
                del self.ducks[index]
                self.score += POINTS_PER_HIT
                return True
        return False
