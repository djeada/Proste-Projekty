// Page logic: reads the form, shows the result and updates it while typing.
const messageInput = document.getElementById('message');
const keyInput = document.getElementById('key');
const crackButton = document.getElementById('crack');
const resultOutput = document.getElementById('result');
const statusLine = document.getElementById('status');

function selectedMode() {
  return document.querySelector('input[name="mode"]:checked').value;
}

function update() {
  statusLine.textContent = '';
  if (keyInput.value === '' || !Number.isInteger(Number(keyInput.value))) {
    resultOutput.value = '';
    statusLine.textContent = 'The key must be a whole number.';
    return;
  }
  const key = Number(keyInput.value);
  const text = messageInput.value;
  resultOutput.value = selectedMode() === 'encrypt' ? encrypt(text, key) : decrypt(text, key);
}

function onCrack() {
  const { key, text } = crack(messageInput.value);
  keyInput.value = key;
  document.getElementById('mode-decrypt').checked = true;
  resultOutput.value = text;
  statusLine.textContent = `Most likely key: ${key}`;
}

messageInput.addEventListener('input', update);
keyInput.addEventListener('input', update);
for (const radio of document.querySelectorAll('input[name="mode"]')) {
  radio.addEventListener('change', update);
}
crackButton.addEventListener('click', onCrack);
update();
