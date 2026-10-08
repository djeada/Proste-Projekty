// The page: the text area, the toolbar, keyboard shortcuts, the status bar and saving.
const editor = document.getElementById('editor');
const statusBar = document.getElementById('status-bar');
const fileInput = document.getElementById('file-input');

let fileName = '';
let savedText = '';
let needle = '';
let message = '';

function isModified() {
  return editor.value !== savedText;
}

function updateStatus() {
  const { line, column } = lineCol(editor.value, editor.selectionStart);
  const name = fileName || '(new file)';
  const modified = isModified() ? ' (modified)' : '';
  const info = `${name}${modified} | Ln ${line}, Col ${column} | ` +
    `${countLines(editor.value)} lines, ${countWords(editor.value)} words`;
  statusBar.textContent = message ? `${message} | ${info}` : info;
  document.title = `${isModified() ? '*' : ''}${name} - Text editor`;
}

function confirmDiscard() {
  return !isModified() || window.confirm('Discard the unsaved changes?');
}

function newFile() {
  if (!confirmDiscard()) {
    return;
  }
  fileName = '';
  savedText = '';
  editor.value = '';
  message = 'New file';
  updateStatus();
}

function openFile(file) {
  const reader = new FileReader();
  reader.onload = () => {
    fileName = file.name;
    savedText = reader.result;
    editor.value = reader.result;
    editor.setSelectionRange(0, 0);
    message = `Opened ${file.name}`;
    updateStatus();
  };
  reader.onerror = () => {
    message = `Cannot read ${file.name}`;
    updateStatus();
  };
  reader.readAsText(file);
}

function chooseFile() {
  if (confirmDiscard()) {
    fileInput.click();
  }
}

// Saving downloads the text as a file, because a web page cannot write to the disk.
function save() {
  const name = fileName || window.prompt('File name:', 'untitled.txt');
  if (!name) {
    return;
  }
  const url = URL.createObjectURL(new Blob([editor.value], { type: 'text/plain;charset=utf-8' }));
  const link = document.createElement('a');
  link.href = url;
  link.download = name;
  link.click();
  setTimeout(() => URL.revokeObjectURL(url), 1000);
  fileName = name;
  savedText = editor.value;
  message = `Saved ${name}`;
  updateStatus();
}

function findAgain() {
  if (!needle) {
    find();
    return;
  }
  const cursor = editor.selectionStart;
  const index = findNext(editor.value, needle, cursor + 1);
  if (index === -1) {
    message = `Not found: ${needle}`;
  } else {
    editor.focus();
    editor.setSelectionRange(index, index + needle.length);
    message = index <= cursor ? 'Found (wrapped around to the start)' : 'Found';
  }
  updateStatus();
}

function find() {
  const text = window.prompt('Text to find:', needle);
  if (text) {
    needle = text;
    findAgain();
  }
}

function goToLine() {
  const last = countLines(editor.value);
  const line = Number(window.prompt(`Line (1-${last}):`));
  const offset = Number.isInteger(line) && line >= 1 && line <= last ? lineStart(editor.value, line) : -1;
  if (offset === -1) {
    message = `Enter a line from 1 to ${last}`;
  } else {
    editor.focus();
    editor.setSelectionRange(offset, offset);
    message = '';
  }
  updateStatus();
}

// Tab inserts four spaces, as in the C and Python versions, instead of moving the focus.
function insertIndent() {
  editor.focus();
  document.execCommand('insertText', false, '    ');
}

document.getElementById('new-button').addEventListener('click', newFile);
document.getElementById('open-button').addEventListener('click', chooseFile);
document.getElementById('save-button').addEventListener('click', save);
document.getElementById('find-button').addEventListener('click', find);
document.getElementById('goto-button').addEventListener('click', goToLine);
fileInput.addEventListener('change', () => {
  if (fileInput.files.length > 0) {
    openFile(fileInput.files[0]);
  }
  fileInput.value = '';
});

document.addEventListener('keydown', (event) => {
  const command = event.ctrlKey || event.metaKey;
  const key = event.key.toLowerCase();
  if (command && key === 's') {
    event.preventDefault();
    save();
  } else if (command && key === 'f') {
    event.preventDefault();
    find();
  } else if (command && key === 'g') {
    event.preventDefault();
    goToLine();
  } else if (event.key === 'F3') {
    event.preventDefault();
    findAgain();
  } else if (event.key === 'Tab' && !event.ctrlKey && !event.metaKey && !event.altKey) {
    event.preventDefault();
    insertIndent();
  }
});

editor.addEventListener('input', () => {
  message = '';
  updateStatus();
});
editor.addEventListener('keyup', updateStatus);
editor.addEventListener('click', updateStatus);

window.addEventListener('beforeunload', (event) => {
  if (isModified()) {
    event.preventDefault();
    event.returnValue = '';
  }
});

updateStatus();
editor.focus();
