import unittest
import sys
import os

sys.path.insert(
    0, os.path.abspath(os.path.join(os.path.dirname(__file__), "../src/logic"))
)
from timer import Timer


class TestTimer(unittest.TestCase):
    def test_initial_state(self):
        timer = Timer()
        self.assertEqual(timer.seconds, 0)
        self.assertFalse(timer.running)

    def test_start(self):
        timer = Timer()
        timer.start()
        self.assertTrue(timer.is_running())

    def test_stop(self):
        timer = Timer()
        timer.start()
        timer.stop()
        self.assertFalse(timer.is_running())

    def test_tick_when_running(self):
        timer = Timer()
        timer.start()
        timer.tick()
        self.assertEqual(timer.seconds, 1)

    def test_tick_when_stopped(self):
        timer = Timer()
        timer.tick()
        self.assertEqual(timer.seconds, 0)

    def test_reset(self):
        timer = Timer()
        timer.start()
        timer.tick()
        timer.tick()
        timer.reset()
        self.assertEqual(timer.seconds, 0)
        self.assertFalse(timer.running)

    def test_get_time(self):
        timer = Timer()
        timer.seconds = 3661  # 1 hour, 1 minute, 1 second
        hours, minutes, secs = timer.get_time()
        self.assertEqual(hours, 1)
        self.assertEqual(minutes, 1)
        self.assertEqual(secs, 1)

    def test_get_formatted_time(self):
        timer = Timer()
        timer.seconds = 3661
        self.assertEqual(timer.get_formatted_time(), "01:01:01")

    def test_get_formatted_time_zero(self):
        timer = Timer()
        self.assertEqual(timer.get_formatted_time(), "00:00:00")


if __name__ == "__main__":
    unittest.main()
