const display = document.getElementById('display');

let currentValue = '0';
let previousValue = '';
let operator = '';
let shouldResetDisplay = false;

function updateDisplay() {
  display.value = currentValue;
}

function appendNumber(num) {
  if (shouldResetDisplay) {
    currentValue = num;
    shouldResetDisplay = false;
  } else if (currentValue === '0' && num !== '.') {
    currentValue = num;
  } else if (num === '.' && currentValue.includes('.')) {
    return;
  } else {
    currentValue += num;
  }
  updateDisplay();
}

function setOperator(op) {
  if (operator && !shouldResetDisplay) {
    calculate();
  }
  previousValue = currentValue;
  operator = op;
  shouldResetDisplay = true;
}

function calculate() {
  if (!operator || shouldResetDisplay) return;
  
  const prev = parseFloat(previousValue);
  const current = parseFloat(currentValue);
  let result;
  
  switch (operator) {
    case '+':
      result = prev + current;
      break;
    case '-':
      result = prev - current;
      break;
    case '*':
      result = prev * current;
      break;
    case '/':
      if (current === 0) {
        currentValue = 'Error';
        updateDisplay();
        operator = '';
        previousValue = '';
        shouldResetDisplay = true;
        return;
      }
      result = prev / current;
      break;
    default:
      return;
  }
  
  currentValue = String(result);
  operator = '';
  previousValue = '';
  shouldResetDisplay = true;
  updateDisplay();
}

function clear() {
  currentValue = '0';
  previousValue = '';
  operator = '';
  shouldResetDisplay = false;
  updateDisplay();
}

// Number buttons
const numbers = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.'];
for (const num of numbers) {
  const btn = document.getElementById(num);
  if (btn) {
    btn.addEventListener('click', () => appendNumber(num));
  }
}

// Operator buttons
const operatorMap = {
  'plus': '+',
  'minus': '-',
  'multiply': '*',
  'divide': '/'
};

for (const [id, op] of Object.entries(operatorMap)) {
  document.getElementById(id).addEventListener('click', () => setOperator(op));
}

// Clear button
document.getElementById('clear').addEventListener('click', clear);

// Equal button
document.getElementById('equal').addEventListener('click', calculate);

// Keyboard support
document.addEventListener('keydown', (e) => {
  if (e.key >= '0' && e.key <= '9' || e.key === '.') {
    appendNumber(e.key);
  } else if (e.key === '+') {
    setOperator('+');
  } else if (e.key === '-') {
    setOperator('-');
  } else if (e.key === '*') {
    setOperator('*');
  } else if (e.key === '/') {
    e.preventDefault();
    setOperator('/');
  } else if (e.key === 'Enter' || e.key === '=') {
    e.preventDefault();
    calculate();
  } else if (e.key === 'Escape' || e.key === 'c' || e.key === 'C') {
    clear();
  }
});