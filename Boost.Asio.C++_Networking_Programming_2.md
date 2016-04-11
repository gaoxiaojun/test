# 网络API #

这部分展示为了使用Boost.Asio编写网络程序你必须知道的内容。

## Boost.Asio命名空间 ##

Boost.Asio所有东西都放在boost::asio命名空间或者子命名空间：

* boost::asio：核心类和函数放在这里。重要的类有io_service和streambuf。重要的自由函数，比如read，read_at，read_util，它们相应的异步版本，以及同步写和异步写函数。
* boost::asio::ip：网络部分属于这里。重要的类有address，endpoint，tcp，udp，icmp。重要的自由函数connect和async_connect。注意boost::asio::ip::tcp::socket只是一个boost::asio::ip::tcp中的typedef。
* boost::asio::error：包含调用I/O例程的错误码。
* boost::asio::ssl：包含处理SSL的类。
* boost::asio::local：包含POSIX相关的类。
* boost::asio::windows：包含Windows相关的类。

## IP地址 ##

为了处理IP地址，Boost.Asio提供ip::address，ip::address_v4和ip::address_v6类。以下是一些最重要的函数：

* ip::address(v4_or_v6_address)：这个函数转换一个v4或v6地址为ip::address。
* ip::address::from_string(str)：从一个IPv4或IPv6地址字符串创建地址。
* ip::address::to_string()：返回一个友好可读的地址字符串。
* ip::address_v4::broadcast([addr, mask])：创建一个广播地址。
* ip::address_v4::any()：返回一个代表任何地址的地址。
* ip::address_v4::loopback()，ip_address_v6::loopback()：返回回路地址
* ip::host_name()：返回当前主机名。

你可能最常用到ip::address::from_string：

	ip::address addr = ip::address::from_string("127.0.0.1");

## 端点 ##

端点是你要连接的地址和端口。不同类型有自己的endpoint类，比如ip::tcp::endpoint，ip::udp::endpoint和ip::icmp::endpoint。

如果你想连接本机80端口，这样：

	ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 80);

创建端点有3种方法：

* endpoint()：默认构造函数，有时候可以用于UDP/ICMP套接字。
* endpoint(protocol, port)：通常用于服务端套接字接受新连接。
* endpoint(addr, port)：创建一个指定地址和端口的端点。

下面是例子：

	ip::tcp::endpoint ep1;
	ip::tcp::endpoint ep2(ip::tcp::v4(), 80);
	ip::tcp::endpoint ep3(ip::address::from_string("127.0.0.1"), 80);

如果想要连接到一个主机名，可以这样做：

	// outputs "87.248.122.122"
	io_service service;
	ip::tcp::resolver resolver(service);
	ip::tcp::resolver::query query("www.yahoo.com", "80");
	ip::tcp::resolver::iterator iter = resolver.resolve(query);
	ip::tcp::endpoint ep = *iter;
	std::cout << ep.address().to_string() << std::endl;

如果resolve()函数成功，它将返回至少一个入口。你可以使用第一个，也可以遍历所有的。

## 套接字 ##

Boost.Asio有3中socket类：ip::tcp，ip::udp和ip::icmp，并且可以扩展。你可以创建自己的socket类。

你可以认为ip::tcp，ip::udp和ip::icmp类是占位符，它们提供其他类和函数的简便访问：

* ip::tcp::socket, ip::tcp::acceptor, ip::tcp::endpoint, ip::tcp::resolver, ip::tcp::iostream
* ip::udp::socket, ip::udp::endpoint, ip::udp::resolver
* ip::icmp::socket, ip::icmp::endpoint, ip::icmp::resolver

socket类创建相应的套接字，创建时需要传递io_service实例：

	io_service service;
	ip::udp::socket sock(service)
	sock.set_option(ip::udp::socket::reuse_address(true));

每一个socket名字是一个typedef：

* ip::tcp::socket = basic_stream_socket\<tcp\>
* ip::udp::socket = basic_datagram_socket\<udp\>
* ip::icmp::socket = basic_raw_socket\<icmp\>

## 同步错误码 ##

所有的同步函数重载了抛出异常或返回错误码的版本，像下面的例子：

	sync_func(arg1, arg2 ... argN); // throws
	boost::system::error_code ec;
	sync_func(arg1 arg2, ..., argN, ec); // returns error code

## Socket成员函数 ##

不是所有的函数对每一种类型的socket都可用。注意所有的异步函数立刻返回，同步函数只有在操作完成才会返回。

### 连接相关的函数 ###

* assign(protocol, socket)：这个函数将一个原始套接字赋给一个socket实例。使用它处理遗留代码。
* open(protocol)：这个函数使用指定IP协议（v4或v6）打开一个socket。这个函数主要用于UDP/ICMP套接字，或者服务器套接字。
* bind(endpoint)：绑定到指定端点。
* connect(endpoint)：同步连接到指定端点。
* async_connect(endpoint)：异步连接到指定端点。
* is_open()：如果socket已打开返回true。
* close()：关闭套接字。此套接字任何异步操作立刻取消并以error::operation_aborted错误码结束。
* shutdown(type_of_shutdown)：从现在开始禁止send，receive操作。
* cancel()：取消此套接字所有异步操作。所有异步操作立刻以error::operation_aborted错误码结束。

例子如下：

	ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 80);
	ip::tcp::socket sock(service);
	sock.open(ip::tcp::v4());
	sock.connect(ep);
	sock.write_some(buffer("GET /index.html\r\n"));
	char buff[1024]; sock.read_some(buffer(buff,1024));
	sock.shutdown(ip::tcp::socket::shutdown_receive);
	sock.close();

### 读写函数 ###

对于所有异步读写函数，其处理函数的签名为：

	void handler(const boost::system::error_code &e, size_t bytes)

* async_receive(buffer, [flags,] handler)：从套接字开始异步receive操作。
* async_read_some(buffer, handler)：等价于async_receive(buffer, handler)。
* async_receive_from(buffer, endpoint[, flags], handler)：从指定端点开始异步receive操作。
* async_send(buffer[, flags], handler)：开始异步发送buffer中的数据。
* async_write_some(buffer, handler)：等价于async_send(buffer, handler)。
* async_send_to(buffer, endpoint, handler)：开始异步发送buffer中的数据到指定端点。
* receive(buffer[, flags])：同步读取数据到指定buffer。这个函数阻塞知道收到数据或一个错误发生。
* read_some(buffer)：等价于receive(buffer)。
* receive_from(buffer, endpoint[, flags])：