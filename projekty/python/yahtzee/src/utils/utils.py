from typing import Dict, List


def create_histogram(dice_list: List[int]) -> Dict[int, int]:
    histogram = {}
    for dice in dice_list:
        if dice.value in histogram:
            histogram[dice.value] += 1
        else:
            histogram[dice.value] = 1
    return histogram


def most_common_value(dice_list: List[int]) -> int:
    histogram = create_histogram(dice_list)
    max_value = 0
    max_key = 0
    for key, value in histogram.items():
        if value > max_value:
            max_value = value
            max_key = key
    return max_key
    