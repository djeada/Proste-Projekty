import unittest
import sys
import os
import tempfile
import shutil

sys.path.insert(
    0, os.path.abspath(os.path.join(os.path.dirname(__file__), "../src/logic"))
)
from repository import Repository, Commit, FileSnapshot


class TestRepository(unittest.TestCase):
    def setUp(self):
        self.test_dir = tempfile.mkdtemp()
        self.repo = Repository(self.test_dir)

    def tearDown(self):
        shutil.rmtree(self.test_dir)

    def test_init(self):
        result = self.repo.init()
        self.assertTrue(result)
        self.assertTrue(self.repo.is_initialized())

    def test_init_twice_fails(self):
        self.repo.init()
        result = self.repo.init()
        self.assertFalse(result)

    def test_add_file(self):
        # Create a test file
        test_file = os.path.join(self.test_dir, "test.txt")
        with open(test_file, "w") as f:
            f.write("Hello, world!")

        self.repo.init()
        result = self.repo.add_file("test.txt")
        self.assertTrue(result)
        self.assertIn("test.txt", self.repo.tracked_files)

    def test_add_nonexistent_file_fails(self):
        self.repo.init()
        result = self.repo.add_file("nonexistent.txt")
        self.assertFalse(result)

    def test_commit(self):
        test_file = os.path.join(self.test_dir, "test.txt")
        with open(test_file, "w") as f:
            f.write("Hello, world!")

        self.repo.init()
        self.repo.add_file("test.txt")
        commit_id = self.repo.commit("Initial commit")
        self.assertEqual(commit_id, 1)

    def test_get_commit(self):
        test_file = os.path.join(self.test_dir, "test.txt")
        with open(test_file, "w") as f:
            f.write("Hello, world!")

        self.repo.init()
        self.repo.add_file("test.txt")
        self.repo.commit("Initial commit")

        commit = self.repo.get_commit(1)
        self.assertIsNotNone(commit)
        self.assertEqual(commit.message, "Initial commit")

    def test_get_commit_count(self):
        self.repo.init()
        self.assertEqual(self.repo.get_commit_count(), 0)
        self.repo.commit("Commit 1")
        self.assertEqual(self.repo.get_commit_count(), 1)
        self.repo.commit("Commit 2")
        self.assertEqual(self.repo.get_commit_count(), 2)

    def test_status(self):
        self.repo.init()
        status = self.repo.status()
        self.assertIn("Repository", status)
        self.assertIn("Commits: 0", status)


if __name__ == "__main__":
    unittest.main()
