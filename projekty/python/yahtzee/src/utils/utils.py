from typing import Dict, List


def create_histogram(dices: List[int]) -> Dict[int, int]:
    histogram = {}
    for dice in dices:
        if dice.value in histogram:
            histogram[dice.value] += 1
        else:
            histogram[dice.value] = 1
    return histogram
