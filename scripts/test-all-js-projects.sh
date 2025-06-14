#!/bin/bash
set -e
set -x
ROOT_DIR=$(pwd)
if [ "$#" -eq 0 ]; then
  PROJECT_PATHS=$(ls -d src/vanilla_js/*)
else
  PROJECT_PATHS="$@"
fi
for proj_path in $PROJECT_PATHS; do
  echo "Testing project: $proj_path"
  if [ ! -d "$proj_path" ]; then
    echo "Directory $proj_path does not exist!"
    exit 1
  fi
  cd "$ROOT_DIR/$proj_path"
  if [ -f package.json ]; then
    npm install
    npm test || exit 1
  else
    echo "No package.json in $proj_path, skipping."
  fi
  cd "$ROOT_DIR"
done
