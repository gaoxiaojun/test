title: 重载操作和转换
date: 2015-09-20 10:47:02
tags: c++
---

这是C++ primer第五版第14章的读书笔记。

# 基本概念

重载操作符是具有特殊名字的函数：关键字`operator`后面跟着需要被定义的操作符的符号。
像其它函数一样，重载操作符有返回值，参数列表和函数体。

重载操作符的参数和操作符的操作数一样多。除了函数调用操作符，`operator()`，重载操作符
没有默认实参。

>**注意**
>当重载操作符是成员函数时，`this`绑定到左操作数。因此，成员操作符函数少一个操作数。

操作符函数必须为类的成员或至少有一个类类型参数：

    // error: cannot redefine the built-in operator for ints
    int operator+(int, int);

我们可以重载大多数操作符，而且只能重载已经存在的操作符：

                    可以重载的操作符
    +       -       *       /       %       ^
    &       |       ~       !       ,       =
    <       >       <=      >=      ++      --
    <<      >>      ==      !=      &&      ||
    +=      -=      /=      %=      ^=      &=
    |=      *=      <<=     >>=     []      ()
    ->      ->*     new     new[]   delete  delete[]
                    不能重载的操作符
            ::      .*      .       ?:

重载操作符和相应的内置操作符有相同的优先级和结合性。

## 直接调用重载操作符函数

    data1 + data2;           // 表达式调用
    operator+(data1, data2); // 等价的函数调用
    data1 += data2;          // 表达式调用
    data1.operator+=(data2); // 等价的成员函数调用

## 有些操作符不应该重载

重载操作符不保留求值顺序或短路求值。

>**最佳实践**
>通常，逗号（,），取地址（&），逻辑与（&&）和逻辑或（||）不应该重载。

## 使用和内置操作符意思一致的定义

*   如果类使用IO，定义与内置类型IO操作一致的`shift`操作符
*   如果类有测试相等的操作，定义`operator==`。如果类有`operator==`，通常也应该定义`operator!=`。
*   如果类有单一的，自然顺序的操作，定义`operator<`。如果类有`operator<`，通常也应该定义其它关系操作符。
*   重载操作符的返回类型应该和内置操作符的返回类型兼容：逻辑操作符和关系操作符应该返回`bool`，算术操作符
    应该返回类的值，赋值操作符应该返回左操作数的引用。

>**注意**：明智地使用操作符重载
>当内置操作符和类的操作有逻辑映射的时候，操作符重载最有用。

## 选择成员或非成员实现

以下指导方针能够帮助我们决定是否将一个操作符定义为成员函数或普通的非成员函数：

*   赋值操作符（=），下标操作符（[]），调用操作符（()），成员访问操作符（->）必须定义为成员。
*   复合赋值操作符通常应该定义为成员，但是没有要求一定这样。
*   改变对象状态或与类对象联系紧密的操作符，通常应该定义为成员。比如++，--和解引用（*）。
*   那些可能转换操作数的对称的操作符，比如算术操作符，关系操作符，位操作符，通常应该定义为非成员。

# 输入输出操作符

## 重载输出操作符 <<

    ostream &operator<<(ostream &os, const Sales_data &item)
    {
        os << item.isbn() << " " << item.units_sold << " "
           << item.revenue << " " << item.avg_price();
        return os;
    }

>**最佳实践**
>通常输出操作符应该最小格式化输出对象的内容，不应该打印换行。

## IO操作符必须是非成员函数

输入输出操作符遵循库`iostream`的惯例必须为普通的非成员函数。

    Sales_data data;
    data << cout; // 如果operator<<是Sales_data的一个成员

如果这些操作符是任意一个类的成员，那它们应该是`istream`或`ostream`的成员，但是我们无法给标准库添加成员。
输入输出操作符通常需要读或写非公开的数据成员。因此，IO操作符通常必须声明为`friend`。

## 重载输入操作符 >>

    istream &operator>>(istream &is, Sales_data &item)
    {
        double price;
        is >> item.bookNo >> item.units_sold >> price;
        if (is)        // 检查输入是否成功
            item.revenue = item.units_sold * price;
        else
            item = Sales_data(); // 输入失败：给对象一个默认值
        return is;
    }

>**注意**
>输入操作符必须处理输入失败的可能性。输出操作符通常没有这个困扰。

我们选择在读取了所有数据之后，使用数据之前来检查流状态。

>**最佳实践**
>输入操作符应该决定做什么错误恢复。当错误发生时，使对象处于一个合法状态尤其重要，因为对象可能被部分修改。

