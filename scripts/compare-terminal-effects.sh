#!/bin/bash
# Runs every terminal effect in C, Python and JavaScript, checks that all three
# print exactly the same frames, and compares speed and code size.
# Usage: ./scripts/compare-terminal-effects.sh [frames]
set -e
FRAMES=${1:-200}
ROOT=$(cd "$(dirname "$0")/.." && pwd)
C=$ROOT/src/c/terminal_effects
PY=$ROOT/src/python/terminal_effects/src
JS=$ROOT/src/vanilla_js/terminal_effects/src

cmake -S "$C" -B "$C/build" > /dev/null
cmake --build "$C/build" > /dev/null
export NO_SLEEP=1

lines() { grep -cv '^\s*$' "$1"; }

# Prints the output's checksum and the average time per frame in milliseconds.
measure() {
    local start end sum
    start=$(date +%s%N)
    sum=$("$@" "$FRAMES" | sha256sum | cut -c1-16)
    end=$(date +%s%N)
    echo "$sum $(( (end - start) / FRAMES / 10000 ))"
}

ms() { printf "%d.%02d" $(($1 / 100)) $(($1 % 100)); }

printf "%-22s %7s %7s %7s   %9s %9s %9s   %s\n" effect "C" "Python" "JS" "C ms" "Python ms" "JS ms" "same output"
failed=0
for file in "$C"/src/*.c; do
    name=$(basename "$file" .c)
    [ "$name" = term ] && continue
    read -r c_sum c_t <<< "$(measure "$C/build/$name")"
    read -r py_sum py_t <<< "$(measure python3 "$PY/$name.py")"
    read -r js_sum js_t <<< "$(measure node "$JS/$name.js")"
    same=yes
    if [ "$c_sum" != "$py_sum" ] || [ "$c_sum" != "$js_sum" ]; then
        same=NO
        failed=1
    fi
    printf "%-22s %7d %7d %7d   %9s %9s %9s   %s\n" "$name" "$(lines "$file")" "$(lines "$PY/$name.py")" \
        "$(lines "$JS/$name.js")" "$(ms "$c_t")" "$(ms "$py_t")" "$(ms "$js_t")" "$same"
done
printf "%-22s %7d %7d %7d\n" "helpers (term)" "$(( $(lines "$C/src/term.c") + $(lines "$C/src/term.h") ))" \
    "$(lines "$PY/term.py")" "$(lines "$JS/term.js")"
exit $failed
