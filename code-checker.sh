#!/bin/bash

#
# Code Checker
# This will allow you to easily grep your codebase in search for useful
# terms. Typically used for finding `TODO`s or other notes, it can be
# used to find any term in the codebase.
#

function find_in_code {
    echo "finding " $1
    find . -name '*.h' -print0 | xargs -0 grep -H $1
    find . -name '*.cpp' -print0 | xargs -0 grep -H $1
    find . -name '*.inl' -print0 | xargs -0 grep -H $1
    find . -name '*.c' -print0 | xargs -0 grep -H $1
}

echo ""
echo "Starting Code Checker"
echo "---------------------"

if [ "$1" ]
then
    find_in_code $1
else
    echo "Usage:"
    echo "./code-checker.sh SEARCH_TERM"
    echo ""
    echo "Useful Search Terms"
    echo "  -  CLEANUP"
    echo "  -  IMPORTANT"
    echo "  -  TODO"
    echo "  -  BUG"
    echo ""
fi