通常输入操作符应该只设置`failbit`。`eofbit`和`badbit`最好是留给IO库去指示。

# 算术和关系操作符

通常我们将算术和关系操作符定义为非成员函数，以允许转换左右操作符中的一个。这些操作符不需要修改
操作数的状态，所以参数通常为`const`引用。
定义了算术操作符的类通常也要定义相应的复合赋值操作符。

    Sales_data
    operator+(const Sales_data &lhs, const Sales_data &rhs)
    {
        Sales_data sum = lhs;
        sum += rhs;
        return sum;
    }

>**提示**
>定义了算术操作符和相应的复合赋值操作符的类，应该使用复合赋值操作符来实现算术操作符。

## 相等操作符

    bool operator==(const Sales_data &lhs, const Sales_data &rhs)
    {
        return lhs.isbn() == rhs.isbn() &&
               lhs.units_sold == rhs.units_sold &&
               lhs.revenue == rhs.revenue;
    }
    bool operator!=(const Sales_data &lhs, const Sales_data &rhs)
    {
        return !(lhs == rhs);
    }

相等操作符的设计原理：
*   如果一个类有一个确定2个对象是否相等的操作，那么该类应该定义`operator==`，而不是命名函数。
    而且可以容易使用标准库容器和算法。
*   如果一个类定义了`operator==`，这个操作符通常应该确定给定的对象是否包含相同的数据。
*   通常地，相等操作符应该是可传递的， `if a == b and b == c, then a == c`。
*   如果一个类定义了`operator==`，那也应该定义`operator!=`，反之也一样。
*   相等或不等操作符应该将实际工作委托给另一个。

>**最佳实践**
>相等性具有逻辑意义的类通常应该定义`operator==`，定义了`operator==`的类使得用户容易使用标准库算法。

## 关系操作符

定义了相等操作符的类通常（不总是）也会定义关系操作符。特别地，关系容器和有些算法使用`<`操作符，
定义`operator<`会很有用。

通常关系操作符应该：
*   定义一个顺序关系和关联容器的key的需求一致。
*   定义一个关系和`==`一致，如果类2种操作符都定义了。特别地，如果2个对象`!=`，则其中一个应该`<`另一个。

>**最佳实践**
>如果只有一个逻辑定义`<`存在，类通常应该定义`<`操作符。然而，如果类也有`==`，只有当`<`和`==`产生
的结果一致时，定义`<`。

# 赋值操作符

除了`copy-`和`move-`赋值操作符之外，类还可以定义其它的赋值操作符允许其它类型作为右操作数。

    vector<string> v;
    v = {"a", "an", "the"};
    class StrVec {
    public:
        StrVec &operator=(std::initializer_list<std::string>);
    };
    StrVec &StrVec::operator=(initializer_list<string> il)
    {
        auto data = alloc_n_copy(il.begin(), il.end());
        free();
        elements = data.first;
        first_free = cap = data.second;
        return *this;
    }

不像`copy-`和`move-`赋值操作符，这个操作符不需要检查自我赋值。

>**注意**
>赋值操作符可以被重载。不管参数类型是什么，赋值操作符必须定义为成员函数。

复合赋值操作符不需要是类成员。然而，我们倾向于定义所有的赋值操作符，包括复合赋值操作符为类成员。

    Sales_data& Sales_data::operator+=(const Sales_data &rhs)
    {
        units_sold += rhs.units_sold;
        revenue += rhs.revenue;
        return *this;
    }

>**最佳实践**
>赋值操作符必须，而且通常复合赋值操作符应该，定义为成员。这些操作符应该返回左操作数的引用。

# 下标操作符

可以通过位置来获取元素的代表容器的类，通常定义下标操作符，`operater[]`。

>**注意**
>下标操作符必须为成员函数。

>**最佳实践**
>如果一个类有下标操作符，它通常应该定义2个版本：一个返回普通引用，一个返回const引用。

    class StrVec {
    public:
        std::string& operator[](std::size_t n)
            { return elements[n]; }
        const std::string& operator[](std::size_t n) const
            { return elements[n]; }
    private:
        std::string *elements; // 指向数组第一个元素
    };
 
    const StrVec cvec = svec;
    if (svec.size() && svec[0].empty())  {
        svec[0] = "zero"; // ok: 返回一个普通引用
        cvec[0] = "Zip";  // error: 返回一个const引用
    }

# 自增和自减操作符

