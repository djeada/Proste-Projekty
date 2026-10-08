const { test } = require('node:test');
const assert = require('node:assert/strict');
const {
  MENU, STATUS_STEP_SECONDS, MAX_QUANTITY, dishById, newOrder, setQuantity, addDish, itemCount,
  totalCents, formatMoney, validateAddress, setAddress, validatePhone, setPhone, luhnIsValid,
  setCard, setPayment, placeOrder, statusAt, isPlaced,
} = require('../src/food_ordering.js');

function orderReadyForCheckout() {
  const order = newOrder();
  addDish(order, 3);
  setAddress(order, 'Main Street 12');
  setPhone(order, '123 456 789');
  setPayment(order, 'cash');
  return order;
}

test('the menu has ten dishes in four categories with prices in cents', () => {
  assert.equal(MENU.length, 10);
  assert.equal(new Set(MENU.map((dish) => dish.category)).size, 4);
  assert.equal(dishById(3).name, 'Margherita Pizza');
  assert.equal(dishById(3).priceCents, 1200);
  assert.equal(dishById(11), null);
  assert.ok(MENU.every((dish) => Number.isInteger(dish.priceCents)));
});

test('dishes can be added, changed and removed', () => {
  const order = newOrder();
  assert.ok(addDish(order, 1, 2));
  assert.ok(addDish(order, 1));
  assert.equal(order.lines[1], 3);
  assert.ok(setQuantity(order, 1, 1));
  assert.equal(order.lines[1], 1);
  assert.ok(setQuantity(order, 1, 0));
  assert.deepEqual(order.lines, {});
});

test('quantity limits and unknown dishes are refused', () => {
  const order = newOrder();
  assert.equal(addDish(order, 99), false);
  assert.equal(addDish(order, 1, 0), false);
  assert.equal(setQuantity(order, 1, MAX_QUANTITY + 1), false);
  assert.equal(setQuantity(order, 1, -1), false);
  assert.equal(setQuantity(order, 1, 1.5), false);
  assert.ok(setQuantity(order, 1, MAX_QUANTITY));
  assert.equal(addDish(order, 1), false);
  assert.equal(order.lines[1], MAX_QUANTITY);
});

test('the total is computed in integer cents', () => {
  const order = newOrder();
  assert.equal(totalCents(order), 0);
  addDish(order, 9, 2); // 2 x 2.90
  addDish(order, 6); // 1 x 18.90
  assert.equal(totalCents(order), 2 * 290 + 1890);
  assert.equal(itemCount(order), 3);
  assert.equal(formatMoney(totalCents(order)), '$24.70');
  assert.equal(formatMoney(705), '$7.05');
});

test('address validation needs a street name, a house number and 5-100 characters', () => {
  assert.equal(validateAddress('  Main Street 12  '), 'Main Street 12');
  assert.equal(validateAddress('Main St'), null);
  assert.equal(validateAddress('12345'), null);
  assert.equal(validateAddress('1 A'), null);
  assert.equal(validateAddress(`${'A'.repeat(101)} 1`), null);
});

test('phone validation accepts separators and an optional leading plus, and 9-15 digits', () => {
  assert.equal(validatePhone('+48 (123) 456-789'), '+48123456789');
  assert.equal(validatePhone('123456789'), '123456789');
  assert.equal(validatePhone('12345'), null);
  assert.equal(validatePhone('1234567890123456'), null);
  assert.equal(validatePhone('123abc4567'), null);
  assert.equal(validatePhone('12+3456789'), null);
});

test('the Luhn check accepts valid card numbers and rejects others', () => {
  assert.ok(luhnIsValid('4111 1111 1111 1111'));
  assert.ok(luhnIsValid('4539-1488-0343-6467'));
  assert.ok(luhnIsValid('378282246310005'));
  assert.equal(luhnIsValid('4111 1111 1111 1112'), false);
  assert.equal(luhnIsValid('1234'), false);
  assert.equal(luhnIsValid('4111 1111 1111 abcd'), false);
});

test('a card payment stores only the last four digits', () => {
  const order = newOrder();
  assert.equal(setCard(order, '4111 1111 1111 1112'), false);
  assert.equal(order.payment, null);
  assert.ok(setCard(order, '4111 1111 1111 1111'));
  assert.equal(order.payment, 'card');
  assert.equal(order.cardLast4, '1111');
  assert.ok(setPayment(order, 'paypal'));
  assert.equal(order.cardLast4, '');
  assert.equal(setPayment(order, 'card'), false);
});

test('an order needs items, address, phone and payment before it can be placed', () => {
  const order = newOrder();
  assert.notEqual(placeOrder(order, 0), null);
  addDish(order, 3);
  assert.notEqual(placeOrder(order, 0), null);
  setAddress(order, 'Main Street 12');
  assert.notEqual(placeOrder(order, 0), null);
  setPhone(order, '123456789');
  assert.notEqual(placeOrder(order, 0), null);
  setPayment(order, 'cash');
  assert.equal(placeOrder(order, 1000), null);
  assert.equal(order.placedAt, 1000);
});

test('a placed order cannot be changed', () => {
  const order = orderReadyForCheckout();
  placeOrder(order, 0);
  assert.ok(isPlaced(order));
  assert.notEqual(placeOrder(order, 5), null);
  assert.equal(addDish(order, 1), false);
  assert.equal(setAddress(order, 'Other Road 5'), false);
  assert.equal(setPayment(order, 'paypal'), false);
});

test('the status follows the time since the order was placed', () => {
  const step = STATUS_STEP_SECONDS;
  assert.equal(statusAt(0), 0);
  assert.equal(statusAt(step - 0.5), 0);
  assert.equal(statusAt(step), 1);
  assert.equal(statusAt(2 * step), 2);
  assert.equal(statusAt(3 * step), 3);
  assert.equal(statusAt(1000), 3);
  assert.equal(statusAt(-5), 0);
});
