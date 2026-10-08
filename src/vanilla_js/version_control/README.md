# Version Control (JavaScript)

A tiny git-like version control tool for Node.js. It works on the current directory: `init` creates a hidden `.vcs/` folder, `commit` saves a copy of all regular files as a numbered commit, and `log`, `status`, `diff` and `checkout` let you see the history and go back to an earlier version.

![Screenshot](screenshot.png)

## Features
- Create a repository in any directory with `init`
- Save all files of the directory as a numbered commit with a one-line message
- List the commits, newest first
- Show which files were added, modified or deleted since the last commit
- Show the changed lines of each file compared with any commit
- Restore the files of any earlier commit

## How to use
Run the program inside the directory you want to version. Every command works on the current directory.

| Command | What it does |
|---|---|
| `version_control init` | create the `.vcs/` folder |
| `version_control commit "message"` | save all files as a new commit (the message must be one line) |
| `version_control log` | list the commits, newest first |
| `version_control status` | list the files added, modified or deleted since the last commit |
| `version_control diff [N]` | show the removed (`-`) and added (`+`) lines of each changed file, compared with commit N (default: the last commit) |
| `version_control checkout N` | write the files of commit N back into the directory |

In this README `version_control` stands for `node /path/to/src/main.js`.

### Example session
```
$ version_control init
Initialized empty repository in ./.vcs
$ version_control commit "first draft"
Created commit #1
$ version_control commit "bread and a call"
Created commit #2
$ version_control log
#2    2026-10-08 21:47:16  bread and a call
#1    2026-10-08 21:47:16  first draft
$ version_control status
Changes since commit #2:
  modified   notes.txt
$ version_control diff
== notes.txt (modified)
-Buy milk
+Buy oat milk
-Call Anna
+Call Anna before Friday
```

## How it works

### Repository format
The format is the same in the C, Python and JavaScript versions, so a repository created by one version can be used by the others.

```
.vcs/
└── commits/
    ├── 1/
    │   ├── meta            line 1: time of the commit (Unix seconds), line 2: the message
    │   └── files/
    │       └── notes.txt   byte-for-byte copy of the file at commit time
    └── 2/
        ├── meta
        └── files/
            ├── notes.txt
            └── plans.txt
```

- Commits are numbered 1, 2, 3, ... without gaps. Commit N exists when `commits/N/meta` exists.
- Only regular files directly inside the directory are saved. Subdirectories and `.vcs` are ignored.
- A message must be one line and shorter than 255 bytes.

### Commit, log, status and checkout
- `commit` reads every regular file of the directory and copies it to `commits/N/files/`, where N is the number of commits plus one. Then it writes `meta`.
- `log` reads the `meta` file of every commit and prints them in reverse order.
- `status` compares the current files with the last commit by name and contents. A file is *added* if the commit does not have it, *modified* if the bytes differ, and *deleted* if the commit has it but the directory does not.
- `checkout N` writes the files of commit N into the directory, overwriting files with the same name. It never deletes files, so files that are not in commit N stay. Changes that were not committed to those files are lost, so run `status` first.

### Diff algorithm
The `diff` command compares lines, using a longest common subsequence (LCS):

1. Each file is split into lines. The `\n` is not part of a line, and a last line without `\n` still counts.
2. A table `lcs[i][j]` is filled from the end: it holds the number of lines in the longest sequence that the old lines from `i` and the new lines from `j` have in common. A common line adds 1 to the cell diagonally below and to the right. A different line takes the larger of the cell below and the cell to the right.
3. The table is walked from the start. A line that is equal in both files is common and is not printed. Otherwise the walk removes an old line (`-`) or adds a new line (`+`), choosing the step that keeps the longer common sequence.

The table needs one cell for every pair of lines, so very large files are slow and use a lot of memory. A change that only adds or removes the final newline is not shown. The comparison decodes files as UTF-8, so `diff` is meant for text files.

### Project layout
```
version_control/
├── src/
│   ├── version_control.js   logic: snapshots, commits, checkout, LCS diff (no printing)
│   └── main.js              command line interface: parses arguments and prints
├── tests/
│   └── version_control.test.js   tests of the logic in a temporary directory
├── package.json             the test script (no dependencies)
├── .editorconfig            editor settings
├── screenshot.png
└── README.md
```

### How the program is structured
`main.js` reads the command and its arguments, calls one function from the logic and prints the result. The logic never prints, so the tests can check its results directly. Errors are thrown as `VcsError` and shown as a message, and the program exits with status 1. The program uses only the built-in `fs` and `path` modules (and `os` in the tests).

## Requirements
- Node.js 18 or newer
- No npm packages

## Run
```sh
cd /path/to/your/project
node /path/to/version_control/src/main.js init
node /path/to/version_control/src/main.js commit "first draft"
```

## Test
```sh
cd src/vanilla_js/version_control
npm test
```

## Comparison with the other versions
- [C version](../../c/version_control)
- [Python version](../../python/version_control)

| | C | Python | JavaScript |
|---|---|---|---|
| Interface | command line | command line | command line (Node.js) |
| Lines of logic | 397 | 107 | 158 |
| Lines of interface | 210 | 83 | 110 |
| Tests | 9 | 12 | 12 |

JavaScript uses `Map` objects of `Buffer` values for the snapshots, so the file contents need no manual memory handling. Errors are thrown as exceptions, and `fs.readdirSync` with `withFileTypes` tells regular files apart from folders without an extra `stat` call. The LCS diff is written with plain arrays, and it looks almost the same as the Python version. Its size is close to the Python logic, while the C version is about twice as large because it manages every buffer and array itself.

## Ideas for extensions
- Refuse `checkout` when there are uncommitted changes, unless a flag forces it
- Store each file once by its hash, so unchanged files are not copied again
- Add an ignore list (like `.gitignore`) for files that should not be saved
- Show a few unchanged lines around each change in `diff`
- Add `diff N M` to compare two commits
