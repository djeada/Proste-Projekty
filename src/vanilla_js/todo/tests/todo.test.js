const { test } = require('node:test');
const assert = require('node:assert/strict');
const {
  isValidDate, isValidTask, isOverdue, nextId, formatDate,
  visibleTasks, tasksToJson, tasksFromJson,
} = require('../src/todo.js');

const TODAY = '2026-10-08';

function task(id, title, extra = {}) {
  return { id, title, priority: 'medium', due: '', category: '', done: false, ...extra };
}

test('dates are checked against the calendar', () => {
  assert.equal(isValidDate('2026-10-20'), true);
  assert.equal(isValidDate('2028-02-29'), true);
  assert.equal(isValidDate('2026-02-29'), false);
  assert.equal(isValidDate('2100-02-29'), false);
  assert.equal(isValidDate('2026-13-01'), false);
  assert.equal(isValidDate('20-10-2026'), false);
  assert.equal(isValidDate('2026-1-5'), false);
});

test('invalid tasks are rejected', () => {
  assert.equal(isValidTask(task(1, 'Buy milk')), true);
  assert.equal(isValidTask(task(1, '')), false);
  assert.equal(isValidTask(task(1, 'Buy\tmilk')), false);
  assert.equal(isValidTask(task(1, 'Buy milk', { category: 'home\nwork' })), false);
  assert.equal(isValidTask(task(1, 'Buy milk', { priority: 'urgent' })), false);
  assert.equal(isValidTask(task(1, 'Buy milk', { due: '2026-02-30' })), false);
  assert.equal(isValidTask(task(1, 'x'.repeat(256))), false);
});

test('overdue means not done and due before today', () => {
  assert.equal(isOverdue(task(1, 'Pay rent', { due: '2026-10-01' }), TODAY), true);
  assert.equal(isOverdue(task(1, 'Pay rent', { due: TODAY }), TODAY), false);
  assert.equal(isOverdue(task(1, 'Pay rent', { due: '2026-10-01', done: true }), TODAY), false);
  assert.equal(isOverdue(task(1, 'Read'), TODAY), false);
});

test('visible tasks are sorted by done, due date, then priority', () => {
  const tasks = [
    task(1, 'No date low', { priority: 'low' }),
    task(2, 'Later high', { priority: 'high', due: '2026-12-01' }),
    task(3, 'Soon low', { priority: 'low', due: '2026-10-09' }),
    task(4, 'Soon high', { priority: 'high', due: '2026-10-09' }),
    task(5, 'Finished', { priority: 'high', due: '2026-01-01', done: true }),
    task(6, 'No date high', { priority: 'high' }),
  ];
  const titles = visibleTasks(tasks).map((item) => item.title);
  assert.deepEqual(titles, [
    'Soon high', 'Soon low', 'Later high', 'No date high', 'No date low', 'Finished',
  ]);
});

test('tasks can be filtered by category and status', () => {
  const tasks = [
    task(1, 'Report', { category: 'work' }),
    task(2, 'Dishes', { category: 'home', done: true }),
  ];
  assert.deepEqual(visibleTasks(tasks, { category: 'work' }).map((t) => t.title), ['Report']);
  assert.deepEqual(visibleTasks(tasks, { category: 'home', status: 'active' }), []);
  assert.deepEqual(visibleTasks(tasks, { status: 'done' }).map((t) => t.title), ['Dishes']);
  assert.equal(visibleTasks(tasks, { status: 'all' }).length, 2);
});

test('ids and dates are computed', () => {
  assert.equal(nextId([]), 1);
  assert.equal(nextId([task(7, 'a'), task(2, 'b')]), 8);
  assert.equal(formatDate(new Date(2026, 0, 5)), '2026-01-05');
});

test('tasks survive a JSON round trip and bad data is refused', () => {
  const tasks = [task(1, 'Buy milk', { priority: 'high', due: '2026-10-20', category: 'home' })];
  assert.deepEqual(tasksFromJson(tasksToJson(tasks)), tasks);
  assert.throws(() => tasksFromJson('{"id": 1}'));
  assert.throws(() => tasksFromJson('[{"id": 0, "title": "x", "priority": "low", "due": "", "category": "", "done": false}]'));
  assert.throws(() => tasksFromJson('[null]'));
});