自增和自减操作符最常被迭代器类实现。语言并没要要求这些操作符是成员，但是由于这些操作符改变它们操作
的对象的状态，我们选择将它们定义为成员。

>**最佳实践**
>定义自增或自减操作符的类应该定义`prefix`和`postfix`版本。这些操作符应该定义为成员。

    class StrBlobPtr {
    public:
        StrBlobPtr& operator++(); // 前自增操作符
        StrBlobPtr& operator--(); // 前自减操作符
    };

    StrBlobPtr& StrBlobPtr::operator++()
    {
        check(curr, "increment past end of StrBlobPtr");
        ++curr;
        return *this;
    }
    StrBlobPtr& StrBlobPtr::operator--()
    {
        --curr;
        check(-1, "decrement past begin of StrBlobPtr");
        return *this;
    }

>**最佳实践**
>为了和内置操作符一致，前置操作符应该返回自增过或自减过的对象的引用。

    class StrBlobPtr {
    public:
        StrBlobPtr operator++(int);    // 后置操作符
        StrBlobPtr operator--(int);
    };

    StrBlobPtr StrBlobPtr::operator++(int)
    {
        StrBlobPtr ret = *this;
        ++*this;
        return ret;
    }
    StrBlobPtr StrBlobPtr::operator--(int)
    {
        StrBlobPtr ret = *this;
        --*this;
        return ret;
    }

>**最佳实践**
>为了和内置操作符一致，后置操作符应该返回旧的（未自增或自减）值，而不是引用。

>**注意**
>`int`参数没有用到，所以没有给它一个名字。

    StrBlobPtr p(a1);
    p.operator++(0);  // 调用后自增操作符
    p.operator++();   // 调用前自增操作符

# 成员访问操作符

解引用（*）和箭头（->）操作符经常使用在迭代器类和智能指针类中。

    class StrBlobPtr {
    public:
        std::string& operator*() const
        { 
            auto p = check(curr, "dereference past end");
            return (*p)[curr];  // (*p)是这个对象指向的vector
        }
        std::string* operator->() const
        {
            return & this->operator*(); // 将实际工作委托给解引用操作符
        }
    };

>**注意**
>箭头操作符必须定义为成员，没有要求解引用为成员但通常也应该为成员。

值得注意的是我们定义这些操作符为`const`成员，获取一个元素不会改变这个对象的状态。

    StrBlob a1 = {"hi", "bye", "now"};
    StrBlobPtr p(a1);            // p指向a1里面的vector
    *p = "okay";                 // 给a1中第一个元素赋值
    cout << p->size() << endl;   // 第一个元素的size()
    cout << (*p).size() << endl; // 等价于p->size()

当我们写`point->mem`时，`point`必须是指向一个类对象的指针或是一个重载了`operator->`的类
的对象。取决于`point`的类型，`point->mem`等价于：

    (*point).mem;          // point是一个内置指针类型
    point.operator()->mem; // point是一个类的对象

>**注意**
>箭头操作符必须返回一个指向类类型的指针或一个定义了自己的箭头操作符的类类型的对象。

# 函数调用操作符

重载了函数调用操作符的类允许它的对象像函数一样使用。因为这种类可以保存状态，它们比普通函数更灵活。

    struct absInt {
        int operator()(int val) const {
            return val < 0 ? -val : val;
        }
    };

定义了函数调用操作符的类的对象被称为函数对象，我们可以像函数调用一样提供一个参数列表给函数对象：

    int i = -42;
    absInt absObj;
    int ui = absObj(i);

>**注意**
>函数调用操作符必须是成员函数。一个类可以定义多个版本的调用操作符，每一个参数类型或数量必须不一样。

和其他类一样，一个函数对象类除了`operator()`，还可以有其它成员。函数对象类通常包含其它成员用来
调整函数调用操作符的操作。

    class PrintString {
    public:
        PrintString(ostream &o = cout, char c = ' '):
            os(o), sep(c) { }
        void operator()(const string &s) const { os << s << sep;}
    private:
        ostream &os;   // stream on which to write
        char sep;      // character to print after each output
    };

    PrintString printer;   // uses the defaults; prints to cout
    printer(s);            // prints s followed by a space on cout
    PrintString errors(cerr, '\n');
    errors(s);             // prints s followed by a newline on cerr

函数对象最常用于泛型算法的参数：

    for_each(vs.begin(), vs.end(), PrintString(cerr, '\n'));

## Lambdas是函数对象

