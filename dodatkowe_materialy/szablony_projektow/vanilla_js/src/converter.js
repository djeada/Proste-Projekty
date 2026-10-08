function celsiusToFahrenheit(celsius) {
  return (celsius * 9) / 5 + 32;
}

function fahrenheitToCelsius(fahrenheit) {
  return ((fahrenheit - 32) * 5) / 9;
}

function convert(value, unit) {
  switch (unit.toUpperCase()) {
    case 'C':
      return { value: celsiusToFahrenheit(value), unit: 'F' };
    case 'F':
      return { value: fahrenheitToCelsius(value), unit: 'C' };
    default:
      throw new Error(`unknown unit: ${unit}`);
  }
}

if (typeof module !== 'undefined') module.exports = { celsiusToFahrenheit, fahrenheitToCelsius, convert };
