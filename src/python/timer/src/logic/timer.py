"""
Timer logic.
"""


class Timer:
    """
    Simple timer/stopwatch.
    """

    def __init__(self) -> None:
        self.seconds: int = 0
        self.running: bool = False

    def start(self) -> None:
        """Start the timer."""
        self.running = True

    def stop(self) -> None:
        """Stop the timer."""
        self.running = False

    def reset(self) -> None:
        """Reset the timer to zero."""
        self.seconds = 0
        self.running = False

    def tick(self) -> None:
        """Increment timer by one second if running."""
        if self.running:
            self.seconds += 1

    def get_time(self) -> tuple:
        """
        Get the current time as (hours, minutes, seconds).

        :return: Tuple of (hours, minutes, seconds)
        """
        hours = self.seconds // 3600
        minutes = (self.seconds % 3600) // 60
        secs = self.seconds % 60
        return (hours, minutes, secs)

    def get_formatted_time(self) -> str:
        """
        Get the current time as a formatted string.

        :return: Time in HH:MM:SS format
        """
        hours, minutes, secs = self.get_time()
        return f"{hours:02d}:{minutes:02d}:{secs:02d}"

    def is_running(self) -> bool:
        """Check if timer is running."""
        return self.running
