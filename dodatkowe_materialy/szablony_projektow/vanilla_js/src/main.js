const form = document.getElementById('form');
const valueInput = document.getElementById('value');
const unitSelect = document.getElementById('unit');
const output = document.getElementById('result');

form.addEventListener('submit', (event) => {
  event.preventDefault();
  const value = Number(valueInput.value);
  const result = convert(value, unitSelect.value);
  output.textContent = `${value.toFixed(1)} °${unitSelect.value} = ${result.value.toFixed(1)} °${result.unit}`;
});
