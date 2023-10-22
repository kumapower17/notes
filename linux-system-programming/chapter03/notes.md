# Buffered I/O

> Inside the kernel, all filesystem operations occur in terms of blocks. Indeed, the block is the lingua franca of I/O. Consequently, no I/O operation may execute on an amount of data less than the block size or that is not an integer multiple of the block size. If you only want to read a byte, too bad: you’ll have to read a whole block. Want to write 4.5 blocks worth of data? You’ll need to write 5 blocks, which implies reading that partial block in its entirety, updating only the half you’ve changed, and then writing back the whole block.


## User-Buffered I/O

## Standard I/O

### File Pointers

> The FILE name is often derided for its ugly use of all-caps, which is particularly egregious given that the C standard (and consequently most application’s own coding styles) use all-lowercase names for functions and types. The peculiarity lies in history: standard I/O was originally written as macros. Not only FILE but all of the methods in the library were implemented as a set of macros. The style at the time, which remains common to this day, is to give macros all-caps names. As the C language progressed and standard I/O was ratified as an official part, most of the methods were reimplemented as proper functions and FILE became a typedef. But the uppercase remains.

## Opening Files

``` c
#include <stdio.h>

FILE * fopen (const char *path, const char *mode);

```

> The given mode may also contain the character b, although this value is always ignored on Linux. Some operating systems treat text and binary files differently, and the b mode instructs the file to be opened in binary mode. Linux, as with all POSIX-conforming systems, treats text and binary files identically.


## Opening a Stram via File Descriptor

``` c
#include <stdio.h>

FILE * fdopen (int fd, const char *mode);

```

> The possible modes are the same as for fopen() and must be compatible with the modes originally used to open the file descriptor. The modes w and w+ may be specified, but they will not cause truncation. The stream is positioned at the file position associated with the file descriptor.

> Once a file descriptor is converted to a stream, I/O should no longer be directly performed on the file descriptor. It is, however, legal to do so. Note that the file descriptor is not duplicated, but is merely associated with a new stream. Closing the stream will close the file descriptor as well.

## Closing Streams

``` c
#include <stdio.h>

int fclose (FILE *stream);

```

> Any buffered and not-yet-written data is first flushed. On success, fclose() returns 0. On failure, it returns EOF and sets errno appropriately.

### Closing All Streams

The fcloseall() function closes all streams associated with the current process, including standard in, standard out, and standard error:

``` c
#define _GNU_SOURCE
#include <stdio.h>
int fcloseall (void);
```

Before closing, all streams are flushed. The function always returns 0; it is Linux-specific.

## Flushing a Stream

The standard I/O library provides an interface for writing out the user buffer to the kernel, ensuring that all data written to a stream is flushed via write().

``` c
#include <stdio.h>

int fflush (FILE *stream);
```

> To understand the effect of fflush(), you have to understand the difference between the buffer maintained by the C library and the kernel’s own buffering. All of the calls described in this chapter work with a buffer that is maintained by the C library, which resides in user space, not kernel space. That is where the performance improvement comes in—you are staying in user space and therefore running user code, not issuing system calls. A system call is issued only when the disk or some other medium has to be accessed. fflush() merely writes the user-buffered data out to the kernel buffer. The effect is the same as if user buffering was not employed and write() was used directly. It does not guarantee that the data is physically committed to any medium—for that need, use something like fsync(). For situations where you are concerned with ensuring that your data is commited to the backing store, you will want to call fflush(), followed immediately by fsync(): that is, first ensure that the user buffer is written out to the kernel and then ensure that the kernel’s buffer is written out to disk.

## Errors and End-of-File

https://stackoverflow.com/questions/5431941/why-is-while-feoffile-always-wrong


## Obtaining the Associated File Descriptor

> Intermixing standard I/O calls with system calls is not normally advised. Programmers must exercise caution when using fileno() to ensure their file descriptor—based actions do not conflict with the user buffering. Particularly, a good practice is to flush the stream before manipulating the backing file descriptor. You should almost never intermix file descriptor and stream-based I/O operations.

## Buffering

``` c
#include <stdio.h>

int setvbuf (FILE *stream, char *buf, int mode, size_t size);

```

> The setvbuf() function must be called after opening the stream but before any other operations have been performed on it. It returns 0 on success, and a nonzero value otherwise.
>
> The supplied buffer, if any, must exist when the stream is closed. A common mistake is to declare the buffer as an automatic variable in a scope that ends before the stream is closed.

> Generally, developers need not mess with the buffering on a stream. With the exception of standard error, terminals are line-buffered, and that makes sense. Files are blockbuffered, and that, too, makes sense. The default buffer size for block buffering is BUFSIZ, defined in <stdio.h>, and it is usually an optimal choice (a large multiple of a typical block size).

## Thread Safety

> As the standard I/O functions are thread-safe, the individual writes will not interleave and result in garbled output. That is, even if two threads each issue a write request at the same time, locking will ensure that one write request completes before the other.

### Manual File Locking

The function `flockfile()` waits until stream is no longer locked, bumps the lock count, and then acquires the lock, becoming the owning thread of the stream, and returns.

``` c
#include <stdio.h>

void flockfile (FILE *stream);
void funlockfile (FILE *stream);
int ftrylockfile (FILE *stream);

```

### Unlocked Stream Operations

There is a second reason for performing manual locking on streams. With the finergrained and more precise control of locking that only the application programmer can provide, it might be possible to minimize the overhead of locking and to improve performance. To this end, Linux provides a family of functions, cousins to the usual standard I/O interfaces, that do not perform any locking whatsoever. They are, in effect, the unlocked counterparts to standard I/O:

``` c
#define _GNU_SOURCE

#include <stdio.h>

int fgetc_unlocked (FILE *stream);
char *fgets_unlocked (char *str, int size, FILE *stream);
size_t fread_unlocked (void *buf, size_t size, size_t nr, FILE *stream);
int fputc_unlocked (int c, FILE *stream);
int fputs_unlocked (const char *str, FILE *stream);
size_t fwrite_unlocked (void *buf, size_t size, size_t nr, FILE *stream);
int fflush_unlocked (FILE *stream);
int feof_unlocked (FILE *stream);
int ferror_unlocked (FILE *stream);
int fileno_unlocked (FILE *stream);
void clearerr_unlocked (FILE *stream);

```

These functions all behave identically to their locked cousins, except that they do not check for or acquire the lock associated with the given stream. If locking is required, it is the responsibility of the programmer to ensure that the lock is manually acquired and released.


## Critiques of Standard I/O

The biggest complaint with standard I/O is the performance impact from the double copy. When reading data, standard I/O issues a read() system call to the kernel, copying the data from the kernel to the standard I/O buffer. When an application then issues a read request via standard I/O using, say, fgetc(), the data is copied again, this time from the standard I/O buffer to the supplied buffer. Write requests work in the opposite fashion: the data is copied once from the supplied buffer to the standard I/O buffer and then later from the standard I/O buffer to the kernel via write().
