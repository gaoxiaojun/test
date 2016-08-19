title: io_operations
date: 2016-08-18 08:55:42
tags: [C++, Boost]
---

## 介绍

IO操作是任何分布式应用的网络基础设施的关键操作。它们直接参与数据交换的过程。输入操作用来接收数据，输出操作用来发送数据。

### IO缓冲区

网络编程都是关于通过计算机网络进行进程间通信。像其他类型的IO操作一样，网络IO操作涉及使用内存缓冲区。

### 同步和异步IO操作

Boost.Asio支持两张那类型的IO操作：同步和异步。同步操作阻塞当前线程的执行直到操作完成。异步操作初始化时关联一个回调函数，当操作完成时，Boost.Asio会调用回调函数。

在一个或多个异步操作初始化后，程序使用一个线程来运行Boost.Asio库。Boost.Asio库使用这个线程运行事件循环并调用回调函数通知异步操作完成，异步操作的结果作为参数传递给回调函数。

### 附加操作

取消之前初始化的异步操作的能力很重要。它允许程序声明之前初始化的异步操作不再有效。

关闭套接字很有用，如果需要通知另一个程序整个报文已发送。

## 使用固定长度IO缓冲区

固定长度的IO缓冲区通常用在报文长度已知的情况下。在Boost.Asio里面，固定长度IO缓冲区由asio::mutable_buffer或asio::const_buffer表示。asio::mutable_buffer表示可写缓冲区，asio::const_buffer表示只读缓冲区。

但是asio::mutable_buffer和asio::const_buffer并没有直接在Boost.Asio的IO函数中直接使用。相反MutableBufferSequence和ConstBufferSequence概念被引入。MutableBufferSequence指定一个对象表示asio::mutable_buffer对象的集合。相应地，ConstBufferSequence指定一个对象表示asio::const_buffer对象的集合。

asio::buffer()自由函数拥有28个重载形式，接收多种缓冲区表示形式并返回一个asio::mutable_buffers_1或asio::const_buffers_1对象。如果传给asio::buffer()函数的参数是只读类型，则返回asio::const_buffers_1对象，反之返回asio::mutable_buffers_1对象。asio::mutable_buffers_1和asio::const_buffers_1是asio::mutable_buffer和asio::const_buffer相应的适配。

### 为输出操作准备缓冲区

1. 分配一个缓冲区。
2. 将输出数据填入缓冲区。
3. 将缓冲区表示为满足ConstBufferSequence需求的对象。
4. 缓冲区已经可以用在Boost.Asio的输出函数或方法。

```c++
std::string buf = "Hello";
asio::const_buffer_1 output_buf = asio::buffer(buf);
// output_buf can be used in Boost.Asio output operations
```
为了更好的理解为什么需要将缓冲区表示成满足ConstBufferSequence需求，可以看一下send()的声明：

```c++
template <typename ConstBufferSequence>
std::size_t send(const ConstBufferSequence& buffers);
```
为了使用send()发送string对象，我们可以这样做：

```c++
asio::const_buffer asio_buf(buf.c_str(), buf.size());
std::vector<asio::const_buffer> buffers_sequence;
buffers_sequence.push_back(asio_buf);
```

但是这样没有使用asio::buffer()方便。

### 为输入操作准备缓冲区

1. 分配一个缓冲区。缓冲区的大小必须足以保存接收到的数据。
2. 将缓冲区表示为满足MutableBufferSequence需求的对象。
3. 缓冲区已经可以用在Boost.Asio的输入函数或方法。

```c++
const size_t BUF_SIZE_BYTES = 20;
std::unique_ptr<char[]> buf(new char[BUF_SIZE_BYTES]);
asio::mutable_buffers_1 input_buf =
    asio::buffer(static_cast<void*>(buf.get()),
        BUF_SIZE_BYTES);
// input_buf can be used in Boost.Asio input operations
```

> 缓冲区所有权
asio::mutable_buffer，asio::const_buffer，asio::mutable_buffers_1，asio::const_buffers_1等并没有原始缓冲区的所有权，它们只提供访问缓冲区的接口，不控制其生命周期。


