#!/bin/bash
set -e
set -x
ROOT_DIR=$(pwd)
if [ "$#" -eq 0 ]; then
  PROJECT_PATHS=$(ls -d src/c/*)
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
  cmake -S . -B build
  cmake --build build
  cd build && ctest --output-on-failure || exit 1
  cd "$ROOT_DIR"
done