当我们写一个`lambda`时，编译器将表达式翻译成一个匿名类的匿名对象。由`lambda`生成的类
包含了一个重载的函数调用操作符。

    stable_sort(words.begin(), words.end(),
                [](const string &a, const string &b)
                  { return a.size() < b.size();});

行为就像一个匿名对象的类，看起来像：

    class ShorterString {
    public:
        bool operator()(const string &s1, const string &s2) const
        { return s1.size() < s2.size(); }
    };

    stable_sort(words.begin(), words.end(), ShorterString());

## 类表示Lambdas带捕获成员

    auto wc = find_if(words.begin(), words.end(),
                [sz](const string &a){ return a.size() >= sz;});

会生成像这样的类：

    class SizeComp {
    public:
        SizeComp(size_t n): sz(n) { } // parameter for each captured variable
        bool operator()(const string &s) const
            { return s.size() >= sz; }
    private:
        size_t sz; // a data member for each variable captured by value
    };

    auto wc = find_if(words.begin(), words.end(), SizeComp(sz));

由lambda表达式生成的类有一个`deleted`默认构造函数，`deleted`赋值操作符和一个默认析构函数。
是否有默认的或`deleted``copy/move`构造函数取决于捕获的数据成员。

## 标准库定义的函数对象

标准库定义了一个类集合，表示算术，关系和逻辑运算符。
`plus`类有一个函数调用操作符，应用`+`操作符。
`modulus`类有一个函数调用操作符，应用`%`操作符。
`equal_to`类有一个函数调用操作符，应用`==`操作符。

    plus<int> intAdd;       // function object that can add two int values
    negate<int> intNegate;  // function object that can negate an int value
    int sum = intAdd(10, 20);         // sum = 30
    sum = intNegate(intAdd(10, 20));  // sum = -30
    sum = intAdd(10, intNegate(10));  // sum = 0

## 算法中使用标准库函数对象

    // passes a temporary function object that applies the < operator to two strings
    sort(svec.begin(), svec.end(), greater<string>());

标准库函数对象保证对指针也有作用。

    vector<string *> nameTable;  // vector of pointers
    // error: the pointers in nameTable are unrelated, so < is undefined
    sort(nameTable.begin(), nameTable.end(),
         [](string *a, string *b) { return a < b; });
    // ok: library guarantees that less on pointer types is well defined
    sort(nameTable.begin(), nameTable.end(), less<string*>());

值得注意的是关联容器使用`less<key_type>`排序它们的元素。因此，我们能定义一个指针集合，或者使用
一个指针作为`key`的`map`容器而不用直接指定`less`。

## 可调用对象和函数

C++有几种可调用对象：函数和函数指针，lambdas，由`bind`创建的对象和重载了函数调用操作符的类。

不同类型可以有相同的调用签名：

    int add(int i, int j) { return i + j; } // 普通函数
    auto mod = [](int i, int j) { return i % j; }; // 匿名函数
    // 函数对象类
    struct div {
        int operator()(int denominator, int divisor) {
            return denominator / divisor;
        }
    };

    map<string, int(*)(int,int)> binops;
   
    binops.insert({"+", add}); // ok: add是一个指向合适类型的函数指针
    binops.insert({"%", mod}); // error: mod不是一个指向函数的指针

### 标准库function类型

	#include <functional>

    function<T> f;          空函数对象。
    function<T> f(nullptr); 显式构造空函数对象。
    function<T> f(obj);     保存一个obj的副本。
    f                       true当f拥有一个可调用对象，否则false。
    f(args)                 传递参数args，调用函数。

    定义为function<T>的成员的类型
    result_type             可调用对象的返回类型。
    argument_type           当参数为1个或2个时的类型。
    first_argument_type
    second_argument_type

`function`是一个模板。和其它模板一样，我们必须指定`function`的函数类型。

    function<int(int, int)> f1 = add;    // 函数指针
    function<int(int, int)> f2 = div();  // 函数对象
    function<int(int, int)> f3 = [](int i, int j) { return i * j;}; // 匿名函数
    cout << f1(4,2) << endl; // prints 6
    cout << f2(4,2) << endl; // prints 2
    cout << f3(4,2) << endl; // prints 8

    map<string, function<int(int, int)>> binops = {
        {"+", add},                  // function pointer
        {"-", std::minus<int>()},    // library function object
        {"/",  div()},               // user-defined function object
        {"*", [](int i, int j) { return i * j; }}, // unnamed lambda
        {"%", mod} };                // named lambda object

    binops["+"](10, 5); // calls add(10, 5)
    binops["-"](10, 5); // uses the call operator of the minus<int> object
    binops["/"](10, 5); // uses the call operator of the div object
    binops["*"](10, 5); // calls the lambda function object
    binops["%"](10, 5); // calls the lambda function object

