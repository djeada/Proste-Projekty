"""Stopwatch and countdown rules. Time is given in milliseconds by the caller."""

from dataclasses import dataclass
from typing import List, Optional

MAX_LAPS = 100


@dataclass(frozen=True)
class Lap:
    split_ms: int  # time since the previous lap
    total_ms: int  # time since the start


class Timer:
    def __init__(self, limit_ms: int = 0):
        self.limit_ms = limit_ms  # countdown length, 0 for a stopwatch
        self.laps: List[Lap] = []
        self._accumulated_ms = 0
        self._started_at_ms = 0
        self.running = False

    def _raw_elapsed(self, now: int) -> int:
        if not self.running:
            return self._accumulated_ms
        return self._accumulated_ms + now - self._started_at_ms

    def elapsed(self, now: int) -> int:
        elapsed = self._raw_elapsed(now)
        if self.limit_ms > 0:
            return min(elapsed, self.limit_ms)
        return elapsed

    def display(self, now: int) -> int:
        """Elapsed time for a stopwatch, remaining time for a countdown."""
        if self.limit_ms > 0:
            return self.limit_ms - self.elapsed(now)
        return self.elapsed(now)

    def finished(self, now: int) -> bool:
        return self.limit_ms > 0 and self.elapsed(now) >= self.limit_ms

    def start(self, now: int) -> None:
        if self.running or self.finished(now):
            return
        self.running = True
        self._started_at_ms = now

    def stop(self, now: int) -> None:
        if self.running:
            self._accumulated_ms = self._raw_elapsed(now)
            self.running = False

    def tick(self, now: int) -> None:
        """Stops a countdown that has reached zero. Call it regularly."""
        if self.running and self.finished(now):
            self.stop(now)

    def lap(self, now: int) -> Optional[Lap]:
        if not self.running or len(self.laps) >= MAX_LAPS:
            return None
        total = self.elapsed(now)
        previous = self.laps[-1].total_ms if self.laps else 0
        lap = Lap(split_ms=total - previous, total_ms=total)
        self.laps.append(lap)
        return lap

    def reset(self) -> None:
        self._accumulated_ms = 0
        self._started_at_ms = 0
        self.running = False
        self.laps = []


def countdown_ms(minutes: int, seconds: int) -> int:
    if not 0 <= seconds <= 59 or minutes < 0:
        raise ValueError("minutes must be 0 or more and seconds 0 to 59")
    return (minutes * 60 + seconds) * 1000


def format_time(ms: int) -> str:
    """Formats as mm:ss.mmm, with hours in front when needed: 1:02:03.004."""
    hours, rest = divmod(ms, 3_600_000)
    minutes, rest = divmod(rest, 60_000)
    seconds, millis = divmod(rest, 1000)
    if hours:
        return f"{hours}:{minutes:02d}:{seconds:02d}.{millis:03d}"
    return f"{minutes:02d}:{seconds:02d}.{millis:03d}"


def format_lap(number: int, lap: Lap) -> str:
    return f"Lap {number}: split {format_time(lap.split_ms)}  total {format_time(lap.total_ms)}"


def laps_text(laps: List[Lap]) -> str:
    return "".join(format_lap(number, lap) + "\n" for number, lap in enumerate(laps, start=1))
