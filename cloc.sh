#!/bin/bash

#
# Count Lines of Code
# Insert the cloc metrics of your codebase, excluding the directories
# listed in the --exclude-dirs. There are other configuration options
# that can be used for this tool, please refer to the official
# documentation.
#
# https://github.com/AlDanial/cloc#options-
#

if hash cloc 2>/dev/null; then 
    cloc . --match-d=src --not-match-d=stb
fi