### 重载函数和function

我们不能直接保存一个重载函数的函数名：

    int add(int i, int j) { return i + j; }
    Sales_data add(const Sales_data&, const Sales_data&);
    map<string, function<int(int, int)>> binops;
    binops.insert({"+", add}); // error: which add?

解决二义性的一个方法是存储函数指针：

    int (*fp)(int,int) = add;
    binops.insert({"+", fp});

或者使用lambda消除歧义：

    binops.insert({"+", [](int a, int b) {return add(a, b);}});

# 重载，转换和操作符

转换构造函数和转换操作符定义了类类型的转换。这种转换又叫做用户定义的转换。

## 转换操作符

转换操作符是一种特殊的成员函数，将一个类类型的值转换为其它类型的值。一个典型的转换函数具有形式：

    operator type() const;

转换操作符可以为任意（除了`void`）函数返回的类型定义转换。不允许转换为数组或函数类型。
允许转换为指针（数据指针和函数指针）和引用。

>**注意**
>转换函数必须是一个成员函数，不能指定返回类型，参数列表必须为空，通常为const。


### Defining a Class with a Conversion Operator

    class SmallInt {
    public:
        SmallInt(int i = 0): val(i)
        {
            if (i < 0 || i > 255)
                throw std::out_of_range("Bad SmallInt value");
        }
        operator int() const { return val; }
    private:
        std::size_t val;
    };

    SmallInt si;
    si = 4; // implicitly converts 4 to SmallInt then calls SmallInt::operator=
    si + 3; // implicitly converts si to int followed by integer addition

Although the compiler will apply only one user-defined conversion at a time (§
4.11.2, p. 162), an implicit user-defined conversion can be preceded or followed by a
standard (built-in) conversion (§ 4.11.1, p. 159).

    // the double argument is converted to int using the built-in conversion
    SmallInt si = 3.14; // calls the SmallInt(int) constructor
    // the SmallInt conversion operator converts si to int;
    si + 3.14; // that int is converted to double using the built-in conversion

Because conversion operators are implicitly applied, there is no way to pass
arguments to these functions. Hence, conversion operators may not be defined to
take parameters. Although a conversion function does not specify a return type, each
conversion function must return a value of its corresponding type:

    class SmallInt;
    operator int(SmallInt&);                 // error: nonmember
    class SmallInt {
    public:
        int operator int() const;            // error: return type
        operator int(int = 0) const;         // error: parameter list
        operator int*() const { return 42; } // error: 42 is not a
    pointer
    };

>Caution: Avoid Overuse of Conversion Functions

### Conversion Operators Can Yield Suprising Results

In practice, classes rarely provide conversion operators.
However, there is one important exception to this rule of
thumb: It is not uncommon for classes to define conversions to bool.

    int i = 42;
    cin << i; // this code would be legal if the conversion to bool were not explicit!

### explicit Conversion Operators

To prevent such problems, the new standard introduced explicit conversion

    class SmallInt {
    public:
        // the compiler won't automatically apply this conversion
        explicit operator int() const { return val; }
        // other members as before
    };

    SmallInt si = 3;  // ok: the SmallInt constructor is not explicit
    si + 3; // error: implicit is conversion required, but operator int is explicit
    static_cast<int>(si) + 3; // ok: explicitly request the conversion

If the conversion operator is explicit, we can still do the conversion. However, with
one exception, we must do so explicitly through a cast.

The exception is that the compiler will apply an explicit conversion to an
expression used as a condition. That is, an explicit conversion will be used
implicitly to convert an expression used as

• The condition of an if, while, or do statement 
• The condition expression in a for statement header
• An operand to the logical NOT(!), OR(||), or AND(&&) operators
• The condition expression in a conditional (?:) operator

### Conversion to bool

Whenever we use a stream object in a condition, we use the operator bool that
is defined for the IO types. For example,

    while (std::cin >> value)

>Best Practices
Conversion to bool is usually intended for use in conditions. As a result,
operator bool ordinarily should be defined as explicit.

### 14.9.2. Avoiding Ambiguous Conversions

There are two ways that multiple conversion paths can occur. The first happens
when two classes provide mutual conversions. For example, mutual conversions exist
when a class A defines a converting constructor that takes an object of class B and B
itself defines a conversion operator to type A.

