from tkinter import ttk
from ttkthemes.themed_tk import ThemedTk


class StyledWindow(ThemedTk):
    """
    The purpose of this class is to provide
    a window with a custom theme and consistent styling.
    """

    # Color scheme
    BG_COLOR = "#f5f5f5"
    PRIMARY_COLOR = "#2196F3"
    SECONDARY_COLOR = "#4CAF50"
    ACCENT_COLOR = "#FF9800"
    TEXT_COLOR = "#212121"
    DICE_BG = "#ffffff"
    DICE_BORDER = "#bdbdbd"

    def __init__(self):
        super().__init__(theme="arc")
        self.configure(background=self.BG_COLOR)
        self._configure_styles()

    def _configure_styles(self):
        """Configure custom ttk styles for the application."""
        style = ttk.Style(self)

        # Frame styles
        style.configure(
            "Card.TFrame",
            background=self.DICE_BG,
            relief="solid",
            borderwidth=1
        )

        style.configure(
            "Main.TFrame",
            background=self.BG_COLOR
        )

        # Label styles
        style.configure(
            "Header.TLabel",
            font=("Helvetica", 16, "bold"),
            foreground=self.TEXT_COLOR,
            background=self.BG_COLOR,
            padding=(10, 5)
        )

        style.configure(
            "SubHeader.TLabel",
            font=("Helvetica", 12, "bold"),
            foreground=self.TEXT_COLOR,
            background=self.DICE_BG,
            padding=(5, 3)
        )

        style.configure(
            "Score.TLabel",
            font=("Helvetica", 11),
            foreground=self.TEXT_COLOR,
            background=self.DICE_BG,
            padding=(5, 2)
        )

        style.configure(
            "ScoreValue.TLabel",
            font=("Helvetica", 11, "bold"),
            foreground=self.PRIMARY_COLOR,
            background=self.DICE_BG,
            padding=(5, 2)
        )

        style.configure(
            "DiceLabel.TLabel",
            font=("Helvetica", 10),
            foreground="#666666",
            background=self.DICE_BG,
            padding=(5, 5)
        )

        # Button styles
        style.configure(
            "Dice.TButton",
            font=("Helvetica", 18, "bold"),
            padding=(15, 15)
        )

        style.configure(
            "Roll.TButton",
            font=("Helvetica", 12, "bold"),
            padding=(20, 10)
        )

        style.configure(
            "Move.TButton",
            font=("Helvetica", 10),
            padding=(10, 5)
        )

        style.configure(
            "Active.TLabel",
            font=("Helvetica", 14, "bold"),
            foreground=self.SECONDARY_COLOR,
            background=self.BG_COLOR
        )

        style.configure(
            "Inactive.TLabel",
            font=("Helvetica", 14),
            foreground="#9e9e9e",
            background=self.BG_COLOR
        )
