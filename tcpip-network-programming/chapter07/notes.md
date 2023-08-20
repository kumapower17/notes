# 优雅地断开套接字连接


## 7.1 基于TCP的半关闭

### 套接字和流（Stream）

``` c
#include <sys/socket.h>
int shutdown(int sock, int howto);
```

> It’s important to note that shutdown() doesn’t actually close the file descriptor—it just changes its usability. To free a socket descriptor, you need to use close().

https://beej.us/guide/bgnet/html/#close-and-shutdownget-outta-my-face

> Big difference between shutdown and close on a socket is the behavior when the socket is shared by other processes. A shutdown() affects all copies of the socket while close() affects only the file descriptor in one process
https://stackoverflow.com/questions/4160347/close-vs-shutdown-socket


### 基于半关闭的文件传输程序

> RETURN VALUES
>
> The functions fread() and fwrite() advance the file position indicator for the stream by the number of bytes read or written.  They return the number of objects read or written.
>
> If an error occurs, or the end-of-file is reached, the return value is a short object count (or zero).
>
> The function fread() does not distinguish between end-of-file and error; callers must use feof(3) and ferror(3) to determine which occurred.  The function fwrite() returns a value less than nitems only if a write error has occurred.
