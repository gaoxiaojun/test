title: 基础
date: 2016-08-16 08:45:19
tags: [C/C++]
---

## 介绍

TCP协议是具有下列特性的传输层协议：

- 它是可靠的。这意味着TCP协议保证报文以正确的顺序传输，或者通知报文没有传输成功。TCP协议包含错误处理机制。
- 它假定建立逻辑连接。在一个程序通过TCP协议与另一个程序通信之前，它必须根据标准通过交换服务报文建立一个逻辑连接。
- 它假定点对点通信模型。也就是在单个连接上只有两个程序可以通信，不支持多播消息。
- 它是面向数据流的。这意味着一个程序发送给另一个程序的数据会被TCP协议解释为字节流。

UDP协议是一个与TCP协议不同的传输层协议，它具有下列特性：

- 它是不可靠的。这意味着如果一个发送者通过UDP协议发送一个报文，不保证报文会被发送。UDP协议不会尝试检查或修复任何错误。开发者负责所有的错误处理。
- 它是无连接的。这意味着在程序通信之前不需要建立连接。
- 它支持一对一或一对多通信模型。UDP协议支持多播消息。
- 它是面向数据报的。这意味着UDP协议将数据解释为特定大小的报文并且会尝试将报文作为一个整体发送。数据报文要么被作为一个整体发送，要么当发送失败时根本不会发送。

因为UDP协议是不可靠的，它通常被用在可靠的局域网。如果想在因特网使用UDP协议，开发者必须实现错误处理机制。当需要在因特网传输时，因为可靠性，TCP协议通常是最好的选择。

## 创建一个端点

端点服务两个目标：

- 客户端程序使用端点指定一个想要通信的特定的服务端程序。
- 服务端程序使用端点指定一个本地的IP地址和端口，在此之上接收来自客户端的报文。

### 客户端创建端点

1. 获取服务器程序的IP地址和端口。IP地址应该是点号隔开的字符串（IPv4）或者16进制的字符串（IPv6）。
2. 使用一个asio::ip::address对象代表IP地址。
3. 用第2步的创建的地址和端口初始化一个asio::ip::tcp::endpoint对象。

```c++
#include <boost/asio.hpp>

std::string raw_ip_address = "127.0.0.1";
unsigned short port_num = 3333;

boost::system::error_code ec;
asio::ip::address ip_address = asio::ip::address::from_string(raw_ip_address, ec);
if (ec.value() != 0) {
    // handle error
}

asio::ip::tcp::endpoint ep(ip_address, port_num);
```

### 服务端创建端点

1. 获取一个服务器将要监听请求的端口。
2. 创建一个特殊的asio::ip::address对象带包这个服务器所有可用的IP地址。
3. 使用address对象和端口初始化一个asio::ip::tcp::endpoint对象。

```c++
#include <boost/asio.hpp>

unsigned short port_num = 3333;

// IPv6
asio::ip::address ip_address = asio::ip::address_v6::any();
asio::ip::tcp::endpoint ep(ip_address, port_num);
```

Boost.Asio提供3个类用来表示IP地址：

- asio::ip::address_v4，代表IPv4地址
- asio::ip::address_v6，代表IPv6地址
- asio::ip::address，代表IPv4或IPv6地址

from_string()是asio::ip::address的静态方法，它有4个重载形式：

```c++
static asio::ip::address from_string(
    const char* str);
    
static asio::ip::address from_string(
    const char* str,
    boost::system::error_code& ec);
    
static asio::ip::address from_string(
    const std::string& str);
    
static asio::ip::address from_string(
    const std::string& str,
    boost::system::error_code& ec);
```

为了表示主机可用的所有IP地址，asio::ip::address_v4和asio::ip::address_v6提供了一个静态方法any()。

asio::ip::tcp::endpoint其实是basic_endpoint<>模板的一个特例。

```c++
typedef basic_endpoint<tcp> endpoint;
```

同样asio::ip::udp::endpoint也是basic_endpoint<>模板的一个特例。

```c++
typedef basic_endpoint<udp> endpoint;
```

## 创建一个主动套接字

基本上，套接字有两种类型。主动套接字用来发起一个连接建立过程。被动套接字用来被动地等待传入的连接请求。

1. 创建一个asio::io_service实例，或者使用之前创建的。
2. 创建一个代表传输层协议（TCP或UDP）和下层IP协议（IPv4或IPv6）的类对象。
3. 创建一个代表套接字的对象，传递asio::io_service给它的构造函数。
4. 调用open()方法，传递协议参数给它。