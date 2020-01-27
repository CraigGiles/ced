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
  ced_*   - The core editor and cross platform code
  osx_*   - The OSX platform layer for the editor.
  gilesc_ - My own personal C libraries

The idea for the platform layers is to split the platform specific
code into its own layer calling into the 'core' of the editor.

Since this is just a side project for my own editor and I don't forsee
me actually using this as a day to day driver unless I really get
annoyed with my current editor options, I've put it under the MIT
license.
