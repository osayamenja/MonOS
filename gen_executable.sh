#!/bin/sh

# Author:  Osayamen Aimuyo
# Created: 6/17/2023.

gen_executable ()
{
  BUILD_DIR="$1"
  APP="$2"

  cmake -DCMAKE_BUILD_TYPE=Debug -G 'CodeBlocks - Unix Makefiles' -S . -B "$BUILD_DIR"
  cmake --build "$BUILD_DIR" --target "$APP"

  chmod a+x "$BUILD_DIR"/"$APP"
}
