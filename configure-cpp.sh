#!/usr/bin/env bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

fatal() {
  echo "$@" 1>&2
  exit 1
}

# check pre-requisites
command -v cmake &>/dev/null || fatal cmake not found!
command -v ninja &>/dev/null || fatal ninja not found!
[[ ! -z $VCPKG_ROOT ]] || fatal VCPKG_ROOT is not set

if [[ -d "$SCRIPT_DIR/build/cpp" ]]; then
  rm -r "$SCRIPT_DIR/build/cpp"
fi

cmake -G 'Ninja Multi-Config' -S "$SCRIPT_DIR/." -B "$SCRIPT_DIR/build/cpp"
