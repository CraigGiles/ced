#!/bin/bash

#
# Taken from the `ctime.c` file:
# ctime is a simple utility that helps you keep track of how much time
# you spend building your projects.  You use it the same way you would
# use a begin/end block profiler in your normal code, only instead of
# profiling your code, you profile your build.
#
# NOTE: this script contains a helper function that allows you to
# display the metrics from your build. Simply run ./ctime_stats.sh and
# the output will show build time metrics gathered by the ctime utility.
#
# Please refer to the BASIC INSTRUCTIONS section of the ctime.c file
# for a full description of the utility
#
function stats {
    ./ctime/ctime -stats ctime_info.ctm
}

stats
