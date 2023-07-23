#!/bin/sh

# Author:  Osayamen Aimuyo
# Created: 6/18/2023.

BUILD_DIR=cmake-build-debug
APP=printer

. ./gen_executable.sh
gen_executable "$BUILD_DIR" "$APP" && "$BUILD_DIR"/"$APP"

cmake --build "$BUILD_DIR" --target clean