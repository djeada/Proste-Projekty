"""
GUI for Timer application using tkinter.
"""
import tkinter as tk
from tkinter import ttk

import sys
import os

sys.path.insert(
    0, os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
)
from gui.styled_window import StyledWindow
from logic.timer import Timer


class Gui:
    """
    Main window for the Timer application.
    """

    def __init__(self, timer: Timer) -> None:
        self.root = StyledWindow()
        self.root.title("Timer")
        self.root.resizable(False, False)
        self.timer = timer
        self.setup()

    def run(self) -> None:
        """Start the main loop of the GUI."""
        self.update_display()
        self.root.mainloop()

    def setup(self) -> None:
        """Create the frames and internal widgets."""
        # Time display
        self.time_label = tk.Label(
            self.root,
            text="00:00:00",
            font=("Arial", 48, "bold"),
            fg="#333333",
        )
        self.time_label.pack(pady=30, padx=30)

        # Button frame
        button_frame = ttk.Frame(self.root)
        button_frame.pack(pady=20)

        # Start/Stop button
        self.start_stop_button = ttk.Button(
            button_frame, text="Start", command=self.toggle_timer, width=10
        )
        self.start_stop_button.grid(row=0, column=0, padx=5)

        # Reset button
        reset_button = ttk.Button(
            button_frame, text="Reset", command=self.reset_timer, width=10
        )
        reset_button.grid(row=0, column=1, padx=5)

    def toggle_timer(self) -> None:
        """Toggle timer between running and stopped."""
        if self.timer.is_running():
            self.timer.stop()
            self.start_stop_button.config(text="Start")
        else:
            self.timer.start()
            self.start_stop_button.config(text="Stop")

    def reset_timer(self) -> None:
        """Reset the timer."""
        self.timer.reset()
        self.start_stop_button.config(text="Start")
        self.time_label.config(text="00:00:00")

    def update_display(self) -> None:
        """Update the time display."""
        if self.timer.is_running():
            self.timer.tick()

        self.time_label.config(text=self.timer.get_formatted_time())
        self.root.after(1000, self.update_display)