The second way to generate multiple conversion paths is to define multiple
conversions from or to types that are themselves related by conversions. The most
obvious instance is the built-in arithmetic types. A given class ordinarily ought to
define at most one conversion to or from an arithmetic type.

>Warning
Ordinarily, it is a bad idea to define classes with mutual conversions or to
define conversions to or from two arithmetic types.

### Argument Matching and Mutual Conversions

    // usually a bad idea to have mutual conversions between two class types
    struct B;
    struct A {
        A() = default;
        A(const B&);        // converts a B to an A
        // other members
    };
    struct B {
        operator A() const; // also converts a B to an A
        // other members
    };
    A f(const A&);
    B b;
    A a = f(b); // error ambiguous: f(B::operator A())
                //          or f(A::A(const B&))

    A a1 = f(b.operator A()); // ok: use B's conversion operator
    A a2 = f(A(b));           // ok: use A's constructor

Note that we can’t resolve the ambiguity by using a cast—the cast itself would have
the same ambiguity.

### Ambiguities and Multiple Conversions to Built-in Types

    struct A {
        A(int = 0);   // usually a bad idea to have two
        A(double);    // conversions from arithmetic types
        operator int() const;    // usually a bad idea to have two
        operator double() const; // conversions to arithmetic types
        //   other members
    };
    void f2(long double);
    A a;
    f2(a); // error ambiguous: f(A::operator int())
           //          or f(A::operator double())
    long lg;
    A a2(lg); // error ambiguous: A::A(int) or A::A(double)

    short s = 42;
    // promoting short to int is better than converting short to double
    A a3(s);  // uses A::A(int)

>Note
When two user-defined conversions are used, the rank of the standard
conversion, if any, preceding or following the conversion function is used to
select the best match.

### Overloaded Functions and Converting Constructors

>Caution: Conversions and Operators
• Don’t define mutually converting classes—if class Foo has a constructor
that takes an object of class Bar, do not give Bar a conversion operator to
type Foo.
• Avoid conversions to the built-in arithmetic types. In particular, if you do
define a conversion to an arithmetic type, then
>>– Do not define overloaded versions of the operators that take arithmetic
types. If users need to use these operators, the conversion operation will
convert objects of your type, and then the built-in operators can be used.
– Do not define a conversion to more than one arithmetic type. Let the
standard conversions provide conversions to the other arithmetic types.

>The easiest rule of all: With the exception of an explicit conversion to
bool, avoid defining conversion functions and limit nonexplicit
constructors to those that are “obviously right.”

    struct C {
        C(int);
        // other members
    };
    struct D {
        D(int);
        // other members
    };
    void manip(const C&);
    void manip(const D&);
    manip(10); // error ambiguous: manip(C(10)) or manip(D(10))

    manip(C(10)); // ok: calls manip(const C&)

>Warning
Needing to use a constructor or a cast to convert an argument in a call to an
overloaded function frequently is a sign of bad design.

### Overloaded Functions and User-Defined Conversion

In a call to an overloaded function, if two (or more) user-defined conversions provide
a viable match, the conversions are considered equally good.

    struct E {
        E(double);
        // other members
    };
    void manip2(const C&);
    void manip2(const E&);
    // error ambiguous: two different user-defined conversions could be used
    manip2(10); // manip2(C(10) or manip2(E(double(10)))

>Note
In a call to an overloaded function, the rank of an additional standard
conversion (if any) matters only if the viable functions require the same user-
defined conversion. If different user-defined conversions are needed, then the
call is ambiguous.

## 14.9.3. Function Matching and Overloaded Operators

If
a has a class type, the expression a sym b might be

    a.operatorsym (b); // a has operatorsym as a member function
    operatorsym(a, b); // operatorsym is an ordinary function

>Note
The set of candidate functions for an operator used in an expression can
contain both nonmember and member functions.

    class SmallInt {
        friend
        SmallInt operator+(const SmallInt&, const SmallInt&);
    public:
        SmallInt(int = 0);                   // conversion from int
        operator int() const { return val; } // conversion to int
    private:
        std::size_t val;
    };

    SmallInt s1, s2;
    SmallInt s3 = s1 + s2;  // uses overloaded operator+
    int i = s3 + 0;         // error: ambiguous

>Warning
Providing both conversion functions to an arithmetic type and overloaded
operators for the same class type may lead to ambiguities between the
overloaded operators and the built-in operators.

# Chapter Summary

The assignment, subscript, function-call, and arrow operators
must be class members.
