// Command line interface of the version control tool. Works on the current directory.
'use strict';

const vcs = require('./version_control');

const USAGE = `Usage: version_control <command> [argument]

Commands:
  init               create a repository in this directory
  commit "message"   save all files as a new commit
  log                list commits, newest first
  status             show files changed since the last commit
  diff [N]           show changed lines compared with commit N (default: last)
  checkout N         restore the files of commit N
`;

function parseNumber(text) {
  if (!/^\d+$/.test(text) || Number(text) < 1) {
    throw new vcs.VcsError('Commit number must be a positive integer.');
  }
  return Number(text);
}

function showLog() {
  const count = vcs.commitCount('.');
  if (count === 0) {
    console.log('No commits yet.');
  }
  for (let number = count; number >= 1; number--) {
    const { time, message } = vcs.commitInfo('.', number);
    console.log(`#${String(number).padEnd(4)} ${vcs.formatTime(time)}  ${message}`);
  }
}

function showStatus() {
  const count = vcs.commitCount('.');
  const base = count > 0 ? vcs.loadCommit('.', count) : new Map();
  console.log(count === 0 ? 'No commits yet.' : `Changes since commit #${count}:`);
  const found = vcs.changes(base, vcs.readDirectory('.'));
  if (found.length === 0) {
    console.log('  nothing changed');
  }
  for (const { kind, name } of found) {
    console.log(`  ${kind.padEnd(10)} ${name}`);
  }
}

function showDiff(params) {
  const count = vcs.commitCount('.');
  if (count === 0) {
    throw new vcs.VcsError('No commits yet.');
  }
  const number = params.length > 0 ? parseNumber(params[0]) : count;
  if (number > count) {
    throw new vcs.VcsError(`There is no commit #${number}.`);
  }

  const base = vcs.loadCommit('.', number);
  const current = vcs.readDirectory('.');
  const found = vcs.changes(base, current);
  if (found.length === 0) {
    console.log(`No changes since commit #${number}.`);
  }
  for (const { kind, name } of found) {
    const oldLines = kind === 'added' ? [] : vcs.splitLines(base.get(name));
    const newLines = kind === 'deleted' ? [] : vcs.splitLines(current.get(name));
    console.log(`== ${name} (${kind})`);
    for (const { tag, line } of vcs.diffLines(oldLines, newLines)) {
      if (tag !== ' ') {
        console.log(tag + line);
      }
    }
  }
}

// Runs one command and returns the exit code.
function run(args) {
  const [command, ...params] = args;
  if (command === 'init' && params.length === 0) {
    vcs.init('.');
    console.log(`Initialized empty repository in ./${vcs.VCS_DIR}`);
  } else if (command === 'commit' && params.length === 1) {
    console.log(`Created commit #${vcs.commit('.', params[0])}`);
  } else if (command === 'log' && params.length === 0) {
    showLog();
  } else if (command === 'status' && params.length === 0) {
    showStatus();
  } else if (command === 'diff' && params.length <= 1) {
    showDiff(params);
  } else if (command === 'checkout' && params.length === 1) {
    const number = parseNumber(params[0]);
    vcs.checkout('.', number);
    console.log(`Restored the files of commit #${number}`);
  } else {
    process.stdout.write(USAGE);
    return 1;
  }
  return 0;
}

function main() {
  const args = process.argv.slice(2);
  if (args.length === 0) {
    process.stdout.write(USAGE);
    process.exitCode = 1;
    return;
  }
  try {
    process.exitCode = run(args);
  } catch (error) {
    if (!(error instanceof vcs.VcsError)) {
      throw error;
    }
    console.log(error.message);
    process.exitCode = 1;
  }
}

if (require.main === module) {
  main();
}
