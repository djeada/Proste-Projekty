const todoForm = document.querySelector('.todo-form');
const todoInput = document.getElementById('todo-input');
const todoList = document.getElementById('todo-list');
const taskCount = document.getElementById('task-count');
const clearCompletedBtn = document.getElementById('clear-completed');
const filterBtns = document.querySelectorAll('.filter-btn');

let tasks = [];
let currentFilter = 'all';
let nextId = 1;

function init() {
  loadFromStorage();
  render();
}

function loadFromStorage() {
  const stored = localStorage.getItem('todo-tasks');
  if (stored) {
    tasks = JSON.parse(stored);
    nextId = tasks.length > 0 ? Math.max(...tasks.map(t => t.id)) + 1 : 1;
  }
}

function saveToStorage() {
  localStorage.setItem('todo-tasks', JSON.stringify(tasks));
}

function addTask(title) {
  if (!title.trim()) return;
  
  const task = {
    id: nextId++,
    title: title.trim(),
    completed: false
  };
  
  tasks.push(task);
  saveToStorage();
  render();
}

function removeTask(id) {
  tasks = tasks.filter(task => task.id !== id);
  saveToStorage();
  render();
}

function toggleTask(id) {
  const task = tasks.find(task => task.id === id);
  if (task) {
    task.completed = !task.completed;
    saveToStorage();
    render();
  }
}

function clearCompleted() {
  tasks = tasks.filter(task => !task.completed);
  saveToStorage();
  render();
}

function setFilter(filter) {
  currentFilter = filter;
  filterBtns.forEach(btn => {
    btn.classList.toggle('active', btn.dataset.filter === filter);
  });
  render();
}

function getFilteredTasks() {
  switch (currentFilter) {
    case 'active':
      return tasks.filter(task => !task.completed);
    case 'completed':
      return tasks.filter(task => task.completed);
    default:
      return tasks;
  }
}

function render() {
  const filteredTasks = getFilteredTasks();
  
  todoList.innerHTML = '';
  
  filteredTasks.forEach(task => {
    const li = document.createElement('li');
    li.className = task.completed ? 'completed' : '';
    li.innerHTML = `
      <input type="checkbox" ${task.completed ? 'checked' : ''} />
      <span>${escapeHtml(task.title)}</span>
      <button class="remove-btn">×</button>
    `;
    
    const checkbox = li.querySelector('input[type="checkbox"]');
    checkbox.addEventListener('change', () => toggleTask(task.id));
    
    const removeBtn = li.querySelector('.remove-btn');
    removeBtn.addEventListener('click', () => removeTask(task.id));
    
    todoList.appendChild(li);
  });
  
  updateTaskCount();
}

function escapeHtml(text) {
  const div = document.createElement('div');
  div.textContent = text;
  return div.innerHTML;
}

function updateTaskCount() {
  const activeCount = tasks.filter(task => !task.completed).length;
  const totalCount = tasks.length;
  taskCount.textContent = `${activeCount} of ${totalCount} tasks remaining`;
}

todoForm.addEventListener('submit', (e) => {
  e.preventDefault();
  addTask(todoInput.value);
  todoInput.value = '';
});

clearCompletedBtn.addEventListener('click', clearCompleted);

filterBtns.forEach(btn => {
  btn.addEventListener('click', () => setFilter(btn.dataset.filter));
});

init();
