# 习题

1. 请说明TCP/IP的四层协议栈，并说明TCP和UDP套接字经过的层级结构差异。

链路层-IP层-传输层-应用层

2. 请说出TCP/IP协议栈中链路层和IP层的作用，并给出二者关系。

链路层是frame，IP层是Packet。对链路层而言IP层的Packet是payload。

3. 为何需要把TCP/IP协议栈分为4层（或7层）？结合开放式系统回答？

可以屏蔽实现细节，对外提供抽象接口，各个层次可以独自演进。

4. 客户端调用 `connect` 函数向服务器端发送连接请求。服务器端调用哪个函数后，客户端可以调用 `connect` 函数？

`listen`

5. 什么时候创建连接请求队列？它有何作用？与 `accept` 有什么关系？

`listen`

请求队列用来缓存三次握手之后但是还没被 `accept` 取出的连接

https://www.alibabacloud.com/blog/tcp-syn-queue-and-accept-queue-overflow-explained_599203

6. 客户端中为何不需要调用 `bind` 函数分配地址？如果不调用 `bind` 函数，那何时、如何向套接字分配IP地址和端口号？

客户端socket的地址端口通常是操作系统随机分配的，为什么不需要用 `bind` 是因为大部分情况对客户端的地址端口没有要求，这一点是和服务器端相反的，因为服务器端通常是被连接的一端，因此需要指定固定端口。

https://stackoverflow.com/questions/21645970/in-socket-programming-why-a-client-is-not-bind-to-an-address

https://stackoverflow.com/questions/8527410/how-to-force-client-in-udp-to-open-port-when-sending-with-sendto/8527652#8527652

```
// 主动发起tcp连接
connect(fd, servaddr, addrlen);
-> sock->ops->connect() == inet_stream_connect
-> tcp_v4_connect()
    -> inet_hash_connect()
        -> __inet_hash_connect()
           /*
           如果指定了port，则使用指定的port作为客户端端口
           否则，随机选取一个port
           */
           // 端口可用性检查
             -> check_established()
           // bind端口
             -> inet_bind_bucket_create
             -> inet_bind_hash
```
https://zhuanlan.zhihu.com/p/587402192


``` c
/*
 *	Automatically bind an unbound socket.
 */

static int inet_autobind(struct sock *sk)
{
	struct inet_sock *inet;
	/* We may need to bind the socket. */
	lock_sock(sk);
	inet = inet_sk(sk);
	if (!inet->inet_num) {
		if (sk->sk_prot->get_port(sk, 0)) {
			release_sock(sk);
			return -EAGAIN;
		}
		inet->inet_sport = htons(inet->inet_num);
	}
	release_sock(sk);
	return 0;
}
```

https://elixir.bootlin.com/linux/latest/source/net/ipv4/af_inet.c#L172


7. 把第1章的hello_server.c和hello_server_win.c改成迭代服务器端，并利用客户端测试更改是否准确。
