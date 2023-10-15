# File I/O

## Owners of New Files

> determining which user owns a new file is straightforward: the uid of the file’s owner is the effective uid of the process creating the file.

> Determining the owning group is more complicated. The default behavior is to set the file’s group to the effective gid of the process creating the file. This is the System V behavior, which is the behavioral model for much of Linux, and thus the standard Linux modus operandi.

> To be difficult, however, BSD defined its own behavior: the file’s group is set to the gid of the parent directory. This behavior is available on Linux via a mount-time option it is also the behavior that will occur on Linux by default if the file’s parent directory has the set group ID (setgid) bit set.


## Reading via read()

> It is legal for read() to return a positive nonzero value less than len. This can happen for a number of reasons: less than len bytes may have been available, the system call may have been interrupted by a signal, the pipe may have broken (if fd references a pipe), and so on.

> Some errors are recoverable. For example, if a call to read() is interrupted by a signal before any bytes are read, it returns −1 (a 0 could be confused with EOF), and errno is set to EINTR. In that case, you can and should resubmit the read.

> Indeed, a call to read() can result in many possibilities:
>
> The call returns a value equal to len. All len read bytes are stored in buf. The results are as intended.
>
> The call returns a value less than len, but greater than zero. The read bytes are stored in buf. This can occur because a signal interrupted the read midway; an error occurred in the middle of the read; more than zero, but less than len bytes’ worth of data was available; or EOF was reached before len bytes were read. Reissuing the read (with correspondingly updated buf and len values) will read the remaining bytes into the rest of the buffer or indicate the cause of the problem.
>
> The call returns 0. This indicates EOF. There is nothing to read.
>
> The call blocks because no data is currently available. This won’t happen in nonblocking mode.
>
> The call returns −1, and errno is set to EINTR. This indicates that a signal was received before any bytes were read. The call can be reissued.
>
> The call returns −1, and errno is set to EAGAIN. This indicates that the read would block because no data is currently available, and that the request should be reissued later. This happens only in nonblocking mode.
>
> The call returns −1, and errno is set to a value other than EINTR or EAGAIN. This indicates a more serious error. Simply reissuing the read is unlikely to succeed.

