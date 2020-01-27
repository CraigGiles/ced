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

# Build Statistics for 27th Jan, 2020

### Compile Time (ctime): 0.279 seconds

### Lines of Code (cloc):
```
      16 text files.
      16 unique files.
       1 file ignored.

github.com/AlDanial/cloc v 1.84  T=0.03 s (497.0 files/s, 41128.2 lines/s)
-------------------------------------------------------------------------------
Language                     files          blank        comment           code
-------------------------------------------------------------------------------
C/C++ Header                    10            125             19            550
C                                6            100             48            482
-------------------------------------------------------------------------------
SUM:                            16            225             67           1032
-------------------------------------------------------------------------------
```
