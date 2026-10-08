from converter import convert


def main():
    text = input("Temperature (e.g. 36.6 C or 98 F): ")
    try:
        value, unit = text.split()
        result, result_unit = convert(float(value), unit)
    except ValueError:
        print("Expected a number followed by C or F.")
        return
    print(f"{float(value):.1f} {unit.upper()} = {result:.1f} {result_unit}")


if __name__ == "__main__":
    main()
