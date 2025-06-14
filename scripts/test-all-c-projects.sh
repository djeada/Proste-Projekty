#!/bin/bash
set -e
set -x
ROOT_DIR=$(pwd)
if [ "$#" -eq 0 ]; then
  PROJECTS=$(ls -1 src/c)
else
  PROJECTS="$@"
fi
for proj in $PROJECTS; do
  echo "Testing project: $proj"
  if [ ! -d src/c/$proj ]; then
    echo "Directory src/c/$proj does not exist!"
    exit 1
  fi
  cd "$ROOT_DIR/src/c/$proj"
  cmake -S . -B build
  cmake --build build
  cd build && ctest --output-on-failure || exit 1
  cd "$ROOT_DIR"
done
