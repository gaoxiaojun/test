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

### 同步错误码 ###

所有的同步函数重载了抛出异常或返回错误码的版本，像下面的例子：

	sync_func(arg1, arg2 ... argN); // throws
	boost::system::error_code ec;
	sync_func(arg1 arg2, ..., argN, ec); // returns error code

### Socket成员函数 ###

不是所有的函数对每一种类型的socket都可用。注意所有的异步函数立刻返回，同步函数只有在操作完成才会返回。

#### 连接相关的函数 ####

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

#### 读写函数 ####

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
* receive_from(buffer, endpoint[, flags])：同步地从指定端点读取数据到指定buffer。这个函数阻塞直到受到数据或错误发生。
* send(buffer[,  flags])：同步发送buffer中的数据。函数阻塞直到受到数据或发生错误。
* write_some(buffer)：等价于send(buffer)。
* send_to(buffer, endpoint[, flags])：同步发送buffer中的数据到指定端点。函数阻塞直到受到数据或发生错误。
* avaliable()：返回可以同步读取的字节数，不用阻塞。

flags的默认值是0，但可以是下列的组合：

* ip::socket_type::socket::message_peek：这个标志只是偷看消息。它返回这个消息，但是下一次调用会重读这个消息。
* ip::socket_type::socket::message_out_of_band：这个标志处理out-of-band（OOB）数据。OOB数据是标记为比普通数据更重要的数据。
* ip::socket_type::socket::message_do_not_route：这个标志指定消息应该不使用路由表发送。
* ip::socket_type::socket::message_end_of_record：这个标志指示数据标志一个记录的结尾。在Windows下不支持。

如果你使用下面这段代码，你最可能使用message_peek：

	char buff[1024];
	sock.receive(buffer(buff), ip::tcp::socket::message_peek);
	memset(buff,1024, 0);
	// re-reads what was previously read
	sock.receive(buffer(buff));

同步读写一个TCP套接字：

	ip::tcp::endpoint ep( ip::address::from_string("127.0.0.1"), 80);
	ip::tcp::socket sock(service);
	sock.connect(ep);
	sock.write_some(buffer("GET /index.html\r\n"));
	std::cout << "bytes available " << sock.available() << std::endl;
	char buff[512];
	size_t read = sock.read_some(buffer(buff));

同步读写一个UDP套接字：

	ip::udp::socket sock(service);
	sock.open(ip::udp::v4());
	ip::udp::endpoint receiver_ep("87.248.112.181", 80);
	sock.send_to(buffer("testing\n"), receiver_ep);
	char buff[512];
	ip::udp::endpoint sender_ep;
	sock.receive_from(buffer(buff), sender_ep);

注意为了使用receive_from从一个UDP套接字读取数据，你需要一个默认构造的端点。

异步读取一个UDP服务器套接字：

	using namespace boost::asio;
	io_service service;
	ip::udp::socket sock(service);
	boost::asio::ip::udp::endpoint sender_ep;
	char buff[512];
	void on_read(const boost::system::error_code & err, std::size_t read_bytes) {
	    std::cout << "read " << read_bytes << std::endl;
	    sock.async_receive_from(buffer(buff), sender_ep, on_read);
	}
	int main(int argc, char* argv[]) {
	    ip::udp::endpoint ep(ip::address::from_string("127.0.0.1"), 8001);
	    sock.open(ep.protocol());
	    sock.set_option(boost::asio::ip::udp::socket::reuse_address(true));
	    sock.bind(ep);
	    sock.async_receive_from(buffer(buff,512), sender_ep, on_read);
	    service.run();
	}

#### 套接字控制 ####

这些函数处理高级套接字选项：

* get_io_service()：返回构造时传递的io_service实例。
* get_option(option)：返回一个套接字选项。
* set_option(option)：设置套接字选项
* io_control(cmd)：在套接字上执行一个I/O命令。

以下是你可以读取或设置的套接字选项：

| 名字 | 描述 | 类型 |
| -- | -- | -- |
| broadcast | 如果真，允许广播消息 | bool |
| debug | 如果真，使socket-level调试生效 | bool |
| enable_connection_aborted | 如果真，报告在accept()时连接被中止 | bool |
| receive_buffer_size | 套接字的接收缓冲区大小 | int |
| receive_low_watermark | 提供处理套接字输入的最小字节数 | int |
| reuse_address | 如果真，套接字可以绑定一个已经在使用的地址 | bool |
| send_buffer_size | 套接字发送缓冲区大小 | int |
| send_low_watermark | 提供处理套接字输出的最小字节数 | int |
| ip::v6_only | 如果真，只允许IPv6通信 | bool |

