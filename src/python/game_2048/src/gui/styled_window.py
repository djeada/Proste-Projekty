from tkinter import ttk

from ttkthemes.themed_tk import ThemedTk


class StyledWindow(ThemedTk):
    """
    Provides a window with a modern, consistent theme
    for the application.
    """

    # Color scheme
    BG_COLOR = "#f5f5f5"
    PRIMARY_COLOR = "#2196F3"
    SECONDARY_COLOR = "#4CAF50"
    ACCENT_COLOR = "#FF9800"
    TEXT_COLOR = "#212121"
    CARD_BG = "#ffffff"
    BORDER_COLOR = "#e0e0e0"

    def __init__(self):
        super().__init__(theme="arc")
        self.configure(background=self.BG_COLOR)
        self._configure_styles()

    def _configure_styles(self):
        """Configure custom ttk styles for consistent UI."""
        style = ttk.Style(self)

        # Frame styles
        style.configure(
            "Card.TFrame",
            background=self.CARD_BG,
        )

        style.configure(
            "TFrame",
            background=self.BG_COLOR
        )

        # Label styles
        style.configure(
            "TLabel",
            background=self.BG_COLOR,
            foreground=self.TEXT_COLOR,
        )

        style.configure(
            "Header.TLabel",
            font=("Helvetica", 16, "bold"),
            foreground=self.TEXT_COLOR,
            background=self.BG_COLOR,
            padding=(10, 5)
        )

        # Button styles
        style.configure(
            "TButton",
            padding=(10, 5)
        )

        style.configure(
            "Primary.TButton",
            font=("Helvetica", 11, "bold"),
        )

        # Entry styles
        style.configure(
            "TEntry",
            padding=(5, 5)
        )
