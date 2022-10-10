from typing import Dict, List


def create_histogram(dice_list: List[int]) -> Dict[int, int]:
    """
    Creates a histogram of the dice list.

    :param dice_list: list of dice
    :return: histogram of the dice list
    """
    histogram = {}
    for dice in dice_list:
        if dice.value in histogram:
            histogram[dice.value] += 1
        else:
            histogram[dice.value] = 1
    return histogram


def most_common_value(dice_list: List[int]) -> int:
    """
    Finds the most common value in the dice list.

    :param dice_list: list of dice
    :return: most common value in the dice list
    """
    histogram = create_histogram(dice_list)
    max_value = 0
    max_key = 0
    for key, value in histogram.items():
        if value > max_value:
            max_value = value
            max_key = key
    return max_key
