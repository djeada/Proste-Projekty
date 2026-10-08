import pytest

from timer import Timer, countdown_ms, format_lap, format_time, laps_text, Lap


def test_format_time_without_hours():
    assert format_time(0) == "00:00.000"
    assert format_time(65432) == "01:05.432"


def test_format_time_with_hours():
    assert format_time(3723004) == "1:02:03.004"


def test_format_lap_and_laps_text():
    lap = Lap(split_ms=5120, total_ms=12300)
    assert format_lap(2, lap) == "Lap 2: split 00:05.120  total 00:12.300"
    assert laps_text([lap]) == "Lap 1: split 00:05.120  total 00:12.300\n"


def test_stopwatch_pauses_and_resumes():
    timer = Timer()
    timer.start(1000)
    assert timer.elapsed(1500) == 500
    timer.stop(2000)
    assert timer.elapsed(9000) == 1000
    timer.start(9000)
    assert timer.elapsed(9250) == 1250


def test_start_and_stop_ignore_repeats():
    timer = Timer()
    timer.stop(100)
    assert timer.elapsed(200) == 0
    timer.start(0)
    timer.start(500)
    assert timer.elapsed(1000) == 1000


def test_laps_have_split_and_total():
    timer = Timer()
    assert timer.lap(0) is None
    timer.start(0)
    assert timer.lap(1000) == Lap(split_ms=1000, total_ms=1000)
    assert timer.lap(1500) == Lap(split_ms=500, total_ms=1500)
    timer.stop(2000)
    assert timer.lap(3000) is None
    assert len(timer.laps) == 2


def test_reset_clears_time_and_laps():
    timer = Timer()
    timer.start(0)
    timer.lap(100)
    timer.stop(200)
    timer.reset()
    assert not timer.running
    assert timer.laps == []
    assert timer.elapsed(500) == 0


def test_countdown_counts_down_and_finishes():
    timer = Timer(countdown_ms(0, 3))
    assert timer.display(0) == 3000
    timer.start(0)
    assert timer.display(1000) == 2000
    assert not timer.finished(2999)
    assert timer.finished(3000)
    assert timer.display(5000) == 0


def test_countdown_stops_itself_at_zero():
    timer = Timer(2000)
    timer.start(0)
    timer.tick(1999)
    assert timer.running
    timer.tick(2000)
    assert not timer.running
    assert timer.elapsed(9000) == 2000


def test_finished_countdown_cannot_restart_until_reset():
    timer = Timer(1000)
    timer.start(0)
    timer.tick(1000)
    timer.start(2000)
    assert not timer.running
    timer.reset()
    timer.start(2000)
    assert timer.display(2500) == 500


def test_countdown_ms_validates_seconds():
    assert countdown_ms(1, 30) == 90000
    with pytest.raises(ValueError):
        countdown_ms(0, 60)
