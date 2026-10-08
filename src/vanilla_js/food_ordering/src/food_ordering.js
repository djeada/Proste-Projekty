// Food ordering rules: menu, order, validation, card check and order status. No DOM here.

const MAX_QUANTITY = 20;
const STATUS_STEP_SECONDS = 3;
const STATUS_NAMES = ['Received', 'Preparing', 'On the way', 'Delivered'];
const CATEGORY_NAMES = { starter: 'Starters', main: 'Main courses', dessert: 'Desserts', drink: 'Drinks' };
const PAYMENT_NAMES = { paypal: 'PayPal', card: 'Credit card', cash: 'Cash on delivery' };

// Prices are integers in cents, so 9.90 is 990 and no rounding errors appear.
const MENU = [
  { id: 1, name: 'Garlic Bread', description: 'Toasted baguette with garlic butter', priceCents: 450, category: 'starter' },
  { id: 2, name: 'Tomato Soup', description: 'Slow-cooked tomato soup with basil', priceCents: 520, category: 'starter' },
  { id: 3, name: 'Margherita Pizza', description: 'Tomato, mozzarella and fresh basil', priceCents: 1200, category: 'main' },
  { id: 4, name: 'Chicken Pasta', description: 'Penne with grilled chicken and pesto', priceCents: 1350, category: 'main' },
  { id: 5, name: 'Beef Burger', description: 'Beef patty, cheddar, pickles and fries', priceCents: 1450, category: 'main' },
  { id: 6, name: 'Grilled Salmon', description: 'Atlantic salmon with lemon and herbs', priceCents: 1890, category: 'main' },
  { id: 7, name: 'Cheesecake', description: 'Baked cheesecake with berry sauce', priceCents: 620, category: 'dessert' },
  { id: 8, name: 'Chocolate Cake', description: 'Rich dark chocolate layer cake', priceCents: 590, category: 'dessert' },
  { id: 9, name: 'Coffee', description: 'Freshly brewed coffee', priceCents: 290, category: 'drink' },
  { id: 10, name: 'Lemonade', description: 'Homemade lemonade with mint', priceCents: 350, category: 'drink' },
];

function dishById(dishId) {
  return MENU.find((dish) => dish.id === dishId) || null;
}

function newOrder() {
  return { lines: {}, address: '', phone: '', payment: null, cardLast4: '', placedAt: null };
}

function isPlaced(order) {
  return order.placedAt !== null;
}

// Sets the quantity of a dish. A quantity of 0 removes the dish.
function setQuantity(order, dishId, quantity) {
  if (isPlaced(order) || !dishById(dishId) || !Number.isInteger(quantity)) return false;
  if (quantity < 0 || quantity > MAX_QUANTITY) return false;
  if (quantity === 0) {
    delete order.lines[dishId];
  } else {
    order.lines[dishId] = quantity;
  }
  return true;
}

function addDish(order, dishId, quantity = 1) {
  if (quantity < 1) return false;
  return setQuantity(order, dishId, (order.lines[dishId] || 0) + quantity);
}

function itemCount(order) {
  return Object.values(order.lines).reduce((sum, quantity) => sum + quantity, 0);
}

function totalCents(order) {
  return Object.entries(order.lines).reduce(
    (sum, [dishId, quantity]) => sum + dishById(Number(dishId)).priceCents * quantity,
    0,
  );
}

function formatMoney(cents) {
  return `$${Math.floor(cents / 100)}.${String(cents % 100).padStart(2, '0')}`;
}

// A valid address has 5-100 characters, a street name (letters) and a house number (digits).
function validateAddress(text) {
  const clean = text.trim();
  if (clean.length < 5 || clean.length > 100) return null;
  if (!/\d/.test(clean) || !/\p{L}/u.test(clean)) return null;
  return clean;
}

function setAddress(order, text) {
  const clean = validateAddress(text);
  if (isPlaced(order) || clean === null) return false;
  order.address = clean;
  return true;
}

// Spaces, dashes and brackets are ignored. The result keeps an optional leading '+' and 9-15 digits.
function validatePhone(text) {
  const stripped = text.replace(/[\s()-]/g, '');
  const prefix = stripped.startsWith('+') ? '+' : '';
  const digits = stripped.slice(prefix.length);
  if (!/^\d{9,15}$/.test(digits)) return null;
  return prefix + digits;
}

function setPhone(order, text) {
  const clean = validatePhone(text);
  if (isPlaced(order) || clean === null) return false;
  order.phone = clean;
  return true;
}

// Returns the digits of a card number (spaces and dashes are ignored), or null if there are not 13-19 of them.
function cardDigits(text) {
  const digits = text.replace(/[ -]/g, '');
  return /^\d{13,19}$/.test(digits) ? digits : null;
}

// Luhn check: from the right, double every second digit (subtract 9 if over 9) and add them all up.
// A real card number gives a total that ends in 0.
function luhnIsValid(number) {
  const digits = cardDigits(number);
  if (digits === null) return false;
  let sum = 0;
  for (let index = 0; index < digits.length; index++) {
    let value = Number(digits[digits.length - 1 - index]);
    if (index % 2 === 1) {
      value *= 2;
      if (value > 9) value -= 9;
    }
    sum += value;
  }
  return sum % 10 === 0;
}

function setCard(order, number) {
  const digits = cardDigits(number);
  if (isPlaced(order) || digits === null || !luhnIsValid(number)) return false;
  order.payment = 'card';
  order.cardLast4 = digits.slice(-4);
  return true;
}

// Chooses PayPal or cash. Cards go through setCard, which checks the number.
function setPayment(order, payment) {
  if (isPlaced(order) || !['paypal', 'cash'].includes(payment)) return false;
  order.payment = payment;
  order.cardLast4 = '';
  return true;
}

// Returns null when the order is placed, otherwise the reason it cannot be placed.
function placeOrder(order, nowMs) {
  if (isPlaced(order)) return 'The order has already been placed.';
  if (itemCount(order) === 0) return 'Your order is empty. Add a dish first.';
  if (order.address === '') return 'Please enter a delivery address.';
  if (order.phone === '') return 'Please enter a phone number.';
  if (order.payment === null) return 'Please choose a payment method.';
  order.placedAt = nowMs;
  return null;
}

// Index into STATUS_NAMES. Each status lasts STATUS_STEP_SECONDS after the order was placed.
function statusAt(elapsedSeconds) {
  const index = Math.floor(elapsedSeconds / STATUS_STEP_SECONDS);
  return Math.min(Math.max(index, 0), STATUS_NAMES.length - 1);
}

if (typeof module !== 'undefined') {
  module.exports = {
    MENU, STATUS_NAMES, CATEGORY_NAMES, PAYMENT_NAMES, STATUS_STEP_SECONDS, MAX_QUANTITY,
    dishById, newOrder, isPlaced, setQuantity, addDish, itemCount, totalCents, formatMoney,
    validateAddress, setAddress, validatePhone, setPhone, cardDigits, luhnIsValid,
    setCard, setPayment, placeOrder, statusAt,
  };
}
