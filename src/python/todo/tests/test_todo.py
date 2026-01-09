import unittest
import sys
import os

sys.path.insert(
    0, os.path.abspath(os.path.join(os.path.dirname(__file__), "../src/logic"))
)
from todo_list import TodoList


class TestTodoList(unittest.TestCase):
    def test_add_task(self):
        todo = TodoList()
        result = todo.add("Buy groceries")
        self.assertTrue(result)
        self.assertEqual(todo.count(), 1)

    def test_remove_task(self):
        todo = TodoList()
        todo.add("Task 1")
        todo.add("Task 2")
        result = todo.remove(0)
        self.assertTrue(result)
        self.assertEqual(todo.count(), 1)
        self.assertEqual(todo.get(0), "Task 2")

    def test_remove_invalid_index(self):
        todo = TodoList()
        todo.add("Task 1")
        result = todo.remove(5)
        self.assertFalse(result)
        self.assertEqual(todo.count(), 1)

    def test_get_task(self):
        todo = TodoList()
        todo.add("Test task")
        self.assertEqual(todo.get(0), "Test task")

    def test_get_invalid_index(self):
        todo = TodoList()
        self.assertEqual(todo.get(0), "")

    def test_count(self):
        todo = TodoList()
        self.assertEqual(todo.count(), 0)
        todo.add("Task 1")
        todo.add("Task 2")
        self.assertEqual(todo.count(), 2)

    def test_clear(self):
        todo = TodoList()
        todo.add("Task 1")
        todo.add("Task 2")
        todo.clear()
        self.assertEqual(todo.count(), 0)

    def test_print_all_empty(self):
        todo = TodoList()
        self.assertEqual(todo.print_all(), "No tasks.")

    def test_print_all_with_tasks(self):
        todo = TodoList()
        todo.add("Task 1")
        todo.add("Task 2")
        output = todo.print_all()
        self.assertIn("1: Task 1", output)
        self.assertIn("2: Task 2", output)


if __name__ == "__main__":
    unittest.main()
