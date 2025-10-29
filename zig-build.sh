#!/usr/bin/env bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

fatal() {
  echo "$@" 1>&2
  exit 1
}

command -v zig &>/dev/null || fatal Zig is not installed.

zig build --prefix "$SCRIPT_DIR/build/zig" --cache-dir "$SCRIPT_DIR/build/.zig-cache" "$@"
