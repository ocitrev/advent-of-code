#!/usr/bin/env bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

zig build --prefix "$SCRIPT_DIR/build/zig" --cache-dir "$SCRIPT_DIR/build/.zig-cache" $*
