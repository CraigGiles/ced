#!/bin/bash

# MAIN_FILE=key_code_test.c
MAIN_FILE=osx_main_terminal.c
# MAIN_FILE=test_file.c
EXE_NAME=ced

CTIME_EXEC="ctime/ctime"
CTIME_TIMING_FILE="ctime_info.ctm"

CC=clang

# Build ctime
if [ ! -f "$CTIME_EXEC" ]; then
  ./"ctime/build.sh"
fi

#
# Build Script
# This script is designed to be run from the root of the project.
#
COMPILE_FLAGS="-g \
               -O0 \
               -Wall \
               -Werror \
               -Wno-write-strings \
               -Wno-unused-variable \
               -Wno-unused-value \
               -Wno-unused-function \
               -Wno-missing-braces \
               -Wno-sign-compare \
               -fno-rtti \
               -fno-exceptions \
               -Wno-shift-negative-value" # NOTE(craig): Need this for the stb libraries. 

DEBUG_FLAGS="-DGILESC_DEBUG=1"

LD_FLAGS="\
          -framework AppKit \
         "

EXE_HEADER_INCLUDES="-Isrc -Isrc/stb"

function clean_build_directory {
    echo "cleaning up old build folder"
    rm -rf build
    mkdir -p build
}

function build_platform_layer {
    echo "building platform layer"
    $CC $COMPILE_FLAGS $DEBUG_FLAGS ./src/$MAIN_FILE -o ./build/$EXE_NAME -g $LD_FLAGS $EXE_HEADER_INCLUDES
}

function start_compile_timing {
    $CTIME_EXEC -begin "$CTIME_TIMING_FILE"
}

function stop_compile_timing {
    $CTIME_EXEC -end "$CTIME_TIMING_FILE" $LAST_ERROR
}

function count_lines_of_code {
    echo ""
    ./cloc.sh
}

function run_application {
    ./build/$EXE_NAME
}

if [ "$1" = "run" ]
then
    echo "Running Application"
    echo "-------------------"
    run_application
else
    echo ""
    echo "Starting Build"
    echo "--------------"
    clean_build_directory
    start_compile_timing
    build_platform_layer
    stop_compile_timing
    count_lines_of_code
fi

