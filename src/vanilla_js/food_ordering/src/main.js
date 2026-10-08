// DOM: draws the menu, the order and the status, and reacts to clicks. Uses the rules in food_ordering.js.

let order = newOrder();
let selectedCategory = 'starter';
let statusTimer = null;

const $ = (id) => document.getElementById(id);

function renderAll() {
  renderCategories();
  renderDishes();
  renderOrder();
  renderDelivery();
  renderStatus();
}

function renderCategories() {
  $('category-tabs').innerHTML = Object.entries(CATEGORY_NAMES).map(([key, name]) =>
    `<button type="button" class="tab${key === selectedCategory ? ' active' : ''}" data-category="${key}">${name}</button>`,
  ).join('');
}

function renderDishes() {
  const locked = isPlaced(order);
  $('dishes').innerHTML = MENU.filter((dish) => dish.category === selectedCategory).map((dish) => {
    const full = (order.lines[dish.id] || 0) >= MAX_QUANTITY;
    return `<li class="dish">
      <div><strong>${dish.name}</strong><p>${dish.description}</p></div>
      <div class="dish-side">
        <span class="price">${formatMoney(dish.priceCents)}</span>
        <button type="button" data-add="${dish.id}" ${locked || full ? 'disabled' : ''}>Add</button>
      </div>
    </li>`;
  }).join('');
}

function renderOrder() {
  const locked = isPlaced(order);
  const dishIds = Object.keys(order.lines).map(Number).sort((a, b) => a - b);
  $('lines').innerHTML = dishIds.length === 0
    ? '<li class="empty">Your order is empty. Add a dish from the menu.</li>'
    : dishIds.map((id) => {
      const dish = dishById(id);
      const quantity = order.lines[id];
      return `<li>
        <span class="name">${dish.name}</span>
        <span class="quantity">
          <button type="button" data-change="${id}" data-delta="-1" ${locked ? 'disabled' : ''}>&minus;</button>
          <span>${quantity}</span>
          <button type="button" data-change="${id}" data-delta="1" ${locked || quantity >= MAX_QUANTITY ? 'disabled' : ''}>+</button>
        </span>
        <span class="price">${formatMoney(dish.priceCents * quantity)}</span>
        <button type="button" class="remove" data-remove="${id}" ${locked ? 'disabled' : ''}>Remove</button>
      </li>`;
    }).join('');
  $('total').textContent = formatMoney(totalCents(order));
}

function renderDelivery() {
  const locked = isPlaced(order);
  const cardSelected = document.querySelector('input[name="payment"]:checked')?.value === 'card';
  $('card-box').hidden = !cardSelected;
  for (const input of document.querySelectorAll('#delivery input, #delivery button')) {
    input.disabled = locked;
  }
}

function renderStatus() {
  $('status-section').hidden = !isPlaced(order);
  if (!isPlaced(order)) return;
  const elapsed = (Date.now() - order.placedAt) / 1000;
  const current = statusAt(elapsed);
  const delivered = current === STATUS_NAMES.length - 1;
  $('steps').innerHTML = STATUS_NAMES.map((name, index) => {
    const state = index < current ? 'done' : index === current ? 'current' : '';
    return `<li class="${state}">${name}</li>`;
  }).join('');
  $('elapsed').textContent = delivered
    ? 'Delivered. Enjoy your meal!'
    : `${Math.floor(elapsed)} seconds since the order was placed.`;
  $('new-order').hidden = !delivered;
  if (delivered) {
    clearInterval(statusTimer);
    statusTimer = null;
  } else if (statusTimer === null) {
    statusTimer = setInterval(renderStatus, 1000);
  }
}

function showError(id, text) {
  $(id).textContent = text;
}

// Checks the delivery fields and the card, then places the order if everything is valid.
function placeClicked() {
  const addressText = $('address').value;
  const phoneText = $('phone').value;
  const payment = document.querySelector('input[name="payment"]:checked')?.value || null;
  const cardText = $('card').value;
  showError('address-error', '');
  showError('phone-error', '');
  showError('card-error', '');
  showError('place-error', '');

  let valid = true;
  if (validateAddress(addressText) === null) {
    showError('address-error', 'Address must be 5-100 characters, with a street name and a house number.');
    valid = false;
  }
  if (validatePhone(phoneText) === null) {
    showError('phone-error', 'Phone number must contain 9-15 digits.');
    valid = false;
  }
  if (payment === 'card' && !luhnIsValid(cardText)) {
    showError('card-error', 'Invalid card: it needs 13-19 digits and must pass the Luhn check.');
    valid = false;
  }
  if (!valid) return;

  setAddress(order, addressText);
  setPhone(order, phoneText);
  if (payment === 'card') setCard(order, cardText);
  else if (payment) setPayment(order, payment);

  const error = placeOrder(order, Date.now());
  if (error) {
    showError('place-error', error);
    return;
  }
  renderAll();
}

function startNewOrder() {
  order = newOrder();
  for (const id of ['address', 'phone', 'card']) $(id).value = '';
  document.querySelectorAll('input[name="payment"]').forEach((radio) => { radio.checked = false; });
  renderAll();
}

function init() {
  $('category-tabs').addEventListener('click', (event) => {
    const tab = event.target.closest('[data-category]');
    if (tab) {
      selectedCategory = tab.dataset.category;
      renderAll();
    }
  });
  $('dishes').addEventListener('click', (event) => {
    const button = event.target.closest('[data-add]');
    if (button) {
      addDish(order, Number(button.dataset.add));
      renderAll();
    }
  });
  $('lines').addEventListener('click', (event) => {
    const change = event.target.closest('[data-change]');
    const remove = event.target.closest('[data-remove]');
    if (change) {
      const id = Number(change.dataset.change);
      setQuantity(order, id, order.lines[id] + Number(change.dataset.delta));
    } else if (remove) {
      setQuantity(order, Number(remove.dataset.remove), 0);
    }
    renderAll();
  });
  document.querySelectorAll('input[name="payment"]').forEach((radio) => {
    radio.addEventListener('change', renderDelivery);
  });
  $('place').addEventListener('click', placeClicked);
  $('new-order').addEventListener('click', startNewOrder);
  renderAll();
}

if (typeof document !== 'undefined') init();