每一个名字代表内部socket的一个typedef或一个类。以下是如何使用的例子：

	ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 80);
	ip::tcp::socket sock(service);
	sock.connect(ep);
	// TCP socket can reuse address
	ip::tcp::socket::reuse_address ra(true);
	sock.set_option(ra);
	// get sock receive buffer size
	ip::tcp::socket::receive_buffer_size rbs;
	sock.get_option(rbs);
	std::cout << rbs.value() << std::endl;
	// set sock's buffer size to 8192
	ip::tcp::socket::send_buffer_size sbs(8192);
	sock.set_option(sbs);

#### TCP vs UDP vs ICMP ####

如果一个成员函数不在下表，说明它对所有socket类可用：

| 名字 | TCP | UDP | ICMP |
| -- | -- | -- | -- |
| async_read_some | Yes | - | - |
| async_receive_from | - | Yes | Yes |
| async_write_some | Yes | - | - |
| async_send_to | - | Yes | Yes |
| read_some | Yes | - | - |
| receive_from | - | Yes | Yes |
| write_some | Yes | - | - |
| send_to | - | Yes | Yes |

#### 其它函数 ####

* local_endpoint()：返回本地地址。
* remote_endpoint()：返回远程地址。
* native_handle()：返回原始套接字的句柄。你只有在想要调用原始函数的时候需要这个函数。
* non_blocking()：如果套接字非阻塞返回true，否则false。
* native_non_blocking()：和non_blocking一样，但是它在原始套接字上调用原始的套接字API。
* at_mark()：如果套接字即将读取OOB数据，返回true。很少使用。

### 其它考虑 ###

最后需要注意的是，一个套接字实例不能被复制，因为复制构造函数和赋值操作符无法访问。如果你想要创建拷贝，使用shared指针：

	typedef boost::shared_ptr<ip::tcp::socket> socket_ptr;
	socket_ptr sock1(new ip::tcp::socket(service));
	socket_ptr sock2(sock1); // ok
	socket_ptr sock3;
	sock3 = sock1; // ok

#### Socket缓冲区 ####

当读取或写入一个套接字，需要一个缓冲区，它将保存进来和流出的数据。缓冲区的内存必须比I/O操作更长久。你必须保证I/O操作持续期间它不被释放或超出范围。

针对上面的问题有几种解决方法：

* 使用全局缓冲区。
* 创建一个缓冲区，当操作完成时销毁它。
* 使用一个连接对象维护套接字和额外的数据，比如缓冲区。

#### 缓存包装器函数 ####

任何时候我们需要一个缓冲区用于读写操作时，用buffer()函数包装真实的缓冲区对象。它包装任何缓冲区为一个类，允许Boost.Asio函数迭代访问缓冲区。比如你使用下面的代码：

    sock.async_receive(some_buffer, on_read);

some_buffer实例需要满足一些条件，即ConstBufferSequence或MutableBufferSequence。使用buffer()函数就可以满足这些条件。

简单地说，你可以包装下面的空间进buffer()函数：

* char[] const数组
* void*指针和大小
* std::string
* POD[] const数组（POD代表plain old data，即构造函数和析构函数不用做任何事）
* std::vector<POD>
* boost::array<POD>
* std::array<POD>

## 读/写/连接自由函数 ##

Boost.Asio提供自由函数处理I/O。

### 连接函数 ###

连接socket到一个端点：

* connect(socket, begin[, end][, condition])：同步连接，尝试begin和end之间每一个端点。begin迭代器是socket_type::resolver::query调用返回的结果。你可以提供一个条件函数在每次连接尝试前调用。它的签名是Iterator connect_condition(const boost::system::error_code &ec, Iterator next)。
* async_connect(socket, begin[, end][, condition], handler)：执行异步连接，最后调用完成处理函数。处理函数的签名是void handler(const boost::system::error_code &ec, Iterator iterator)。

示例如下：

	using namespace boost::asio::ip;
	tcp::resolver resolver(service);
	tcp::resolver::iterator iter = resolver.resolve(
		tcp::resolver::query("www.yahoo.com",
		"80"));
	tcp::socket sock(service);
	connect(sock, iter);

一个主机名可以解析为多个地址，因此connect和async_connect将你从尝试每一个地址的负担中释放出来。

### 读/写函数 ###