#!/usr/bin/env bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

if [[ ! -d "$SCRIPT_DIR/build/cpp" ]]; then
  bash ./configure-cpp.sh
fi

cmake --build "$SCRIPT_DIR/build/cpp" $*
