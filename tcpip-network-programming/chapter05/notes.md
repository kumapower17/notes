# 基于TCP的服务器端/客户端（2）

## 回声客户端的完美实现

### 回声客户端问题的解决方法

>>> while(recv_len != str_len) {}
>>> 接收的数据大小应和传输的相同，因此，recv_len中保存的值等于str_len中保存的值时，即可跳出循环。也许各位认为这种循环写法更符合逻辑，但有可能引发无限循环。假设发生异常情况，读取数据过程中recv_len超过str_len，此时就无法退出循环，而如果while循环写成下面这种形式，则即使发生异常也不会陷入无限循环。
>>> while(recv_len < str_len) {}
>>> 写循环语句时应尽量降低异常情况而陷入无限循环的可能。

上面提到的是可靠性层面，从数据完整性、正确性层面还得处理 `recv_len != str_len` 的情况。

### 计算器服务器端/客户端示例

>>> 协议设计是网络编程的核心
>>> 对于专用的业务系统，协议设计是核心任务，决定了系统的开发难度与可靠性，但是这个领域还没 有形成大家公认的设计流程。
>>> 系统中哪个程序发起连接，哪个程序接受连接?如果写标准的网络服务，那么这不是问题，按 RFC 来就行了。自己设计业务系统，有没有章法可循?以网游为例，到底是连接服务器主动连接逻辑服务器， 还是逻辑服务器主动连接“连接服务器”?似乎没有定论，两种做法都行。一般可以按照“依赖->被依 赖”的关系来设计发起连接的方向。
>>> 比新建连接难的是关闭连接。在传统的网络服务中(特别是短连接服务)，不少是服务端主动关闭 连接，比如 daytime、HTTP/1.0。也有少部分是客户端主动关闭连接，通常是些长连接服务，比如 echo、 chargen 等。我们自己的业务系统该如何设计连接关闭协议呢?
>>> 服务端主动关闭连接的缺点之一是会多占用服务器资源。服务端主动关闭连接之后会进入 TIME_WAIT 状态，在一段时间之内 hold 住一些内核资源。如果并发访问量很高，这会影响服务端的处 理能力。这似乎暗示我们应该把协议设计为客户端主动关闭，让 TIME_WAIT 状态分散到多台客户机器 上，化整为零。
>>> 这又有另外的问题:客户端赖着不走怎么办?会不会造成拒绝服务攻击?或许有一个二者结合的方 案:客户端在收到响应之后就应该主动关闭，这样把 TIME_WAIT 留在客户端。服务端有一个定时器， 如果客户端若干秒钟之内没有主动断开，就踢掉它。这样善意的客户端会把 TIME_WAIT 留给自己， buggy 的客户端会把 TIME_WAIT 留给服务端。或者干脆使用长连接协议，这样避免频繁创建销毁连接。

https://github.com/kerasking/book-1/blob/master/OS%20网络编程杂谈-陈硕.pdf

https://github.com/kumapower17/notes/blob/857a1e87f6029abee241c61a4df9a042f17d2b1d/chapter05/op_client.c#L41C3-L45C6

上面这段代码强制类型转换 `char[]` 为 `int*` 在我看来是有问题的：

https://stackoverflow.com/questions/40548852/memory-alignment-when-casting-different-data-types

https://stackoverflow.com/questions/22598992/alignment-of-char-array-struct-members-in-c-standard

https://stackoverflow.com/questions/98650/what-is-the-strict-aliasing-rule

>>> 若想在同一个数组中保存并传输多种数据类型，应把数组声明为 `char` 类型。

我觉得声明为 `unsigned char` 更为合适，毕竟 `char` 可能是有符号的：
>>> A computation involving unsigned operands can never overﬂow, because a result that cannot be represented by the resulting unsigned integer type is reduced modulo the number that is one greater than the largest value that can be represented by the resulting type.
>>> An example of undeﬁned behavior is the behavior on integer overﬂow

https://stackoverflow.com/questions/2054939/is-char-signed-or-unsigned-by-default

https://wiki.sei.cmu.edu/confluence/display/c/INT07-C.+Use+only+explicitly+signed+or+unsigned+char+type+for+numeric+values

