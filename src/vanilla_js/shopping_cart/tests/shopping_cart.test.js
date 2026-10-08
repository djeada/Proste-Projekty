const { test } = require('node:test');
const assert = require('node:assert/strict');
const {
  MAX_QUANTITY, CATALOG, createCart, addToCart, setQuantity, applyCode, clearCart,
  isEmpty, subtotalCents, discountCents, totalCents, validateName, validateAddress,
} = require('../src/shopping_cart.js');

test('catalog has eight products with prices in cents', () => {
  assert.equal(CATALOG.length, 8);
  assert.equal(CATALOG[0].priceCents, 349);
  assert.equal(CATALOG[4].priceCents, 2499);
});

test('adding products updates the subtotal', () => {
  const cart = createCart();
  assert.ok(isEmpty(cart));
  addToCart(cart, 0, 2);
  addToCart(cart, 2, 1);
  assert.equal(subtotalCents(cart), 2 * 349 + 875);
  assert.ok(!isEmpty(cart));
});

test('adding rejects bad products and quantities', () => {
  const cart = createCart();
  assert.throws(() => addToCart(cart, -1, 1));
  assert.throws(() => addToCart(cart, CATALOG.length, 1));
  assert.throws(() => addToCart(cart, 0, 0));
  assert.throws(() => addToCart(cart, 0, 1.5));
  addToCart(cart, 0, MAX_QUANTITY);
  assert.throws(() => addToCart(cart, 0, 1));
  assert.equal(cart.quantities[0], MAX_QUANTITY);
});

test('setting quantity to zero removes the product', () => {
  const cart = createCart();
  addToCart(cart, 1, 3);
  setQuantity(cart, 1, 0);
  assert.ok(isEmpty(cart));
  assert.throws(() => setQuantity(cart, 1, 100));
});

test('percentage discount is applied to the subtotal', () => {
  const cart = createCart();
  addToCart(cart, 4, 1); // 2499
  addToCart(cart, 7, 1); // 999
  applyCode(cart, ' save10 ');
  assert.equal(subtotalCents(cart), 3498);
  assert.equal(discountCents(cart), 350);
  assert.equal(totalCents(cart), 3148);
});

test('percentage discount rounds to the nearest cent', () => {
  const cart = createCart();
  addToCart(cart, 0, 1); // 349 cents: 10% is 34.9, rounds to 35
  applyCode(cart, 'SAVE10');
  assert.equal(discountCents(cart), 35);
  assert.equal(totalCents(cart), 314);
});

test('fixed discount needs the minimum order value', () => {
  const cart = createCart();
  addToCart(cart, 0, 8); // 2792: below the 30.00 minimum
  assert.throws(() => applyCode(cart, 'FLAT5'), /higher order value/);
  assert.equal(cart.discount, null);

  addToCart(cart, 4, 1); // 5291
  applyCode(cart, 'FLAT5');
  assert.equal(discountCents(cart), 500);
  assert.equal(totalCents(cart), 5291 - 500);
});

test('discount stops applying when the order falls below the minimum', () => {
  const cart = createCart();
  addToCart(cart, 4, 2); // 4998
  applyCode(cart, 'FLAT5');
  assert.equal(discountCents(cart), 500);
  setQuantity(cart, 4, 0);
  assert.equal(discountCents(cart), 0);
  assert.equal(totalCents(cart), 0);
});

test('unknown codes are rejected', () => {
  const cart = createCart();
  addToCart(cart, 4, 2);
  for (const text of ['FREE', 'SAVE1', '']) {
    assert.throws(() => applyCode(cart, text), /Unknown discount code/);
  }
  assert.equal(cart.discount, null);
});

test('clearing the cart removes items and the code', () => {
  const cart = createCart();
  addToCart(cart, 4, 2);
  applyCode(cart, 'FLAT5');
  clearCart(cart);
  assert.ok(isEmpty(cart));
  assert.equal(cart.discount, null);
  assert.equal(totalCents(cart), 0);
});

test('names need 2-40 characters and at least one letter', () => {
  for (const name of ['', 'A', '1234']) {
    assert.throws(() => validateName(name));
  }
  validateName('Anna');
  validateName('Jan Kowalski');
});

test('addresses need 5-80 characters and a digit', () => {
  for (const address of ['Main St', '12']) {
    assert.throws(() => validateAddress(address));
  }
  validateAddress('10 Main Street');
});
