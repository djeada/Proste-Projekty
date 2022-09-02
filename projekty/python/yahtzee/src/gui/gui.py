import tkinter as tk

from src.gui.game_gui import GameGui


class Gui:

    def __init__(self):
        root = tk.Tk()
        root.geometry("650x720")
        root.resizable(False, False)
        GameGui(root)
        root.mainloop()
