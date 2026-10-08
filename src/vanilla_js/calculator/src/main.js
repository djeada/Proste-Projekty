// Browser interface: buttons and keyboard edit the expression, evaluate() from calculator.js computes it.
const historyElement = document.getElementById('history');
const display = document.getElementById('display');
const statusElement = document.getElementById('status');

const KEYBOARD = { Enter: '=', Escape: 'C', Backspace: 'Backspace' };
const ALLOWED_KEYS = [...'0123456789.+-*/()=', 'C', 'Backspace'];
const OPERATOR_KEYS = '+-*/';

let expression = '';
let lastExpression = '';
let showsResult = false;

function render() {
  historyElement.textContent = lastExpression;
  display.textContent = expression || '0';
}

function press(key) {
  statusElement.textContent = '';
  if (key === '=') {
    calculate();
    return;
  }
  if (key === 'C') {
    expression = '';
  } else if (key === 'Backspace') {
    expression = expression.slice(0, -1);
  } else {
    if (showsResult && !OPERATOR_KEYS.includes(key)) expression = ''; // a new number starts a new calculation
    expression += key;
  }
  showsResult = false;
  render();
}

function calculate() {
  try {
    const value = evaluate(expression);
    lastExpression = expression;
    expression = String(Number(value.toPrecision(10)));
    showsResult = true;
  } catch (error) {
    statusElement.textContent = error.message;
  }
  render();
}

document.querySelectorAll('button[data-key]').forEach((button) => {
  button.addEventListener('click', () => press(button.dataset.key));
});

document.addEventListener('keydown', (event) => {
  const key = KEYBOARD[event.key] || event.key;
  if (!ALLOWED_KEYS.includes(key)) return;
  event.preventDefault(); // stops "/" from opening quick find and Backspace from going back
  press(key);
});

render();
