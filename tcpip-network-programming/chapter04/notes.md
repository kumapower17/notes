# 基于TCP的服务器端/客户端（1）

## 4.1 理解TCP和UDP

> The OSI protocol suite that was specified as part of the OSI project was considered by many as too complicated and inefficient, and to a large extent unimplementable. Taking the "forklift upgrade" approach to networking, it specified eliminating all existing networking protocols and replacing them at all layers of the stack. This made implementation difficult and was resisted by many vendors and users with significant investments in other network technologies. In addition, the protocols included so many optional features that many vendors' implementations were not interoperable.

https://en.wikipedia.org/wiki/OSI_model#Comparison_with_TCP/IP_model


## 4.2 实现基于TCP的服务器端/客户端

### TCP服务器端的默认函数调用顺序

1. `socket()`
2. `bind()`
3. `listen()`
4. `accept()`
5. `read()/write()`
6. `close()`


https://stackoverflow.com/questions/5895751/a-bind-with-a-port-of-zero-will-bind-you-to-a-free-port-is-this-portable

### 进入等待连接请求状态

https://github.com/torvalds/linux/blob/master/net/socket.c#L1820C1-L1822C24

Maximum Length Control of Accept Queue:
> The maximum length of a TCP accept queue is controlled by min(somaxconn, backlog), where:
> somaxconn is kernel parameter for Linux and is specified by /proc sys/net/core/somaxconn

https://github.com/golang/go/issues/39000

``` bash
ubuntu@primary:~/play$ gcc backlog.c -o backlog
ubuntu@primary:~/play$ cat /proc/sys/net/core/somaxconn
4096
ubuntu@primary:~/play$ ./backlog 4095 &
[1] 6369
ubuntu@primary:~/play$ set backlog to 4095

ubuntu@primary:~/play$ ss -t state listening  sport = 9878 | awk '{print $2}'
Send-Q
4095
ubuntu@primary:~/play$ kill %1
ubuntu@primary:~/play$ ./backlog 4096 &
[2] 6373
[1]   Terminated              ./backlog 4095
ubuntu@primary:~/play$ set backlog to 4096

ubuntu@primary:~/play$ ss -t state listening  sport = 9878 | awk '{print $2}'
Send-Q
4096
ubuntu@primary:~/play$ kill %2
ubuntu@primary:~/play$ ./backlog 4097 &
[3] 6376
[2]   Terminated              ./backlog 4096
ubuntu@primary:~/play$ set backlog to 4097

ubuntu@primary:~/play$ ss -t state listening  sport = 9878 | awk '{print $2}'
Send-Q
4096
```

### TCP客户端的默认函数调用顺序

1. `socket()`
2. `connect()`
3. `read()/write()`
4. `close()`

https://stackoverflow.com/questions/2597608/c-socket-connection-timeout

https://docs.python.org/3/library/socket.html#socket-timeouts


## 4.3 实现迭代服务器端/客户端

