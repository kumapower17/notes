# 套接字类型与协议设置


## 2.1 套接字类型及其数据传输特性

### 套接字类型1:面向连接的套接字（SOCK——STREAM）

>>> 套接字缓冲已满是否意味着数据丢失？
>>> 此时套接字无法再接收数据，但即使这样也不会发生数据丢失，因为传输端套接字将停止传输。也就是说，面向连接的套接字会根据接收端的状态传输数据，如果传输出错还会提供重传服务。因此面向连接的套接字除特殊情况外不会发生数据丢失。

https://stackoverflow.com/questions/42857662/understanding-buffering-in-tcp

``` bash
$ cd chapter02/demo/recvbuf-full; go run recvbuf_full.go 2>log.txt &
$ sudo tcpdump -n -i lo0 src port 54940 -w recvbuf-full.pcap
$ tcpdump -r recvbuf-full.pcap | grep -Eo 'win [0-9]+'
reading from file recvbuf-full.pcap, link-type NULL (BSD loopback)
win 65535
win 6379
win 6364
win 6348
win 6332
win 6317
win 6301
win 6285
win 6270
win 6254
win 6239
win 6223
win 6207
win 6192
win 6176
win 6160
win 6145
win 6129
win 4895
win 3910
win 3114
win 2848
win 2582
win 2317
win 2051
win 1785
win 1520
win 1254
win 989
win 785
win 614
win 489
win 379
win 301
win 239
win 176
win 129
win 98
win 67
win 51
win 35
win 20
win 4
win 0
win 0
win 0
win 0
```


### 套接字类型2：面向消息的套接字（SOCK_DGRAM）

>>> 面向消息的套接字不存在连接的概念

udp是没有连接的，那么golang的 [UnixConn](https://pkg.go.dev/net#UnixConn) 是什么含义：
>>> UDPConn is the implementation of the Conn and PacketConn interfaces for UDP network connections.

>>> The common instantiation of a UDP client in Go is net.Dial("udp", address). This returns a net.Conn object implemented by a net.UDPConn. It provides both Read and Write methods. This is the equivalent of creating a socket of type SOCK_DGRAM and calling connect to bind the socket to a specific remote host. The process of calling connect means that the socket is now a connected socket, despite the fact that UDP is a “connectionless” protocol. Unlike with TCP, calling connect will not cause any packets to be transmitted, since there is no UDP handshake.

https://dadrian.io/blog/posts/udp-in-go/


### 协议的最终选择

``` c
int socket(int domain, int type, int protocol);
```

>>> 前面已经通过socket函数的前两个参数传递了协议族信息和套接字数据传输方式，这些信息还不足以决定采用的协议吗？为什么还需要传递第三个参数呢？
>>> 正如大家所想，传递前两个参数即可创建所需的套接字。所以大部分情况下可以向第三个参数传递0，除非遇到以下这种情况：
>>> 同一个协议族中存在多个数据传输方式相同的协议
>>> 数据传输方式相同，但协议不同。此时需要通过第三个参数具体指定协议信息。

https://stackoverflow.com/questions/27507288/what-does-the-parameter-protocol-stand-for-in-socket-system-call
