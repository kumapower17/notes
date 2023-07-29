# 地址族与数据序列

## 3.1 分配给套接字的IP地址和端口号

>>> IP地址分为两类，IPv4和IPv6。

>>> The Internet Stream Protocol (ST) is a family of experimental protocols first defined in Internet Experiment Note IEN-119 in 1979,[1] and later substantially revised in RFC 1190 (ST-II) and RFC 1819 (ST2+).[2][3][4] The protocol uses the version number 5 in the version field of the Internet Protocol header, but was never known as IPv5. The successor to IPv4 was thus named IPv6 to eliminate any possible confusion about the actual protocol in use.

https://en.wikipedia.org/wiki/Internet_Stream_Protocol

### 网络地址分类与主机地址边界

>>> 只需要通过IP地址的第一个字节即可判断网络地址占用的字节数，因为我们根据IP地址的边界区分网络地址，如下所示：
>>> A类地址的首位以0开始  
>>> B类地址的前2位以10开始  
>>> C类地址的前3位以110开始

书中上段的说明是不准确的，目前普遍使用的是 [CIDR](https://aws.amazon.com/what-is/cidr/)。

### 用来区分套接字的端口号

>>> 另外，虽然端口号不能重复，但TCP套接字和UDP套接字不会共用端口号，所以允许重复。例如：如果某TCP套接字使用9190号端口，则其他TCP套接字就无法使用该端口号，但UDP套接字可以使用。

上面还没有提地址，比如tcp 127.0.0.1:9190和tcp 192.168.2.104:9190并不冲突：

``` bash
$ nc -l 192.168.2.104 9190 &
$ nc -l 127.0.0.1 9190
```

https://stackoverflow.com/questions/14388706/how-do-so-reuseaddr-and-so-reuseport-differ

golang http server默认开启了 `SO_REUSEADDR` 选项：

``` bash
root@primary:/home/ubuntu# strace -e trace=setsockopt ./resuseaddr
setsockopt(3, SOL_SOCKET, SO_REUSEADDR, [1], 4) = 0
^Cstrace: Process 1511 detached
```

## 3.2 地址信息的表示

>>> The actual structure passed for the addr argument will depend on
>>>       the address family.  The sockaddr structure is defined as
>>>       something like:
>>>
>>>           struct sockaddr {
>>>               sa_family_t sa_family;
>>>               char        sa_data[14];
>>>           }
>>>
>>>       The only purpose of this structure is to cast the structure
>>>       pointer passed in addr in order to avoid compiler warnings.  See
>>>       EXAMPLES below.

https://stackoverflow.com/questions/18609397/whats-the-difference-between-sockaddr-sockaddr-in-and-sockaddr-in6


https://stackoverflow.com/questions/42178179/will-casting-around-sockaddr-storage-and-sockaddr-in-break-strict-aliasing#answer-42178347


https://man7.org/linux/man-pages/man3/getaddrinfo.3.html


## 3.3 网络字节序与地址变换

>>> Danny Cohen introduced the terms big-endian and little-endian into computer science for data ordering in an Internet Experiment Note published in 1980.
>>> The adjective endian has its origin in the writings of 18th century Anglo-Irish writer Jonathan Swift. In the 1726 novel Gulliver's Travels, he portrays the conflict between sects of Lilliputians divided into those breaking the shell of a boiled egg from the big end or from the little end. As a boy, the grandfather of the emperor whom Gulliver met had cut his finger while opening an egg from the big end. The boy's father and emperor at the time published an imperial edict commanding all his subjects to break their eggs from the small end. The people resented the change, sparking six rebellions of "Big-Endians."

https://en.wikipedia.org/wiki/Endianness#Etymology