https://stackoverflow.com/questions/47263865/is-signed-char-overflow-undefined-within-the-range-255-to-255

https://stackoverflow.com/questions/18195715/why-is-unsigned-integer-overflow-defined-behavior-but-signed-integer-overflow-is

## TCP原理

https://blog.csdn.net/dog250/article/details/81697403

https://unix.stackexchange.com/questions/210367/changing-the-tcp-rto-value-in-linux

https://blog.cloudflare.com/when-tcp-sockets-refuse-to-die/

https://plantegg.github.io/2017/06/02/就是要你懂TCP--连接和握手/

https://blog.cloudflare.com/this-is-strictly-a-violation-of-the-tcp-specification/

https://stackoverflow.com/questions/3757289/when-is-tcp-option-so-linger-0-required

``` bash
root@primary:/home/ubuntu# tcpdump -ni lo port 8999 &
[1] 2404
root@primary:/home/ubuntu# tcpdump: verbose output suppressed, use -v or -vv for full protocol decode
listening on lo, link-type EN10MB (Ethernet), capture size 262144 bytes

root@primary:/home/ubuntu#
root@primary:/home/ubuntu#
root@primary:/home/ubuntu# ./finwait_linger &
[2] 2405
root@primary:/home/ubuntu# 2023/08/06 23:23:08 tcp addr 127.0.0.1:8999

root@primary:/home/ubuntu#
root@primary:/home/ubuntu# ./finwait_client &
[3] 2409
root@primary:/home/ubuntu# 2023/08/06 23:23:13 dial 127.0.0.1:8999 ok
23:23:13.860267 IP 127.0.0.1.45056 > 127.0.0.1.8999: Flags [S], seq 923573914, win 65495, options [mss 65495,sackOK,TS val 807027987 ecr 0,nop,wscale 7], length 0
23:23:13.860274 IP 127.0.0.1.8999 > 127.0.0.1.45056: Flags [S.], seq 3797568418, ack 923573915, win 65483, options [mss 65495,sackOK,TS val 807027987 ecr 807027987,nop,wscale 7], length 0
23:23:13.860281 IP 127.0.0.1.45056 > 127.0.0.1.8999: Flags [.], ack 1, win 512, options [nop,nop,TS val 807027987 ecr 807027987], length 0
23:23:13.860500 IP 127.0.0.1.8999 > 127.0.0.1.45056: Flags [P.], seq 1:4097, ack 1, win 512, options [nop,nop,TS val 807027987 ecr 807027987], length 4096
23:23:13.860505 IP 127.0.0.1.45056 > 127.0.0.1.8999: Flags [.], ack 4097, win 491, options [nop,nop,TS val 807027987 ecr 807027987], length 0
23:23:13.860513 IP 127.0.0.1.8999 > 127.0.0.1.45056: Flags [P.], seq 4097:8193, ack 1, win 512, options [nop,nop,TS val 807027987 ecr 807027987], length 4096
23:23:13.860516 IP 127.0.0.1.45056 > 127.0.0.1.8999: Flags [.], ack 8193, win 475, options [nop,nop,TS val 807027987 ecr 807027987], length 0
23:23:13.860521 IP 127.0.0.1.8999 > 127.0.0.1.45056: Flags [P.], seq 8193:12289, ack 1, win 512, options [nop,nop,TS val 807027987 ecr 807027987], length 4096
23:23:13.860523 IP 127.0.0.1.45056 > 127.0.0.1.8999: Flags [.], ack 12289, win 459, options [nop,nop,TS val 807027987 ecr 807027987], length 0
23:23:13.860528 IP 127.0.0.1.8999 > 127.0.0.1.45056: Flags [P.], seq 12289:16385, ack 1, win 512, options [nop,nop,TS val 807027987 ecr 807027987], length 4096
23:23:13.860530 IP 127.0.0.1.45056 > 127.0.0.1.8999: Flags [.], ack 16385, win 443, options [nop,nop,TS val 807027987 ecr 807027987], length 0
23:23:13.860534 IP 127.0.0.1.8999 > 127.0.0.1.45056: Flags [P.], seq 16385:20481, ack 1, win 512, options [nop,nop,TS val 807027987 ecr 807027987], length 4096
23:23:13.860537 IP 127.0.0.1.45056 > 127.0.0.1.8999: Flags [.], ack 20481, win 427, options [nop,nop,TS val 807027987 ecr 807027987], length 0
23:23:13.860541 IP 127.0.0.1.8999 > 127.0.0.1.45056: Flags [P.], seq 20481:24577, ack 1, win 512, options [nop,nop,TS val 807027987 ecr 807027987], length 4096
23:23:13.860543 IP 127.0.0.1.45056 > 127.0.0.1.8999: Flags [.], ack 24577, win 411, options [nop,nop,TS val 807027987 ecr 807027987], length 0
23:23:13.860547 IP 127.0.0.1.8999 > 127.0.0.1.45056: Flags [P.], seq 24577:28673, ack 1, win 512, options [nop,nop,TS val 807027987 ecr 807027987], length 4096
23:23:13.875167 IP 127.0.0.1.8999 > 127.0.0.1.45056: Flags [P.], seq 69633:73729, ack 1, win 512, options [nop,nop,TS val 807028002 ecr 807027987], length 4096
23:23:13.903147 IP 127.0.0.1.45056 > 127.0.0.1.8999: Flags [.], ack 73729, win 214, options [nop,nop,TS val 807028030 ecr 807027987,nop,nop,sack 1 {69633:73729}], length 0
23:23:14.115315 IP 127.0.0.1.8999 > 127.0.0.1.45056: Flags [P.], seq 73729:101121, ack 1, win 512, options [nop,nop,TS val 807028242 ecr 807028030], length 27392
23:23:14.115398 IP 127.0.0.1.45056 > 127.0.0.1.8999: Flags [.], ack 101121, win 0, options [nop,nop,TS val 807028242 ecr 807028242], length 0
23:23:14.327301 IP 127.0.0.1.8999 > 127.0.0.1.45056: Flags [.], ack 1, win 512, options [nop,nop,TS val 807028454 ecr 807028242], length 0
23:23:14.327367 IP 127.0.0.1.45056 > 127.0.0.1.8999: Flags [.], ack 101121, win 0, options [nop,nop,TS val 807028454 ecr 807028242], length 0
23:23:14.747196 IP 127.0.0.1.8999 > 127.0.0.1.45056: Flags [.], ack 1, win 512, options [nop,nop,TS val 807028874 ecr 807028454], length 0
23:23:15.611402 IP 127.0.0.1.8999 > 127.0.0.1.45056: Flags [.], ack 1, win 512, options [nop,nop,TS val 807029738 ecr 807028454], length 0
23:23:15.611431 IP 127.0.0.1.45056 > 127.0.0.1.8999: Flags [.], ack 101121, win 0, options [nop,nop,TS val 807029738 ecr 807028242], length 0
23:23:17.307212 IP 127.0.0.1.8999 > 127.0.0.1.45056: Flags [.], ack 1, win 512, options [nop,nop,TS val 807031434 ecr 807029738], length 0
23:23:17.307255 IP 127.0.0.1.45056 > 127.0.0.1.8999: Flags [.], ack 101121, win 0, options [nop,nop,TS val 807031434 ecr 807028242], length 0
23:23:20.731217 IP 127.0.0.1.8999 > 127.0.0.1.45056: Flags [.], ack 1, win 512, options [nop,nop,TS val 807034858 ecr 807031434], length 0
23:23:20.731279 IP 127.0.0.1.45056 > 127.0.0.1.8999: Flags [.], ack 101121, win 0, options [nop,nop,TS val 807034858 ecr 807028242], length 0
2023/08/06 23:23:23 write to client error write tcp 127.0.0.1:8999->127.0.0.1:45056: i/o timeout
23:23:23.862049 IP 127.0.0.1.8999 > 127.0.0.1.45056: Flags [R.], seq 101121, ack 1, win 512, options [nop,nop,TS val 807037989 ecr 807034858], length 0
2023/08/06 23:23:23 server close sock

```
