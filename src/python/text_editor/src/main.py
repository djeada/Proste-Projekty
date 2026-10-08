"""Window of the text editor: a tkinter Text widget with a status bar."""
import os
import sys
import tkinter as tk
import tkinter.font
from tkinter import filedialog, messagebox, simpledialog

from text_editor import count_lines, count_words, find_next, line_col


class Editor(tk.Tk):
    def __init__(self, path=None):
        super().__init__()
        self.path = None
        self.needle = ""
        self.message = ""

        self.text = tk.Text(self, wrap="word", undo=True, font="TkFixedFont", padx=6, pady=4)
        scrollbar = tk.Scrollbar(self, command=self.text.yview)
        self.text.configure(yscrollcommand=scrollbar.set)
        self.status = tk.Label(self, anchor="w", padx=6, relief="flat")
        self.status.pack(side="bottom", fill="x")
        scrollbar.pack(side="right", fill="y")
        self.text.pack(side="left", fill="both", expand=True)

        self.bind_keys()
        self.protocol("WM_DELETE_WINDOW", self.quit_editor)
        self.geometry("800x560")
        if path:
            self.open_path(path)
        self.update_status()
        self.text.focus_set()

    def bind_keys(self):
        shortcuts = {
            "<Control-n>": self.new_file,
            "<Control-o>": self.open_file,
            "<Control-s>": self.save,
            "<Control-f>": self.find,
            "<F3>": self.find_again,
            "<Control-g>": self.go_to_line,
            "<Control-q>": self.quit_editor,
            "<Tab>": self.indent,
        }
        for sequence, command in shortcuts.items():
            self.text.bind(sequence, self.shortcut(command))
        self.text.bind("<KeyPress>", self.clear_message)
        self.text.bind("<Button-1>", self.clear_message)
        self.text.bind("<<Modified>>", lambda event: self.update_status())
        self.text.bind("<KeyRelease>", lambda event: self.update_status())
        self.text.bind("<ButtonRelease-1>", lambda event: self.update_status())

    def shortcut(self, command):
        """Run a command for a key and stop the Text widget from handling that key too."""
        def handler(event):
            self.message = ""
            command()
            return "break"
        return handler

    def clear_message(self, event=None):
        self.message = ""

    def indent(self):
        self.text.insert("insert", "    ")

    def contents(self):
        return self.text.get("1.0", "end-1c")

    def is_modified(self):
        return self.text.edit_modified()

    def update_status(self):
        text = self.contents()
        line, column = line_col(text, len(self.text.get("1.0", "insert")))
        name = os.path.basename(self.path) if self.path else "(new file)"
        modified = " (modified)" if self.is_modified() else ""
        info = f"{name}{modified} | Ln {line}, Col {column} | {count_lines(text)} lines, {count_words(text)} words"
        self.status.configure(text=f"{self.message} | {info}" if self.message else info)
        self.title(f"{'*' if self.is_modified() else ''}{name} - Text editor")

    def confirm_discard(self):
        if not self.is_modified():
            return True
        return messagebox.askyesno("Unsaved changes", "Discard the unsaved changes?", parent=self)

    def open_path(self, path):
        self.path = path
        self.text.delete("1.0", "end")
        if not os.path.exists(path):
            self.message = "New file"
        else:
            try:
                with open(path, encoding="utf-8") as file:
                    self.text.insert("1.0", file.read())
            except (OSError, UnicodeDecodeError) as error:
                messagebox.showerror("Cannot open file", str(error), parent=self)
                self.path = None
                self.text.delete("1.0", "end")
        self.reset_cursor()

    def reset_cursor(self):
        """Put the cursor at the start, with the text unchanged on disk (not modified)."""
        self.text.mark_set("insert", "1.0")
        self.text.edit_modified(False)
        self.update_status()

    def new_file(self):
        if self.confirm_discard():
            self.path = None
            self.text.delete("1.0", "end")
            self.reset_cursor()

    def open_file(self):
        if not self.confirm_discard():
            return
        path = filedialog.askopenfilename(parent=self)
        if path:
            self.open_path(path)

    def save(self):
        if self.path is None:
            path = filedialog.asksaveasfilename(parent=self)
            if not path:
                return
            self.path = path
        try:
            with open(self.path, "w", encoding="utf-8") as file:
                file.write(self.contents())
        except OSError as error:
            messagebox.showerror("Cannot save file", str(error), parent=self)
            return
        self.text.edit_modified(False)
        self.message = "Saved"
        self.update_status()

    def find(self):
        needle = simpledialog.askstring("Find", "Text to find:", initialvalue=self.needle, parent=self)
        if needle:
            self.needle = needle
            self.find_again()

    def find_again(self):
        if not self.needle:
            self.find()
            return
        text = self.contents()
        cursor = len(self.text.get("1.0", "insert"))
        index = find_next(text, self.needle, cursor + 1)
        if index == -1:
            self.message = f"Not found: {self.needle}"
        else:
            start = f"1.0+{index}c"
            self.text.tag_remove("sel", "1.0", "end")
            self.text.tag_add("sel", start, f"{start}+{len(self.needle)}c")
            self.text.mark_set("insert", start)
            self.text.see(start)
            wrapped = index <= cursor
            self.message = "Found (wrapped around to the start)" if wrapped else "Found"
        self.update_status()

    def go_to_line(self):
        last = count_lines(self.contents())
        line = simpledialog.askinteger("Go to line", f"Line (1-{last}):", minvalue=1, maxvalue=last,
                                       parent=self)
        if line:
            self.text.mark_set("insert", f"{line}.0")
            self.text.see("insert")
            self.update_status()

    def quit_editor(self):
        if self.confirm_discard():
            self.destroy()


def main():
    editor = Editor(sys.argv[1] if len(sys.argv) > 1 else None)
    editor.mainloop()


if __name__ == "__main__":
    main()
