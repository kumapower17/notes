# Introduction and Essential Concepts

## Concepts of Linux Programming

### Files and the Filesystem

https://github.com/brgl/busybox/blob/master/procps/lsof.c

https://arvimal.wordpress.com/2016/07/21/max-file-name-length-in-an-ext4-file-system/

> Hard links cannot span filesystems because an inode number is meaningless outside of the inode’s own filesystem. To allow links that can span filesystems, and that are a bit simpler and less transparent, Unix systems also implement symbolic links (often shortened to symlinks).

### Signals

https://unix.stackexchange.com/questions/186200/whats-the-easiest-way-to-detect-what-signals-are-being-sent-to-a-process

https://www.mankier.com/8/killsnoop.bt

> The program jumps to this function as soon as the signal is received. When the signal handler returns, the control of the program resumes at the previously interrupted instruction. Because of the asynchrony of signals, signal handlers must take care not to stomp on the code that was interrupted, by executing only async-safe (also called signal-safe) functions.
