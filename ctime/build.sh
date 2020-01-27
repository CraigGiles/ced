#!/bin/bash

CODE_PATH="$(dirname "$0")"
pushd "$CODE_PATH"

# Build ctime
cc -O2 -Wno-unused-result ctime.c -o ctime

popd

