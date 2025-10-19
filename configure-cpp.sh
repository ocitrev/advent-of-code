#!/usr/bin/env bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
rm -r "$SCRIPT_DIR/build/cpp"
cmake -G 'Ninja Multi-Config' -S "$SCRIPT_DIR/." -B "$SCRIPT_DIR/build/cpp"
