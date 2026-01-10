"""
GUI for Text Editor using tkinter.
"""
import tkinter as tk
from tkinter import ttk, filedialog, messagebox

import sys
import os

sys.path.insert(
    0, os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
)
from gui.styled_window import StyledWindow
from logic.buffer import TextBuffer


class Gui:
    """
    Main window for the Text Editor.
    """

    def __init__(self, buffer: TextBuffer) -> None:
        self.root = StyledWindow()
        self.root.title("Text Editor")
        self.root.geometry("800x600")
        self.buffer = buffer
        self.setup()

    def run(self) -> None:
        """Start the main loop of the GUI."""
        self.root.mainloop()

    def setup(self) -> None:
        """Create the frames and internal widgets."""
        # Menu bar
        menubar = tk.Menu(self.root)
        self.root.config(menu=menubar)

        file_menu = tk.Menu(menubar, tearoff=0)
        menubar.add_cascade(label="File", menu=file_menu)
        file_menu.add_command(label="New", command=self.new_file, accelerator="Ctrl+N")
        file_menu.add_command(
            label="Open...", command=self.open_file, accelerator="Ctrl+O"
        )
        file_menu.add_command(label="Save", command=self.save_file, accelerator="Ctrl+S")
        file_menu.add_command(label="Save As...", command=self.save_file_as)
        file_menu.add_separator()
        file_menu.add_command(label="Exit", command=self.on_close)

        # Text area with scrollbar
        text_frame = ttk.Frame(self.root)
        text_frame.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)

        scrollbar = ttk.Scrollbar(text_frame)
        scrollbar.pack(side=tk.RIGHT, fill=tk.Y)

        self.text_area = tk.Text(
            text_frame,
            yscrollcommand=scrollbar.set,
            font=("Courier", 12),
            undo=True,
        )
        self.text_area.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        scrollbar.config(command=self.text_area.yview)

        # Status bar
        self.status_bar = ttk.Label(
            self.root, text="Ready", relief=tk.SUNKEN, anchor=tk.W
        )
        self.status_bar.pack(side=tk.BOTTOM, fill=tk.X)

        # Bindings
        self.root.bind("<Control-n>", lambda e: self.new_file())
        self.root.bind("<Control-o>", lambda e: self.open_file())
        self.root.bind("<Control-s>", lambda e: self.save_file())
        self.text_area.bind("<<Modified>>", self.on_text_modified)
        self.root.protocol("WM_DELETE_WINDOW", self.on_close)

    def on_text_modified(self, event=None) -> None:
        """Handle text modification."""
        if self.text_area.edit_modified():
            self.buffer.modified = True
            self.update_title()
            self.text_area.edit_modified(False)

    def update_title(self) -> None:
        """Update window title to show filename and modified status."""
        name = self.buffer.filename if self.buffer.filename else "Untitled"
        modified = "*" if self.buffer.modified else ""
        self.root.title(f"{modified}{name} - Text Editor")

    def new_file(self) -> None:
        """Create a new file."""
        if self.buffer.modified:
            if not messagebox.askyesno(
                "Unsaved Changes", "Discard unsaved changes?"
            ):
                return

        self.text_area.delete(1.0, tk.END)
        self.buffer.clear()
        self.update_title()
        self.status_bar.config(text="New file")

    def open_file(self) -> None:
        """Open a file."""
        if self.buffer.modified:
            if not messagebox.askyesno(
                "Unsaved Changes", "Discard unsaved changes?"
            ):
                return

        filename = filedialog.askopenfilename(
            filetypes=[("Text Files", "*.txt"), ("All Files", "*.*")]
        )
        if filename:
            if self.buffer.load(filename):
                self.text_area.delete(1.0, tk.END)
                self.text_area.insert(1.0, self.buffer.get_content())
                self.text_area.edit_modified(False)
                self.buffer.modified = False
                self.update_title()
                self.status_bar.config(text=f"Opened: {filename}")
            else:
                messagebox.showerror("Error", f"Could not open file: {filename}")

    def save_file(self) -> None:
        """Save the current file."""
        if not self.buffer.filename:
            self.save_file_as()
            return

        self.buffer.set_content(self.text_area.get(1.0, tk.END).rstrip())
        if self.buffer.save():
            self.update_title()
            self.status_bar.config(text=f"Saved: {self.buffer.filename}")
        else:
            messagebox.showerror("Error", "Could not save file")

    def save_file_as(self) -> None:
        """Save the file with a new name."""
        filename = filedialog.asksaveasfilename(
            defaultextension=".txt",
            filetypes=[("Text Files", "*.txt"), ("All Files", "*.*")],
        )
        if filename:
            self.buffer.set_content(self.text_area.get(1.0, tk.END).rstrip())
            if self.buffer.save(filename):
                self.update_title()
                self.status_bar.config(text=f"Saved: {filename}")
            else:
                messagebox.showerror("Error", "Could not save file")

    def on_close(self) -> None:
        """Handle window close."""
        if self.buffer.modified:
            if not messagebox.askyesno(
                "Unsaved Changes", "Discard unsaved changes and exit?"
            ):
                return
        self.root.destroy()
