# TODO list

## Current Task
+ DONE: Gap Buffer

## Backlog
+ TODO: Moving the terminal doesn't re-size the 'terminal dimensions'


## Exploration
 
+ TODO: Explore streaming in data from hard drive to solve the very
  large file prob.  Similar to the world of a large game, streaming in
  chunks of world map. You'd want to stream in the direct chunk you're
  working on, and surrounding chunks, so contextual information
  contained on screen. The rest can be written to disk. Would a gap
  buffer be better for this implementation? or rope?
  
+ TODO: when reading a file (ReadOnly Mode) just map the file to an
  array of characters. This is how the file system would store it.
  However.. when moving into Edit mode the text would have to be
  adjusted to whatever buffer strategy is selected. This way we can
  take the readonly mode and modify portion of the buffer (stream it
  to the editor mode data structure?) I'm not sure how this would work
  but the simple idea is there:
  ReadOnly: Stream characters using the fastest possible mechanism for
            viewing
  EditMode: Start streaming data from readonly data structures -> edit
            data structures

 
+ TODO: from a reddit post that I want to explore:
  An edit buffer is made up of a doubly-linked list of fixed sized (4
  KB) gap buffers. A gap buffer has two parts: a ~16 byte header,
  which is always in memory, and the actual buffer, which can be paged
  out to a swap file (a vfile- see vfile.h). A gap buffer consists of
  three regions: text before the gap, the gap and text after the gap
  (which always goes all the way to the end of buffer). (hole and
  ehole in the header indicate the gap position). The size of the gap
  may be 0 (which is the case when a file is first loaded). Gap
  buffers are fast for small inserts and deletes when the cursor is at
  the gap (for delete you just adjust a pointer, for insert you copy
  the data into gap). When you reposition the cursor, you have to move
  the gap before any inserts or deletes occur. If you had only one
  window and a single large gap buffer for the file, you could always
  keep the gap at the cursor- the right-arrow key copies one character
  across the gap.
  
  It should be possible to quickly load files by mapping them directly
  into memory (using mmap()) and treating each 4KB page as a gap
  buffer with 0 size gap. When page is modified, use copy-on-write to
  move the page into the swap file (change pointer in header). This is
  not done now. Instead the file is copied when loaded.

