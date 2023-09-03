# 域名及网络地址

## 域名系统

https://superuser.com/questions/203674/how-to-clear-flush-the-dns-cache-in-google-chrome


> ping can employ various different ways of getting an IP address, (all listed in /etc/nsswitch.conf), while nslookup only asks the designated nameserver.

https://unix.stackexchange.com/questions/638520/what-is-the-difference-between-ping-and-nslookup-in-name-resolution

https://jdebp.uk/FGA/nslookup-flaws.html


## IP地址和域名之间的转换

### 利用域名获取IP地址

``` c
#include <netdb.h>

struct hostent * gethostbyname(const char * hostnname);
```

>  The gethostbyname*(), gethostbyaddr*(), herror(), and hstrerror() functions are obsolete.
>
>  Applications should use getaddrinfo(3), getnameinfo(3), and gai_strerror(3) instead.

https://man7.org/linux/man-pages/man3/gethostbyname.3.html

https://man7.org/linux/man-pages/man5/nsswitch.conf.5.html


### 利用IP地址获取域名

``` c
#include <netdb.h>

struct hostent * gethostbyaddr(const char * addr, socklen_t len, int family);
```

``` bash
$ ./hostaddr 8.8.4.4
Official name: dns.google
Aliases 1: 4.4.8.8.in-addr.arpa
Address type: AF_INET
IP addr 1: 8.8.4.4
```

https://unix.stackexchange.com/questions/56941/what-is-the-point-of-sshd-usedns-option

https://pkg.go.dev/net#hdr-Name_Resolution

https://www.cloudflare.com/learning/dns/dns-records/dns-ptr-record/

https://plantegg.github.io/2019/06/09/一文搞懂域名解析相关问题/

