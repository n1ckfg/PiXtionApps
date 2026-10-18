#!/bin/bash

SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
  DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )"
  SOURCE="$(readlink "$SOURCE")"
  [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE" # if $SOURCE was a relative symlink, we need to resolve it relative to the path where the symlink file was located
done
DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )"

cd "$DIR"

# don't use make clean here: it deletes the tracked OpenNI2 libs in bin/libs
FAILED=()

for APP in PiXtion_*/; do
  APP="${APP%/}"
  [ -f "$APP/Makefile" ] || continue

  echo "=== Building $APP ==="
  if ! (cd "$APP" && make -j"$(nproc)" "$@"); then
    FAILED+=("$APP")
  fi
done

if [ ${#FAILED[@]} -ne 0 ]; then
  echo "=== Failed: ${FAILED[*]} ==="
  exit 1
fi

echo "=== All apps built ==="