[runtime: signal handling: Go assumes SA_RESTART, but does not enforce it](https://github.com/golang/go/issues/20400)

``` bash
ubuntu@ubuntu22:~/workspace/chapter02/demo/godemo$ strace -f -e trace=rt_sigaction ./main
rt_sigaction(SIGHUP, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGHUP, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGINT, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGINT, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGQUIT, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGQUIT, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGILL, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGILL, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGTRAP, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGTRAP, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGABRT, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGABRT, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGBUS, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGBUS, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGFPE, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGFPE, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGUSR1, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGUSR1, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGSEGV, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGSEGV, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGUSR2, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGUSR2, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGPIPE, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGPIPE, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGALRM, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGALRM, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGTERM, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGTERM, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGSTKFLT, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGSTKFLT, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGCHLD, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGCHLD, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGURG, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGURG, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGXCPU, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGXCPU, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGXFSZ, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGXFSZ, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGVTALRM, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGVTALRM, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGPROF, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGPROF, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGWINCH, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGWINCH, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGIO, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGIO, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGPWR, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGPWR, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGSYS, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGSYS, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGRTMIN, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_1, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_1, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGRT_2, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_3, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_3, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGRT_4, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_4, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGRT_5, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_5, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGRT_6, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_6, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGRT_7, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_7, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGRT_8, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_8, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGRT_9, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_9, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGRT_10, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_10, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGRT_11, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_11, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGRT_12, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_12, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGRT_13, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_13, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGRT_14, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_14, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGRT_15, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_15, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGRT_16, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_16, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGRT_17, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_17, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGRT_18, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_18, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGRT_19, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_19, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGRT_20, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_20, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGRT_21, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_21, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGRT_22, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_22, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGRT_23, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_23, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGRT_24, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_24, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGRT_25, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_25, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGRT_26, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_26, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGRT_27, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_27, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGRT_28, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_28, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGRT_29, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_29, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGRT_30, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_30, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGRT_31, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_31, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
rt_sigaction(SIGRT_32, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_32, {sa_handler=0x453d60, sa_mask=~[], sa_flags=SA_RESTORER|SA_ONSTACK|SA_RESTART|SA_SIGINFO, sa_restorer=0x453ea0}, NULL, 8) = 0
strace: Process 1222 attached
strace: Process 1221 attached
strace: Process 1223 attached
[pid  1222] +++ exited with 0 +++
[pid  1221] +++ exited with 0 +++
[pid  1223] +++ exited with 0 +++
+++ exited with 0 +++


```

### Nonblocking Reads

> Sometimes, programmers do not want a call to read() to block when there is no data available. Instead, they prefer that the call return immediately, indicating that no data is available. This is called nonblocking I/O; it allows applications to perform I/O, potentially on multiple files, without ever blocking, and thus missing data available in another file.

> Consequently, an additional errno value is worth checking: EAGAIN. As discussed previously, if the given file descriptor was opened in nonblocking mode (if O_NONBLOCK was given to open() and there is no data to read, the read() call will return −1 and set errno to EAGAIN instead of blocking.

## Writing with write()

### Partial Writes

> The write() system call is less likely to return a partial write than the read() system call is to return a partial read. Also, there is no EOF condition for a write() system call. For regular files, write() is guaranteed to perform the entire requested write, unless an error occurs.

> Consequently, for regular files, you do not need to perform writes in a loop. However, for other file types—say, sockets—a loop may be required to guarantee that you really write out all of the requested bytes. Another benefit of using a loop is that a second call to write() may return an error revealing what caused the first call to perform only a partial write (although, again, this situation is not very common).

https://stackoverflow.com/questions/43189375/why-is-golang-http-server-failing-with-broken-pipe-when-response-exceeds-8kb

### Append Mode

https://man.archlinux.org/man/logrotate.8#copytruncate

https://wsgzao.github.io/post/logrotate/


### Behavior of write()

https://docs.kernel.org/admin-guide/sysctl/vm.html#dirty-expire-centisecs

## Synchronized I/O

### fsync and fdatasync()

Linux also provides the system call fdatasync():

``` c
#include <unistd.h>

int fdatasync (int fd);
```

> This system call does the same thing as fsync(), except that it only flushes data and metadata required to properly access the file in the future. For example, a call to fdata sync() will flush a file’s size, since you need that to read the file correctly. The call does not guarantee that nonessential metadata is synchronized to disk, and is therefore potentially faster. Most use cases do not consider metadata such as the file modification timestamp part of their essential transaction and thus fdatasync() is sufficient for their needs and potentially faster.

> Neither function guarantees that any updated directory entries containing the file are synchronized to disk. This implies that if a file’s link has recently been updated, the file’s data may successfully reach the disk but not the associated directory entry, rendering the file unreachable. To ensure that any updates to the directory entry are also committed to disk, fsync() must also be called on a file descriptor opened against the file’s directory.

Less optimal, but wider in scope, the old-school sync() system call is provided for synchronizing all buffers to disk:

``` c
#include <unistd.h>

void sync (void);
```

> The standards do not require sync() to wait until all buffers are flushed to disk before returning; they require only that the call initiates the process of committing all buffers to disk. For this reason, it is often recommended to invoke sync() multiple times to ensure that all data is safely on disk. Linux, however, does wait until all buffers are committed. Therefore, a single sync() is sufficient.

https://unix.stackexchange.com/questions/5260/is-there-truth-to-the-philosophy-that-you-should-sync-sync-sync-sync

https://utcc.utoronto.ca/~cks/space/blog/unix/TheLegendOfSync

> One way of looking at O_SYNC is that it forces an implicit fsync() after each write() operation, before the call returns. These are indeed the semantics provided, although the Linux kernel implements O_SYNC a bit more efficiently.

> O_SYNC results in slightly worse user and kernel times (time spent in user and kernel space, respectively) for write operations. Moreover, depending on the size of the file being written, O_SYNC can cause an increase in total elapsed time of one or two orders of magnitude because all I/O wait time (time spent waiting for I/O to complete) is incurred by the process. The increase in cost is huge, so synchronized I/O should be used only after exhausting all possible alternatives.

> Normally, applications that need guarantees that write operations have hit the disk use fsync() or fdatasync(). These tend to incur less cost than O_SYNC, as they can be called less often (i.e., only after certain critical operations have completed).

> POSIX defines two other synchronized-I/O-related open() flags: O_DSYNC and O_RSYNC. On Linux, these flags are defined to be synonymous with O_SYNC; they provide the same behavior.


## Direct I/O

> The Linux kernel, like any modern operating system kernel, implements a complex layer of caching, buffering, and I/O management between devices and applications. A high-performance application may wish to bypass this layer of complexity and perform its own I/O management.

> Providing the O_DIRECT flag to open() instructs the kernel to minimize the presence of I/O management. When this flag is provided, I/O will initiate directly from user-space buffers to the device, bypassing the page cache. All I/O will be synchronous; operations will not return until completed.

## Closing File

> Note that closing a file has no bearing on when the file is flushed to disk. To ensure that a file is committed to disk before closing it, an application needs to make use of one of the synchronization options discussed earlier in “Synchronized I/O”

## poll() Versus select()

Although they perform the same basic job, the poll() system call is superior to select() for a handful of reasons:
- poll() does not require that the user calculate and pass in as a parameter the value of the highest-numbered file descriptor plus one.
- poll() is more efficient for large-valued file descriptors. Imagine watching a single file descriptor with the value 900 via select()—the kernel would have to check each bit of each passed-in set, up to the 900th bit.
- select()’s file descriptor sets are statically sized, introducing a trade-off: they are small, limiting the maximum file descriptor that select() can watch, or they are inefficient. Operations on large bitmasks are not efficient, especially if it is not known whether they are sparsely populated. With poll(), one can create an array of exactly the right size.
- With select(), the file descriptor sets are reconstructed on return, so each subsequent call must reinitialize them. The poll() system call separates the input (events field) from the output (revents field), allowing the array to be reused without change.
- The timeout parameter to select() is undefined on return. Portable code needs to reinitialize it. This is not an issue with pselect(), however.

## Kernel Internals

https://docs.kernel.org/admin-guide/sysctl/vm.html#swappiness

