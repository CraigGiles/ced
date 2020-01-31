# CED Terminal Editor

This is the toy terminal editor written in C (C-Editor, CED) that I've
been fleshing out for my own personal use as well as a learning
experience. As of right now it just started out as a main file with
escape terminal codes all based off the `kilo` editor:

https://viewsourcecode.org/snaptoken/kilo/index.html

There are other inspirations at the start too, especially from Walter
Bright:
https://github.com/DigitalMars/me

The basic editor codebase is split into 3 sections:
  + ced_*   - The core editor and cross platform code
  + osx_*   - The OSX platform layer for the editor.
  + gilesc_ - My own personal C libraries

The idea for the platform layers is to split the platform specific
code into its own layer calling into the 'core' of the editor.

Since this is just a side project for my own editor and I don't forsee
me actually using this as a day to day driver unless I really get
annoyed with my current editor options, I've put it under the MIT
license.

## Build Statistics for 27th Jan, 2020

### Compile Time (ctime):

```
Latest compile timing: 0.279 seconds

ctime_info.ctm Statistics
Total complete timings: 2385
Total incomplete timings: 0
Days with timings: 12
Days between first and last timing: 10
Timings marked successful (2385):
  Slowest: 1.966 seconds
  Fastest: 0.024 seconds
  Average: 0.331 seconds
  Total: 13 minutes, 10.316 seconds
```

### Lines of Code (cloc):
```
      16 text files.
      16 unique files.
       1 file ignored.

github.com/AlDanial/cloc v 1.84  T=0.03 s (533.1 files/s, 56960.2 lines/s)
-------------------------------------------------------------------------------
Language                     files          blank        comment           code
-------------------------------------------------------------------------------
C                                5            127             59            608
C/C++ Header                     9            122             21            559
-------------------------------------------------------------------------------
SUM:                            14            249             80           1167
-------------------------------------------------------------------------------

```
