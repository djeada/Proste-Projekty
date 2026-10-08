// Page logic: the form, the filters and the list. Tasks are kept in localStorage.
const STORAGE_KEY = 'todo.tasks';

const form = document.getElementById('task-form');
const titleInput = document.getElementById('title');
const dueInput = document.getElementById('due');
const priorityInput = document.getElementById('priority');
const categoryInput = document.getElementById('category');
const saveButton = document.getElementById('save-button');
const cancelButton = document.getElementById('cancel-button');
const messageText = document.getElementById('message');
const categoryFilter = document.getElementById('filter-category');
const statusFilter = document.getElementById('filter-status');
const taskList = document.getElementById('task-list');
const emptyText = document.getElementById('empty');

let tasks = loadTasks();
let editingId = null;

function loadTasks() {
  try {
    const saved = localStorage.getItem(STORAGE_KEY);
    return saved === null ? [] : tasksFromJson(saved);
  } catch (error) {
    return [];
  }
}

function saveTasks() {
  try {
    localStorage.setItem(STORAGE_KEY, tasksToJson(tasks));
  } catch (error) {
    showMessage('The browser did not save the tasks.');
  }
}

function showMessage(text) {
  messageText.textContent = text;
}

function resetForm() {
  editingId = null;
  form.reset();
  saveButton.textContent = 'Add task';
  cancelButton.hidden = true;
}

function startEditing(task) {
  editingId = task.id;
  titleInput.value = task.title;
  dueInput.value = task.due;
  priorityInput.value = task.priority;
  categoryInput.value = task.category;
  saveButton.textContent = 'Save changes';
  cancelButton.hidden = false;
  showMessage('');
  titleInput.focus();
}

function createTaskFromForm() {
  const current = tasks.find((task) => task.id === editingId);
  return {
    id: current ? current.id : nextId(tasks),
    title: titleInput.value,
    priority: priorityInput.value,
    due: dueInput.value,
    category: categoryInput.value,
    done: current ? current.done : false,
  };
}

function handleSubmit(event) {
  event.preventDefault();
  const task = createTaskFromForm();
  if (!isValidTask(task)) {
    showMessage('Check the title, the category (up to 63 bytes) and the due date.');
    return;
  }
  if (editingId === null && tasks.length >= MAX_TASKS) {
    showMessage(`The list is full (${MAX_TASKS} tasks).`);
    return;
  }
  if (editingId === null) {
    tasks = [...tasks, task];
  } else {
    tasks = tasks.map((item) => (item.id === task.id ? task : item));
  }
  resetForm();
  saveTasks();
  render();
}

function deleteTask(id) {
  tasks = tasks.filter((task) => task.id !== id);
  if (editingId === id) resetForm();
  saveTasks();
  render();
}

function toggleTask(id, done) {
  tasks = tasks.map((task) => (task.id === id ? { ...task, done } : task));
  saveTasks();
  render();
}

function createSpan(text, className = '') {
  const span = document.createElement('span');
  span.textContent = text;
  span.className = className;
  return span;
}

function createButton(text, action, id, className = '') {
  const button = document.createElement('button');
  button.type = 'button';
  button.textContent = text;
  button.className = className;
  button.dataset.action = action;
  button.dataset.id = String(id);
  return button;
}

function createTaskItem(task, today) {
  const item = document.createElement('li');
  const overdue = isOverdue(task, today);
  item.className = `task${task.done ? ' done' : ''}${overdue ? ' overdue' : ''}`;

  const checkbox = document.createElement('input');
  checkbox.type = 'checkbox';
  checkbox.checked = task.done;
  checkbox.setAttribute('aria-label', `Mark "${task.title}" as done`);
  checkbox.dataset.action = 'toggle';
  checkbox.dataset.id = String(task.id);

  const main = document.createElement('div');
  main.className = 'task-main';
  const title = document.createElement('div');
  title.className = 'title';
  title.textContent = task.title;

  const meta = document.createElement('div');
  meta.className = 'meta';
  meta.append(createSpan(task.priority, `priority-${task.priority}`));
  if (task.due !== '') meta.append(createSpan(`Due ${task.due}`));
  if (task.category !== '') meta.append(createSpan(task.category));
  if (overdue) meta.append(createSpan('Overdue', 'overdue-badge'));
  main.append(title, meta);

  const actions = document.createElement('div');
  actions.className = 'actions';
  actions.append(
    createButton('Edit', 'edit', task.id, 'secondary'),
    createButton('Delete', 'delete', task.id, 'delete'),
  );

  item.append(checkbox, main, actions);
  return item;
}

function refreshCategoryOptions() {
  const selected = categoryFilter.value;
  const names = [...new Set(tasks.map((task) => task.category).filter((name) => name !== ''))];
  names.sort();
  categoryFilter.replaceChildren(new Option('All categories', ''));
  for (const name of names) {
    categoryFilter.append(new Option(name, name));
  }
  categoryFilter.value = names.includes(selected) ? selected : '';
}

function render() {
  refreshCategoryOptions();
  const category = categoryFilter.value === '' ? null : categoryFilter.value;
  const visible = visibleTasks(tasks, { category, status: statusFilter.value });
  const today = formatDate(new Date());
  taskList.replaceChildren(...visible.map((task) => createTaskItem(task, today)));
  emptyText.hidden = visible.length > 0;
}

function handleListClick(event) {
  const button = event.target.closest('button');
  if (!button) return;
  const id = Number(button.dataset.id);
  const task = tasks.find((item) => item.id === id);
  if (button.dataset.action === 'edit' && task) startEditing(task);
  if (button.dataset.action === 'delete') deleteTask(id);
}

function handleListChange(event) {
  if (event.target.dataset.action === 'toggle') {
    toggleTask(Number(event.target.dataset.id), event.target.checked);
  }
}

form.addEventListener('submit', handleSubmit);
cancelButton.addEventListener('click', () => {
  resetForm();
  showMessage('');
});
taskList.addEventListener('click', handleListClick);
taskList.addEventListener('change', handleListChange);
categoryFilter.addEventListener('change', render);
statusFilter.addEventListener('change', render);

render();
