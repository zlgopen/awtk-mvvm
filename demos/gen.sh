#!/bin/sh
set -eu

cd "$(dirname "$0")"

for gen in */gen.sh; do
  [ -f "$gen" ] || continue
  demo=${gen%/gen.sh}
  printf 'generate %s\n' "$demo"
  (cd "$demo" && sh ./gen.sh)
done
