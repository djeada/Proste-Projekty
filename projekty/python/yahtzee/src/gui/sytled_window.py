from ttkthemes.themed_tk import ThemedTk


class StyledWindow(ThemedTk):
    """
    The purpose of this class is to provide
    a window with a custom theme.
    """

    def __init__(self):
        super().__init__(theme="adapta")
        self.configure(background="#fafafa")
