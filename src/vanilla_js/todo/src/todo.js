// Rules for tasks: validation, sorting, filtering, overdue check and JSON storage.
const PRIORITIES = ['low', 'medium', 'high'];
const MAX_TASKS = 200;
const MAX_TITLE_BYTES = 255;
const MAX_CATEGORY_BYTES = 63;

function isValidDate(text) {
  if (!/^\d{4}-\d{2}-\d{2}$/.test(text)) return false;
  const year = Number(text.slice(0, 4));
  const month = Number(text.slice(5, 7));
  const day = Number(text.slice(8));
  const leap = (year % 4 === 0 && year % 100 !== 0) || year % 400 === 0;
  const days = [31, leap ? 29 : 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31];
  return month >= 1 && month <= 12 && day >= 1 && day <= days[month - 1];
}

function isValidText(text, maxBytes, required) {
  if (typeof text !== 'string') return false;
  if (required && text === '') return false;
  return new TextEncoder().encode(text).length <= maxBytes && !/[\t\r\n]/.test(text);
}

function isValidTask(task) {
  return isValidText(task.title, MAX_TITLE_BYTES, true)
    && isValidText(task.category, MAX_CATEGORY_BYTES, false)
    && PRIORITIES.includes(task.priority)
    && (task.due === '' || isValidDate(task.due));
}

function isOverdue(task, today) {
  return !task.done && task.due !== '' && task.due < today;
}

function nextId(tasks) {
  return tasks.reduce((max, task) => Math.max(max, task.id), 0) + 1;
}

function formatDate(date) {
  const pad = (number) => String(number).padStart(2, '0');
  return `${date.getFullYear()}-${pad(date.getMonth() + 1)}-${pad(date.getDate())}`;
}

// Active before done, dated before undated, earlier dates first, high priority first.
function compareTasks(a, b) {
  if (a.done !== b.done) return a.done ? 1 : -1;
  if ((a.due === '') !== (b.due === '')) return a.due === '' ? 1 : -1;
  if (a.due !== b.due) return a.due < b.due ? -1 : 1;
  if (a.priority !== b.priority) {
    return PRIORITIES.indexOf(b.priority) - PRIORITIES.indexOf(a.priority);
  }
  return a.id - b.id;
}

function visibleTasks(tasks, { category = null, status = 'all' } = {}) {
  return tasks
    .filter((task) => category === null || task.category === category)
    .filter((task) => status === 'all' || (status === 'done') === task.done)
    .sort(compareTasks);
}

function tasksToJson(tasks) {
  return JSON.stringify(tasks);
}

function tasksFromJson(text) {
  const data = JSON.parse(text);
  if (!Array.isArray(data)) throw new Error('tasks must be a JSON array');
  for (const task of data) {
    const valid = typeof task === 'object' && task !== null
      && Number.isInteger(task.id) && task.id > 0
      && typeof task.done === 'boolean'
      && isValidTask(task);
    if (!valid) throw new Error('invalid task');
  }
  return data;
}

if (typeof module !== 'undefined') {
  module.exports = {
    MAX_TASKS,
    isValidDate,
    isValidTask,
    isOverdue,
    nextId,
    formatDate,
    visibleTasks,
    tasksToJson,
    tasksFromJson,
  };
}
