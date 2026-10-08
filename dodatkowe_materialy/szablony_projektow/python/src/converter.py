def celsius_to_fahrenheit(celsius):
    return celsius * 9 / 5 + 32


def fahrenheit_to_celsius(fahrenheit):
    return (fahrenheit - 32) * 5 / 9


def convert(value, unit):
    """Return (converted value, new unit). Raises ValueError for an unknown unit."""
    unit = unit.upper()
    if unit == "C":
        return celsius_to_fahrenheit(value), "F"
    if unit == "F":
        return fahrenheit_to_celsius(value), "C"
    raise ValueError(f"unknown unit: {unit}")
