"""Tkinter window for the stopwatch and countdown."""

import time
import tkinter as tk
import tkinter.font as tkfont
from pathlib import Path

from timer import Timer, countdown_ms, format_lap, format_time, laps_text

LAPS_FILE = Path("laps.txt")
TICK_MS = 20


def now_ms():
    return time.monotonic_ns() // 1_000_000


class StopwatchApp:
    def __init__(self, root):
        self.root = root
        root.title("Stopwatch")
        self.timer = Timer()
        self.started = False
        self.alerted = False

        big_font = tkfont.nametofont("TkDefaultFont").copy()
        big_font.configure(size=36, weight="bold")
        self.display = tk.Label(root, font=big_font)
        self.display.grid(row=0, column=0, columnspan=5, padx=16, pady=(16, 8))

        self.start_button = tk.Button(root, width=8, command=self.toggle)
        self.start_button.grid(row=1, column=0, padx=4)
        tk.Button(root, text="Lap", width=8, command=self.lap).grid(row=1, column=1, padx=4)
        tk.Button(root, text="Reset", width=8, command=self.reset).grid(row=1, column=2, padx=4)

        self.countdown = tk.BooleanVar(value=False)
        self.minutes = tk.StringVar(value="1")
        self.seconds = tk.StringVar(value="0")
        check = tk.Checkbutton(root, text="Countdown", variable=self.countdown, command=self.config_changed)
        min_box = tk.Spinbox(root, from_=0, to=999, width=4, textvariable=self.minutes)
        sec_box = tk.Spinbox(root, from_=0, to=59, width=4, textvariable=self.seconds)
        self.config_widgets = [check, min_box, sec_box]
        check.grid(row=2, column=0, pady=8)
        min_box.grid(row=2, column=1)
        tk.Label(root, text="min").grid(row=2, column=2)
        sec_box.grid(row=2, column=3)
        tk.Label(root, text="sec").grid(row=2, column=4)
        self.minutes.trace_add("write", self.config_changed)
        self.seconds.trace_add("write", self.config_changed)

        self.laps_box = tk.Listbox(root, width=56, height=8)
        self.laps_box.grid(row=3, column=0, columnspan=5, padx=16, pady=8, sticky="nsew")
        self.status = tk.Label(root, text="")
        self.status.grid(row=4, column=0, columnspan=5, pady=(0, 12))

        self.refresh(now_ms())
        self._tick()

    def limit_ms(self):
        if not self.countdown.get():
            return 0
        try:
            return countdown_ms(int(self.minutes.get()), int(self.seconds.get()))
        except ValueError:
            self.status.config(text="Seconds must be 0 to 59.", fg="red")
            return 0

    def config_changed(self, *_):
        if not self.started:
            self.timer = Timer(self.limit_ms())
            self.refresh(now_ms())

    def toggle(self):
        now = now_ms()
        if self.timer.running:
            self.timer.stop(now)
        else:
            self.started = True
            self.timer.start(now)
            self.set_config_state("disabled")
        self.refresh(now)

    def lap(self):
        lap = self.timer.lap(now_ms())
        if lap is None:
            return
        self.laps_box.insert(tk.END, format_lap(len(self.timer.laps), lap))
        try:
            LAPS_FILE.write_text(laps_text(self.timer.laps), encoding="utf-8")
            self.status.config(text="Lap saved to laps.txt.", fg="black")
        except OSError as error:
            self.status.config(text=f"Could not write laps.txt: {error}", fg="red")

    def reset(self):
        self.started = False
        self.alerted = False
        self.timer = Timer(self.limit_ms())
        self.laps_box.delete(0, tk.END)
        self.status.config(text="", fg="black")
        self.set_config_state("normal")
        self.refresh(now_ms())

    def set_config_state(self, state):
        for widget in self.config_widgets:
            widget.config(state=state)

    def refresh(self, now):
        self.display.config(text=format_time(self.timer.display(now)))
        self.start_button.config(text="Stop" if self.timer.running else "Start")

    def _tick(self):
        now = now_ms()
        self.timer.tick(now)
        if self.timer.finished(now) and not self.alerted:
            self.alerted = True
            self.root.bell()
            self.status.config(text="Time is up!", fg="red")
        self.refresh(now)
        self.root.after(TICK_MS, self._tick)


def main():
    root = tk.Tk()
    StopwatchApp(root)
    root.mainloop()


if __name__ == "__main__":
    main()
