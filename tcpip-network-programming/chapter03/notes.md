# 地址族与数据序列

## 3.1 分配给套接字的IP地址和端口号

> IP地址分为两类，IPv4和IPv6。

> The Internet Stream Protocol (ST) is a family of experimental protocols first defined in Internet Experiment Note IEN-119 in 1979,[1] and later substantially revised in RFC 1190 (ST-II) and RFC 1819 (ST2+).[2][3][4] The protocol uses the version number 5 in the version field of the Internet Protocol header, but was never known as IPv5. The successor to IPv4 was thus named IPv6 to eliminate any possible confusion about the actual protocol in use.

https://en.wikipedia.org/wiki/Internet_Stream_Protocol

### 网络地址分类与主机地址边界

> 只需要通过IP地址的第一个字节即可判断网络地址占用的字节数，因为我们根据IP地址的边界区分网络地址，如下所示：\
> A类地址的首位以0开始\
> B类地址的前2位以10开始\
> C类地址的前3位以110开始

书中上段的说明是不准确的，目前普遍使用的是 [CIDR](https://aws.amazon.com/what-is/cidr/)。

https://unix.stackexchange.com/questions/419880/connecting-to-ip-0-0-0-0-succeeds-how-why

### 用来区分套接字的端口号

> 另外，虽然端口号不能重复，但TCP套接字和UDP套接字不会共用端口号，所以允许重复。例如：如果某TCP套接字使用9190号端口，则其他TCP套接字就无法使用该端口号，但UDP套接字可以使用。

上面还没有提地址，比如 `tcp 127.0.0.1:9190` 和 `tcp 192.168.2.104:9190` 并不冲突：

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

> The actual structure passed for the addr argument will depend on
>       the address family.  The sockaddr structure is defined as
>       something like:
>
>           struct sockaddr {
>               sa_family_t sa_family;
>               char        sa_data[14];
>           }
>
>       The only purpose of this structure is to cast the structure
>       pointer passed in addr in order to avoid compiler warnings.  See
>       EXAMPLES below.

https://stackoverflow.com/questions/18609397/whats-the-difference-between-sockaddr-sockaddr-in-and-sockaddr-in6


https://stackoverflow.com/questions/42178179/will-casting-around-sockaddr-storage-and-sockaddr-in-break-strict-aliasing#answer-42178347


https://man7.org/linux/man-pages/man3/getaddrinfo.3.html


## 3.3 网络字节序与地址变换

> Danny Cohen introduced the terms big-endian and little-endian into computer science for data ordering in an Internet Experiment Note published in 1980.
> The adjective endian has its origin in the writings of 18th century Anglo-Irish writer Jonathan Swift. In the 1726 novel Gulliver's Travels, he portrays the conflict between sects of Lilliputians divided into those breaking the shell of a boiled egg from the big end or from the little end. As a boy, the grandfather of the emperor whom Gulliver met had cut his finger while opening an egg from the big end. The boy's father and emperor at the time published an imperial edict commanding all his subjects to break their eggs from the small end. The people resented the change, sparking six rebellions of "Big-Endians."

https://en.wikipedia.org/wiki/Endianness#Etymology

https://www.quora.com/Is-ARM-big-endian-or-little-endian


## 3.4 网络地址的初始化与分配

``` c
int inet_aton(const char *cp, struct in_addr *inp);

in_addr_t inet_addr(const char *cp);
```

> inet_addr函数不仅可以把IP地址转成32位整数型，而且可以检测无效的IP地址。另外，从输出结果可以验证确实转换成网络字节序。

> The inet_addr() function converts the Internet host address cp from IPv4 numbers-and-dots notation into binary data in network byte order. If the input is invalid, INADDR_NONE (usually -1) is returned. Use of this function is problematic because -1 is a valid address (255.255.255.255). Avoid its use in favor of inet_aton(), inet_pton(3), or getaddrinfo(3) which provide a cleaner way to indicate error return.

https://linux.die.net/man/3/inet_ntoa

https://www.rfc-editor.org/rfc/rfc922#section-7

https://wiki.wireshark.org/DHCP

``` bash
$ tcpdump -n -r dhcp.pcap
reading from file dhcp.pcap, link-type EN10MB (Ethernet)
03:16:24.317453 IP 0.0.0.0.68 > 255.255.255.255.67: BOOTP/DHCP, Request from 00:0b:82:01:fc:42, length 272
03:16:24.317748 IP 192.168.0.1.67 > 192.168.0.10.68: BOOTP/DHCP, Reply, length 300
03:16:24.387484 IP 0.0.0.0.68 > 255.255.255.255.67: BOOTP/DHCP, Request from 00:0b:82:01:fc:42, length 272
03:16:24.387798 IP 192.168.0.1.67 > 192.168.0.10.68: BOOTP/DHCP, Reply, length 300
```

https://man7.org/linux/man-pages/man3/inet_addr.3p.html

> Values specified using the ‘.’ notation take one of the following forms:
>
>     a.b.c.d
>     a.b.c
>     a.b
>     a
>
> When four parts are specified, each is interpreted as a byte of data and assigned, from left to right, to the four bytes of an Internet address.  Note that when an
> Internet address is viewed as a 32-bit integer quantity on the VAX the bytes referred to above appear as “d.c.b.a”.  That is, VAX bytes are ordered from right to left.
>
> When a three part address is specified, the last part is interpreted as a 16-bit quantity and placed in the right-most two bytes of the network address.  This makes the three part address format convenient for specifying Class B network addresses as “128.net.host”.
>
> When a two part address is supplied, the last part is interpreted as a 24-bit quantity and placed in the right most three bytes of the network address.  This makes the two part address format convenient for specifying Class A network addresses as “net.host”.
>
> When only one part is given, the value is stored directly in the network address without any byte rearrangement.
>
> All numbers supplied as “parts” in a ‘.’ notation may be decimal, octal, or hexadecimal, as specified in the C language (i.e., a leading 0x or 0X implies hexadecimal; otherwise, a leading 0 implies octal; otherwise, the number is interpreted as decimal).

``` bash
$ cd chapter03/demo; gcc inet_addr_localhost.c && ./a.out
Network ordered integer addr: 127.0.0.1
```

https://superuser.com/questions/614001/why-can-i-ping-127-1


``` c
#include <arpa/inet.h>
const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);
```

> inet_ntop() extends the inet_ntoa(3) function to support multiple address families, inet_ntoa(3) is now considered to be deprecated in favor of inet_ntop().

https://linux.die.net/man/3/inet_ntop

> 如前所述，同一计算机中可以分配多个IP地址，实际IP地址的个数与计算机中安装的NIC的数量相等。

书中上段的论述是有问题的，即使是同一个网卡也可以支持二级地址。

https://www.garron.me/en/linux/add-secondary-ip-linux.html
