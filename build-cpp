#!/usr/bin/env bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

if [[ ! -d "$SCRIPT_DIR/build/cpp" ]]; then
  ./configure-cpp.sh
fi

NINJA_STATUS='[%s/%t %p :: %e] ' cmake --build "$SCRIPT_DIR/build/cpp" "$@"
