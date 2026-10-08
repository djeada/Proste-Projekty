// Page logic: draws the catalog, the cart and the order summary, and handles the forms.
const state = { cart: createCart(), order: null };

const el = (id) => document.getElementById(id);

function formatMoney(cents) {
  const abs = Math.abs(cents);
  const sign = cents < 0 ? '-' : '';
  return `${sign}$${Math.floor(abs / 100)}.${String(abs % 100).padStart(2, '0')}`;
}

function makeCell(content, className = '') {
  const td = document.createElement('td');
  if (content instanceof Node) {
    td.appendChild(content);
  } else {
    td.textContent = content;
  }
  if (className) td.className = className;
  return td;
}

function headerRow(...names) {
  const tr = document.createElement('tr');
  names.forEach(([name, className]) => {
    const th = document.createElement('th');
    th.textContent = name;
    if (className) th.className = className;
    tr.appendChild(th);
  });
  return tr;
}

function makeQuantityInput(value) {
  const input = document.createElement('input');
  input.type = 'number';
  input.min = '0';
  input.max = String(MAX_QUANTITY);
  input.value = String(value);
  input.setAttribute('aria-label', 'Quantity');
  return input;
}

function makeButton(label, onClick, className = '') {
  const button = document.createElement('button');
  button.type = 'button';
  button.textContent = label;
  if (className) button.className = className;
  button.addEventListener('click', onClick);
  return button;
}

function showMessage(id, text, isError = false) {
  const p = el(id);
  p.textContent = text;
  p.classList.toggle('error', isError);
}

// Runs a cart change; shows the error message instead of stopping the page.
function changeCart(action) {
  try {
    action();
    showMessage('cart-message', '');
  } catch (error) {
    showMessage('cart-message', error.message, true);
  }
  render();
}

function renderCatalog() {
  const table = el('catalog');
  table.replaceChildren(headerRow(['Product'], ['Category'], ['Price', 'num'], ['Quantity', 'num'], ['']));
  CATALOG.forEach((product, index) => {
    const quantity = makeQuantityInput(1);
    const add = makeButton('Add to cart', () => {
      state.order = null;
      changeCart(() => addToCart(state.cart, index, Number(quantity.value)));
    });
    const tr = document.createElement('tr');
    tr.dataset.index = String(index);
    tr.append(
      makeCell(product.name),
      makeCell(product.category),
      makeCell(formatMoney(product.priceCents), 'num'),
      makeCell(quantity, 'num'),
      makeCell(add),
    );
    table.appendChild(tr);
  });
}

function renderCart() {
  const table = el('cart');
  const empty = isEmpty(state.cart);
  el('cart-empty').hidden = !empty;
  table.hidden = empty;
  el('totals').hidden = empty;
  table.replaceChildren();
  if (empty) return;

  table.appendChild(headerRow(['Product'], ['Quantity', 'num'], ['Price', 'num'], ['Line total', 'num'], ['']));
  CATALOG.forEach((product, index) => {
    const quantity = state.cart.quantities[index];
    if (quantity === 0) return;
    const input = makeQuantityInput(quantity);
    input.addEventListener('change', () => changeCart(() => setQuantity(state.cart, index, Number(input.value))));
    const remove = makeButton('Remove', () => changeCart(() => setQuantity(state.cart, index, 0)), 'secondary');
    const tr = document.createElement('tr');
    tr.append(
      makeCell(product.name),
      makeCell(input, 'num'),
      makeCell(formatMoney(product.priceCents), 'num'),
      makeCell(formatMoney(quantity * product.priceCents), 'num'),
      makeCell(remove),
    );
    table.appendChild(tr);
  });
}

function renderTotals() {
  const dl = el('totals');
  const rows = [['Subtotal', formatMoney(subtotalCents(state.cart))]];
  if (state.cart.discount) {
    rows.push([`Discount (${state.cart.discount.code})`, formatMoney(-discountCents(state.cart))]);
  }
  rows.push(['Total', formatMoney(totalCents(state.cart))]);
  dl.replaceChildren();
  rows.forEach(([label, value], i) => {
    const last = i === rows.length - 1;
    const dt = document.createElement('dt');
    const dd = document.createElement('dd');
    dt.textContent = label;
    dd.textContent = value;
    if (last) {
      dt.className = 'grand';
      dd.className = 'grand';
    }
    dl.append(dt, dd);
  });
}

function renderSummary() {
  const section = el('summary');
  const order = state.order;
  section.hidden = order === null;
  if (order === null) return;

  const body = el('summary-body');
  body.replaceChildren();
  const info = [`Name: ${order.name}`, `Address: ${order.address}`];
  info.forEach((text) => {
    const p = document.createElement('p');
    p.textContent = text;
    body.appendChild(p);
  });

  const table = document.createElement('table');
  table.appendChild(headerRow(['Product'], ['Quantity', 'num'], ['Line total', 'num']));
  order.lines.forEach((line) => {
    const tr = document.createElement('tr');
    tr.append(makeCell(line.name), makeCell(String(line.quantity), 'num'), makeCell(formatMoney(line.cents), 'num'));
    table.appendChild(tr);
  });
  body.appendChild(table);

  const totals = document.createElement('p');
  totals.textContent = `Subtotal ${formatMoney(order.subtotal)}, discount ${formatMoney(-order.discount)}, ` +
    `total ${formatMoney(order.total)}.`;
  const thanks = document.createElement('p');
  thanks.textContent = 'Thank you for your order!';
  body.append(totals, thanks);
}

function render() {
  renderCart();
  renderTotals();
  renderSummary();
}

el('discount-form').addEventListener('submit', (event) => {
  event.preventDefault();
  try {
    applyCode(state.cart, el('code').value);
    showMessage('discount-message', 'Discount applied.');
  } catch (error) {
    showMessage('discount-message', error.message, true);
  }
  render();
});

el('checkout-form').addEventListener('submit', (event) => {
  event.preventDefault();
  const name = el('name').value.trim();
  const address = el('address').value.trim();
  try {
    if (isEmpty(state.cart)) throw new Error('Your cart is empty.');
    validateName(name);
    validateAddress(address);
  } catch (error) {
    showMessage('checkout-message', error.message, true);
    return;
  }

  const lines = CATALOG.map((product, i) => ({
    name: product.name,
    quantity: state.cart.quantities[i],
    cents: state.cart.quantities[i] * product.priceCents,
  })).filter((line) => line.quantity > 0);
  state.order = {
    name,
    address,
    lines,
    subtotal: subtotalCents(state.cart),
    discount: discountCents(state.cart),
    total: totalCents(state.cart),
  };
  clearCart(state.cart);
  el('checkout-form').reset();
  showMessage('checkout-message', 'Order placed.');
  showMessage('discount-message', '');
  showMessage('cart-message', '');
  render();
});

renderCatalog();
render();
