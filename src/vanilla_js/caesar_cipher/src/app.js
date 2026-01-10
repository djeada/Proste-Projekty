const messageInput = document.getElementById('message');
const keyInput = document.getElementById('key');
const modeSelect = document.getElementById('mode');
const resultOutput = document.getElementById('result');
const runBtn = document.getElementById('run');
const copyBtn = document.getElementById('copy');

function caesarEncrypt(text, key) {
  let result = '';
  
  for (const char of text) {
    if (char.match(/[A-Z]/)) {
      const offset = 'A'.charCodeAt(0);
      const shifted = (char.charCodeAt(0) - offset + key) % 26;
      result += String.fromCharCode((shifted + 26) % 26 + offset);
    } else if (char.match(/[a-z]/)) {
      const offset = 'a'.charCodeAt(0);
      const shifted = (char.charCodeAt(0) - offset + key) % 26;
      result += String.fromCharCode((shifted + 26) % 26 + offset);
    } else {
      result += char;
    }
  }
  
  return result;
}

function caesarDecrypt(text, key) {
  return caesarEncrypt(text, -key);
}

function run() {
  const message = messageInput.value;
  const key = parseInt(keyInput.value, 10) || 0;
  const mode = modeSelect.value;
  
  if (mode === 'encrypt') {
    resultOutput.value = caesarEncrypt(message, key);
  } else {
    resultOutput.value = caesarDecrypt(message, key);
  }
}

function copyToClipboard() {
  resultOutput.select();
  resultOutput.setSelectionRange(0, 99999);
  navigator.clipboard.writeText(resultOutput.value);
  
  const originalText = copyBtn.textContent;
  copyBtn.textContent = 'Copied!';
  setTimeout(() => {
    copyBtn.textContent = originalText;
  }, 1500);
}

runBtn.addEventListener('click', run);
copyBtn.addEventListener('click', copyToClipboard);

messageInput.addEventListener('keydown', (e) => {
  if (e.key === 'Enter' && e.ctrlKey) {
    run();
  }
});
