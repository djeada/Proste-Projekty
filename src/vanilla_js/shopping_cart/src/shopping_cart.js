// Shopping cart rules: catalog, quantities, discount codes, totals and validation. No DOM here.
// Money is kept in integer cents: 0.10 cannot be stored exactly as a floating-point number.

const MAX_QUANTITY = 99;

const CATALOG = [
  { name: 'Notebook', category: 'Stationery', priceCents: 349 },
  { name: 'Pen set', category: 'Stationery', priceCents: 1290 },
  { name: 'Coffee mug', category: 'Kitchen', priceCents: 875 },
  { name: 'Water bottle', category: 'Kitchen', priceCents: 1120 },
  { name: 'Desk lamp', category: 'Home', priceCents: 2499 },
  { name: 'T-shirt', category: 'Clothing', priceCents: 1500 },
  { name: 'Running socks', category: 'Clothing', priceCents: 650 },
  { name: 'Paperback novel', category: 'Books', priceCents: 999 },
];

const DISCOUNT_CODES = [
  { code: 'SAVE10', percent: 10, amountCents: 0, minOrderCents: 0 },
  { code: 'FLAT5', percent: 0, amountCents: 500, minOrderCents: 3000 },
];

// quantities[i] is the number of CATALOG[i] items in the cart
function createCart() {
  return { quantities: CATALOG.map(() => 0), discount: null };
}

function checkProduct(product) {
  if (!Number.isInteger(product) || product < 0 || product >= CATALOG.length) {
    throw new Error('There is no product with that number.');
  }
}

function addToCart(cart, product, quantity) {
  checkProduct(product);
  if (!Number.isInteger(quantity) || quantity < 1 || cart.quantities[product] + quantity > MAX_QUANTITY) {
    throw new Error('Quantity is out of range: a product can be in the cart at most 99 times.');
  }
  cart.quantities[product] += quantity;
}

function setQuantity(cart, product, quantity) {
  checkProduct(product);
  if (!Number.isInteger(quantity) || quantity < 0 || quantity > MAX_QUANTITY) {
    throw new Error('Quantity must be from 0 to 99.');
  }
  cart.quantities[product] = quantity;
}

function applyCode(cart, text) {
  const code = DISCOUNT_CODES.find((candidate) => candidate.code === text.trim().toUpperCase());
  if (!code) {
    throw new Error('Unknown discount code.');
  }
  if (subtotalCents(cart) < code.minOrderCents) {
    throw new Error('This code needs a higher order value.');
  }
  cart.discount = code;
}

function clearCart(cart) {
  cart.quantities = CATALOG.map(() => 0);
  cart.discount = null;
}

function isEmpty(cart) {
  return cart.quantities.every((quantity) => quantity === 0);
}

function subtotalCents(cart) {
  return CATALOG.reduce((sum, product, i) => sum + product.priceCents * cart.quantities[i], 0);
}

function discountCents(cart) {
  const code = cart.discount;
  const subtotal = subtotalCents(cart);
  if (!code || subtotal < code.minOrderCents) {
    return 0;
  }
  if (code.percent > 0) {
    return Math.floor((subtotal * code.percent + 50) / 100); // rounded to the nearest cent
  }
  return Math.min(code.amountCents, subtotal);
}

function totalCents(cart) {
  return subtotalCents(cart) - discountCents(cart);
}

function validateName(name) {
  if (name.length < 2 || name.length > 40 || !/\p{L}/u.test(name)) {
    throw new Error('Name must be 2-40 characters and contain a letter.');
  }
}

function validateAddress(address) {
  if (address.length < 5 || address.length > 80 || !/\d/.test(address)) {
    throw new Error('Address must be 5-80 characters and contain a house number (a digit).');
  }
}

if (typeof module !== 'undefined') {
  module.exports = {
    MAX_QUANTITY, CATALOG, DISCOUNT_CODES, createCart, addToCart, setQuantity, applyCode, clearCart,
    isEmpty, subtotalCents, discountCents, totalCents, validateName, validateAddress,
  };
}
