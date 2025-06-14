#!/bin/bash
set -e
set -x
ROOT_DIR=$(pwd)
for proj in $(ls -1 src/vanilla_js); do
  echo "Testing project: $proj"
  if [ ! -d src/vanilla_js/$proj ]; then
    echo "Directory src/vanilla_js/$proj does not exist!"
    exit 1
  fi
  cd "$ROOT_DIR/src/vanilla_js/$proj"
  if [ -f package.json ]; then
    npm install
    npm test || exit 1
  else
    echo "No package.json in $proj, skipping."
  fi
  cd "$ROOT_DIR"
done
