# 习题

1. 什么是协议？在收发数据中定义协议有何意义？

为了完成数据交换而定好的约定

2. 面向连接的套接字传输特性有3点，请分别说明。

有序 可靠 字节流

3. 下列哪些是面向消息的套接字的特性？

a. 传输数据可能丢失

c. 以快速传递为目的

e. 与面向连接的套接字不同，不存在连接的概念

4. 下列数据适合哪类套接字传输？并给出原因

a. 演唱会现场直播的多媒体数据 （UDP）

https://www.quora.com/Are-modern-video-streaming-services-built-on-TCP-or-UDP-Is-TCP-fast-enough-to-support-services-such-as-Netflix

https://stackoverflow.com/questions/19620219/does-youtube-stream-videos-via-tcp

b. 某人压缩过的文本文件 (TCP)

c. 网上银行用户与银行之间的数据传递（TCP）

5. 何种类型的套接字不存在数据边界？这类套接字接收数据时需要注意什么？

TCP。

[怎么解决TCP网络传输「粘包」问题？](https://www.zhihu.com/question/20210025)

应用层协议需要自己设计拆包，常见的思路：

- tlv. https://en.wikipedia.org/wiki/Type–length–value
- 分隔符. https://stackoverflow.com/questions/3508338/what-is-the-boundary-in-multipart-form-data
- 一直读到连接关闭。https://www.w3.org/Protocols/HTTP/1.0/draft-ietf-http-spec.html#BodyLength


6.
