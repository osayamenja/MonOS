#!/bin/sh

# Author:  Osayamen Aimuyo
# Created: 6/17/2023.

BUILD_DIR=cmake-build-debug
APP=computer
CID="$1"
IN_FILE="$2"
OUT_FILE="$3"

. ./gen_executable.sh
gen_executable "$BUILD_DIR" "$APP" &&

# Source: https://stackoverflow.com/a/3951175
case "$CID" in
    ''|*[!0-9]*) echo "Computer ID not specified or not non-negative integer" && exit 1 ;;
    *) ;;
esac

if [ -z "$IN_FILE" ] && [ -z "$OUT_FILE" ]
  then
    "$BUILD_DIR"/"$APP" "$CID"
elif [ -n "$IN_FILE" ]
  then
    "$BUILD_DIR"/"$APP" "$CID" < "$IN_FILE"
else
  "$BUILD_DIR"/"$APP" "$CID" < "$IN_FILE" >> "$OUT_FILE"
fi

cmake --build "$BUILD_DIR" --target clean