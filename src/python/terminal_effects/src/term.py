import atexit
import os
import signal
import sys
import time

COLS, ROWS = 64, 22
W, H = COLS, ROWS * 2

pixels = [[0] * W for _ in range(H)]
text = [[" "] * COLS for _ in range(ROWS)]
ink = [[0] * COLS for _ in range(ROWS)]

_frames_left = 0
_delay = 0.0
_state = 1
_last_frame = 0.0


def _restore():
    sys.stdout.write("\033[0m\033[?25h\n")
    sys.stdout.flush()


def start(delay_ms):
    global _frames_left, _delay, _last_frame
    _frames_left = int(sys.argv[1]) if len(sys.argv) > 1 else 0
    _delay = 0 if os.environ.get("NO_SLEEP") else delay_ms / 1000
    signal.signal(signal.SIGINT, lambda *_: sys.exit(0))
    signal.signal(signal.SIGTERM, lambda *_: sys.exit(0))
    atexit.register(_restore)
    sys.stdout.write("\033[?25l\033[2J")
    _last_frame = time.monotonic()


def _color(layer, c):
    return f"\033[{layer};2;{c >> 16};{c >> 8 & 255};{c & 255}m"


def _end_frame(out, status):
    global _frames_left, _last_frame
    out.append(f"\033[0m{status}\033[K")
    sys.stdout.write("".join(out))
    sys.stdout.flush()
    time.sleep(max(0.0, _delay - (time.monotonic() - _last_frame)))
    _last_frame = time.monotonic()
    if _frames_left > 0:
        _frames_left -= 1
        if _frames_left == 0:
            sys.exit(0)


def show_pixels(status=""):
    out = ["\033[H"]
    for y in range(0, H, 2):
        fg = bg = -1
        for top, bottom in zip(pixels[y], pixels[y + 1]):
            if top != fg:
                fg = top
                out.append(_color(38, fg))
            if bottom != bg:
                bg = bottom
                out.append(_color(48, bg))
            out.append("▀")
        out.append("\033[0m\n")
    _end_frame(out, status)


def show_text(status=""):
    out = ["\033[H"]
    for chars, colors in zip(text, ink):
        fg = -1
        out.append(_color(48, 0))
        for ch, c in zip(chars, colors):
            if ch != " " and c != fg:
                fg = c
                out.append(_color(38, fg))
            out.append(ch)
        out.append("\033[0m\n")
    _end_frame(out, status)


def seed(s):
    global _state
    _state = s


def rnd(n):
    global _state
    _state = (_state * 1103515245 + 12345) & 0xFFFFFFFF
    return (_state >> 16) % n
