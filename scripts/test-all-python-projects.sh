#!/bin/bash
set -e
set -x
ROOT_DIR=$(pwd)
for proj in $(ls -1 src/python); do
  echo "Testing project: $proj"
  if [ ! -d src/python/$proj ]; then
    echo "Directory src/python/$proj does not exist!"
    exit 1
  fi
  cd "$ROOT_DIR/src/python/$proj"
  if [ -f requirements.txt ]; then
    pip install -r requirements.txt
  fi
  if [ -d src ]; then
    pytest src || exit 1
  elif [ -d tests ]; then
    pytest tests || exit 1
  else
    echo "No test directory found for $proj"
    exit 1
  fi
  cd "$ROOT_DIR"
done
