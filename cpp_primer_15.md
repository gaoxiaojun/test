title: 面向对象编程
date: 2015-09-21 09:04:53
tags: C++
---

面向对象编程是基于三个基本概念：数据抽象，继承和动态绑定。

# OOP概述

## 继承

通过继承联系的类构成一个层级关系。典型地，有一个基类在层级的顶端，其它类直接或间接继承这个类。
这些继承类被称为派生类。基类定义层级中共同的成员。每一个派生类定义它自己特定的成员。

    class Quote {
    public:
        std::string isbn() const;
        virtual double net_price(std::size_t n) const;
    };

派生类必须指定继承自哪个（或哪些）基类。继承列表是逗号分隔的包含可选访问限定符的基类列表。

    class Bulk_quote : public Quote { // Bulk_quote继承自Quote
    public:
        double net_price(std::size_t) const override;
    };

派生类必须在类定义里面包含所有它想定义的虚函数的声明。派生类可以声明这些函数为virtual，但没有
要求这么做。新标准让派生类可以显式指定想要覆盖的虚函数，在参数列表后面指定`override`就可以。

## 动态绑定

    double print_total(ostream &os,
                       const Quote &item, size_t n)
    {
        // 取决于绑定到item参数的类型调用Quote::net_price或Bulk_quote::net_price
        double ret = item.net_price(n);
        os << "ISBN: " << item.isbn() // 调用Quote::isbn
           << " # sold: " << n << " total due: " << ret << endl;
         return ret;
    }

    // basic是Quote类型; bulk是Bulk_quote类型
    print_total(cout, basic, 20); // 调用Quote::net_price
    print_total(cout, bulk, 20);  // 调用Bulk_quote::net_price

因为运行哪个函数取决于参数的类型的决定直到运行时才能确定，因此动态绑定又叫运行时绑定。

>**注解**
>在C++中，动态绑定发生在通过基类的引用或指针调用虚函数时。

# 定义基类和派生类

## 定义一个基类

    class Quote {
    public:
        Quote() = default;
        Quote(const std::string &book, double sales_price):
                         bookNo(book), price(sales_price) { }
        std::string isbn() const { return bookNo; }
        // 派生类将覆盖和应用不同的打折策略
        virtual double net_price(std::size_t n) const
                   { return n * price; }
        virtual ~Quote() = default; // 析构函数动态绑定
    private:
        std::string bookNo;
    protected:
        double price = 0.0;
    };

>**注解**
>基类通常应该定义一个虚析构函数，即使没有什么工作要做。

### 成员函数和继承

基类将那些期望派生类覆盖的函数定义为虚函数。
任何非`static`的成员函数，除了构造函数，可以为虚函数。关键字`virtual`仅仅出现在类里面声明处，
在类外面函数定义处不需要出现。基类中声明为`virtual`的函数，在派生类中隐式为`virtual`。

### 访问控制和继承

派生类继承基类的成员，但是派生类的成员函数不能访问基类的私有成员。有时候基类允许派生类访问而又
禁止其他用户使用，我们指定这类成员为`protected`。

## 定义一个派生类

派生类必须指定继承自哪个类，子类在类继承列表中指定基类。类继承列表由逗号分隔的基类名组成，每个
基类名前面可能有`public`，`protected`，`private`其中一个。
派生类必须声明每一个想要覆盖的成员函数。

    class Bulk_quote : public Quote { // Bulk_quote继承自Quote
        Bulk_quote() = default;
        Bulk_quote(const std::string&, double, std::size_t, double);
        double net_price(std::size_t) const override;
    private:
        std::size_t min_qty = 0; // 打折需要的最小购买量
        double discount = 0.0;   // 折扣
    };

当继承为`public`时，基类的`public`成员成为派生类的接口。另外，我们能将`public`继承的派生
类对象绑定到基类的指针或引用。

大多数类直接继承一个基类，这种继承称为单继承，继承超过一个基类称为多继承。

### 派生类中的虚函数

派生类经常但不总是覆盖它们继承的虚函数。如果派生类不覆盖虚函数，则像其它成员一样，继承基类的版本。
参数列表后面，或`const`或引用限定符后面指定`override`覆盖虚函数。

### 派生类对象和派生类到基类的转换

派生类对象包含了多个部分：一个子对象包含派生类自己定义的非`static`成员，加上一个从基类继承而来
的相应的子对象。

因为派生类对象包含对应于基类的子部分，我们可以使用子类就好像它是基类一样。特别地，我们可以将子类
对象绑定到基类的引用或指针。

    Quote item;        // 基类对象
    Bulk_quote bulk;   // 子类对象
    Quote *p = &item;  // 指向基类的指针
    p = &bulk;         // p指向bulk的Quote部分
    Quote &r = bulk;   // r绑定到bulk的Quote部分

这种转换称为派生类到基类的转换。和其它转换一样，编译器隐式应用派生类到基类的转换。

>**注解**
>派生类对象包含基类的子对象是继承如何工作的关键。

### 派生类构造函数

尽管派生类对象包含从基类继承的成员，但是派生类不能直接初始化这些成员。派生类必须使用基类的构造函数
初始化基类部分。

>**注解**
>每一个类控制它的成员如何初始化。

    Bulk_quote(const std::string& book, double p,
               std::size_t qty, double disc) :
               Quote(book, p), min_qty(qty), discount(disc) { }
        // as before
    };

>**注解**
>基类成员先初始化，然后按声明的顺序初始化派生类的成员。

### 在派生类中使用基类的成员

派生类可以使用基类的`public`和`protected`成员：

    // 如果购买了指定数量的商品，使用折扣价格
    double Bulk_quote::net_price(size_t cnt) const
    {
        if (cnt >= min_qty)
            return cnt * (1 - discount) * price;
        else
            return cnt * price;
    }

值得知道的是派生类的作用域嵌套在基类作用域里面。因此派生类使用基类和自己的成员没有差别。

>**关键概念**:尊重基类的接口
>派生类构造函数可以对基类的`public`和`protected`成员赋值。尽管如此，通常不应该这么做。像基类
的其他用户一样，派生类应该尊重基类的接口，使用基类的构造函数初始化基类部分成员。

### 继承和static成员

如果基类定义了`static`成员，那么在整个继承层次中只有一个这样的成员。不管有多少个派生类，每一个
`static`成员只有一个实例。

    class Base {
    public:
        static void statmem();
    };
    class Derived : public Base {
        void f(const Derived&);
    };
    void Derived::f(const Derived &derived_obj)
    {
        Base::statmem();    // ok: Base定义了statmem
        Derived::statmem(); // ok: Derived继承类statmem
        // ok: 派生类对象可以访问基类的static成员
        derived_obj.statmem();
        statmem();
    }

### 派生类的声明

派生类的声明和其它类一样。声明包含类名字但不包括派生列表：

    class Bulk_quote : public Quote; // error: 派生列表不能出现在这
    class Bulk_quote; // ok: 声明派生类的正确方式

### 作为基类使用的类

在使用类作为基类前，一个类必须定义，而不仅仅是声明：

    class Quote;   // 声明但是未定义
    // error: Quote必须定义
    class Bulk_quote : public Quote { ... };

这个规则暗示我们一个类不可能继承自己。

### 阻止继承

在新标准下，我们可以在类名后面跟着`final`阻止类被继承：

    class NoDerived final { /*  */ }; // NoDerived不能作为基类
    class Base { /*  */ };
    class Last final : Base { /*  */ }; // Last不能作为基类
    class Bad : NoDerived { /*  */ };   // error: NoDerived is final
    class Bad2 : Last { /*  */ };       // error: Last is final

## 转换和继承

>**警告**
>理解基类和派生类之间的转换是理解C++面向对象编程如何工作的基础

通常我们只能绑定指针或引用到相同类型的对象上或者需要一个可接受的转换。
可以将基类的引用或指针绑定到一个派生类对象的事实暗示：当我们使用基类的指针或引用时，我们不知道指针
或引用绑定的对象的实际类型。这个对象可以是基类对象或派生类对象。

>**注解**
>像内置指针一样，智能指针支持派生类到基类的转换。

### 静态类型和动态类型

静态类型是变量声明的类型或表达式产生的类型，在编译期间就已经知道了。
动态类型是变量或表达式表示的内存中对象的类型，运行时才知道。

不是指针或引用的表达式的动态类型总是和静态类型一样。

>**注解**
>理解基类的指针或引用的静态类型与动态类型可能不一样很重要。

### 没有基类到派生类的隐式转换

    Quote base;
    Bulk_quote* bulkP = &base;  // error: can't convert base to derived
    Bulk_quote& bulkRef = base; // error: can't convert base to derived

令人惊讶地是，我们也不能将基类转换到派生类，即使基类指针或引用绑定的是派生类对象：

    Bulk_quote bulk;
    Quote *itemP = &bulk;        // ok: dynamic type is Bulk_quote
    Bulk_quote *bulkP = itemP;   // error: can't convert base to derived

### 也没有对象间的转换

派生类到基类的自动转换只发生在引用或指针类型的转换。派生类和基类之间没有这种转换。

    Bulk_quote bulk;   // object of derived type
    Quote item(bulk);  // uses the Quote::Quote(const Quote&) constructor
    item = bulk;       // calls Quote::operator=(const Quote&)

>**警告**
>当我们用一个派生类对象给一个基类对象初始化或赋值时，只有基类部分复制，移动或赋值了，派生类部分被
忽略了。

>**关键概念**：继承相关的类型转换
>*  继承类到基类的转换只发生在指针或引用类型
>*  没有基类到继承类的隐式转换
>*  像其它成员一样，继承类到基类的转换可能因为访问控制的原因不能访问。

# 虚函数

因为运行之前我们不知道哪个函数会调用，虚函数必须总是定义了的。

## 虚函数调用可能在运行时确定

动态绑定仅仅发生在通过指针或引用调用虚函数。

    base = derived;         // 从derived拷贝Quote部分到base 
    base.net_price(20);     // 调用Quote::net_price

当我们在一个表达式中用一个非引用且非指针的普通类型调用虚函数时，绑定发生在编译期间。

>**关键概念**：C++中的多态
>指针和引用的动态类型和静态类型可以不同是C++支持多态的基石。

>**注解**
>运行期间确定虚函数仅当通过指针或引用的调用。只有这些情况下，对象的动态类型才可能和静态类型不同。

## 派生类的虚函数

当派生类覆盖了一个虚函数时，声明时可以重复`virtual`关键字，但是没有要求这样做。一旦一个函数被
声明为`virtual`，这个函数在所有继承类中都是`virtual`。

派生类中的虚函数必须和基类中虚函数参数一样。除了一个例外，派生类虚函数的返回类型也必须和基类一样。
这个例外就是返回类型为和继承相关的指针或引用。

>**注解**
>基类中的虚函数在继承类中隐式为`virtual`。当派生类覆盖一个虚函数，参数必须和基类一样。

## final和override限定符

继承类定义一个和基类虚函数同名但是参数不同的函数是合法的。编译器认为这样的函数独立于基类的虚函数。

新标准下我们可以在继承类中指定虚函数为`override`。编译器会拒绝程序如果函数标为`override`却
没有覆盖一个已存在的虚函数。

    struct B {
        virtual void f1(int) const;
        virtual void f2();
        void f3();
    };
    struct D1 : B {
        void f1(int) const override; // ok: f1 matches f1 in the base
        void f2(int) override; // error: B has no f2(int) function
        void f3() override;    // error: f3 not virtual
        void f4() override;    // error: B doesn't have a function named f4
    };

我们也可以将一个函数指定为`final`。任何试图覆盖一个被定义为`final`的函数都被视为错误：

    struct D2 : B {
        // inherits f2() and f3() from B and overrides f1(int)
        void f1(int) const final; // 后续子类不能覆盖f1(int)
    };
    struct D3 : D2 {
        void f2(); // ok: overrides f2 inherited from the indirect base, B
        void f1(int) const; // error: D2 declared f2 as final
    };

`final`和`override`限定符出现在参数列表（包含const或引用修饰符）和尾返回之后。

## 虚函数和默认实参

如果一个函数调用使用默认实参，被使用的值是函数定义的静态类型的值。

>**最佳实践**
>包含默认实参的虚函数应该在基类和派生类中使用相同的默认实参。

## 避开虚机制

我们可以使用`::`操作符强制调用指定版本的虚函数。

    // 调用基类版本，不管baseP的动态类型
    double undiscounted = baseP->Quote::net_price(42);

>**注解**
>通常，只有成员函数或友元函数里面的代码应该使用作用域操作符来避开虚机制。

>**警告**
>如果继承类虚函数想要调用基类版本的虚函数，却忽略了作用域操作符，将会导致无限递归。

# 虚基类

## 纯虚函数

和普通的虚函数不一样的是，纯虚函数没有必要定义。虚函数写上`= 0`替代函数体成为纯虚函数。`= 0`只
出现在类里面函数声明处。

    // class to hold the discount rate and quantity
    // derived classes will implement pricing strategies using these data
    class Disc_quote : public Quote {
    public:
        Disc_quote() = default;
        Disc_quote(const std::string& book, double price,
                  std::size_t qty, double disc):
                     Quote(book, price),
                     quantity(qty), discount(disc) { }
        double net_price(std::size_t) const = 0;
    protected:
        std::size_t quantity = 0; //  purchase size for the discount to apply
        double discount = 0.0;    //  fractional discount to apply
    };

值得注意的是我们可以提供纯虚函数的定义，但是必须定义在类外面。

## 有纯虚函数的类为虚基类

包含纯虚函数的类（或继承而没有覆盖）是虚基类。虚基类为后续的子类定义了可覆盖的接口。不能直接创建
虚基类的对象。

    // Disc_quote定义了Bulk_quote将要覆盖的纯虚函数
    Disc_quote discounted; // error: can't define a Disc_quote object
    Bulk_quote bulk;       // ok: Bulk_quote has no pure virtual functions

## 派生类构造函数只初始化其直接基类

每一个类控制自己类对象的初始化。

>**关键概念：重构**
>重构在面向对象程序里面很常见。

# 访问控制和继承

## protected成员

`protected`限定符可以被认为是`private`和`public`的混合：

*   像`private`，`protected`成员对类用户是不可访问的。
*   像`public`，`protected`成员对派生类的成员和友元是可访问的。

另外，`protected`有一个重要性质：
*   派生类成员或友元只能通过派生类对象访问基类的`protected`成员。派生类没有权限访问基类
    `protected`成员。


    class Base {
    protected:
        int prot_mem;     // protected member
    };
    class Sneaky : public Base  {
        friend void clobber(Sneaky&);  // can access Sneaky::prot_mem
        friend void clobber(Base&);    // can't access Base::prot_mem
        int j;                          // j is private by default
    };
    void clobber(Sneaky &s) { s.j = s.prot_mem = 0; } // ok
    void clobber(Base &b) { b.prot_mem = 0; } // error

## public，private和protected继承

继承类成员的访问由基类中该成员访问控制符和派生列表访问控制符共同控制。

    class Base {
    public:
        void pub_mem();   // public member
    protected:
        int prot_mem;     // protected member
    private:
        char priv_mem;    // private member
    };
    struct Pub_Derv : public Base {
        // ok: derived classes can access protected members
        int f() { return prot_mem; }
        // error: private members are inaccessible to derived classes
        char g() { return priv_mem; }
    };
    struct Priv_Derv : private Base {
        // private derivation doesn't affect access in the derived class
        int f1() const { return prot_mem; }
    };

派生类的访问控制符对派生类访问其直接基类的成员没有影响。基类成员的访问只由基类的访问控制符控制。
派生类的访问控制符是用来控制派生类使用者及子类对其成员的使用。

    Pub_Derv d1;   //  members inherited from Base are public
    Priv_Derv d2;  //  members inherited from Base are private
    d1.pub_mem();  //  ok: pub_mem is public in the derived class
    d2.pub_mem();  //  error: pub_mem is private in the derived class

    struct Derived_from_Public : public Pub_Derv {
        // ok: Base::prot_mem remains protected in Pub_Derv
        int use_base() { return prot_mem; }
    };
    struct Derived_from_Private : public Priv_Derv {
        // error: Base::prot_mem is private in Priv_Derv
        int use_base() { return prot_mem; }
    };

## 派生类到基类的转换的可访问性

假设D继承B：
*   用户代码可以使用派生类到基类的转换当D公开继承B，私有继承或保护继承都不行。
*   D的成员函数和友元可以使用派生类到基类的转换而不管是如何继承的。派生类到直接基类的转换对于
    派生类的成员和友元总是可以访问的。
*   从D派生的成员函数和友元能使用派生类到基类的转换当D公开或保护继承B，私有继承不行。

>**提示**
>在代码的任何地方，如果基类的`public`成员能够访问，则派生类到基类的转换可以访问，反过来不行。

## 友元和继承

就像友元不能传递，友元也不能继承。

    class Base {
        // added friend declaration; other members as before
        friend class Pal; // Pal has no access to classes derived from Base
    };
    class Pal {
    public:
        int f(Base b) { return b.prot_mem; } // ok: Pal is a friend of
    Base
        int f2(Sneaky s) { return s.j; } // error: Pal not friend of
    Sneaky
        // access to a base class is controlled by the base class, even inside a derived
    object
        int f3(Sneaky s) { return s.prot_mem; } // ok: Pal is a friend
    };

当类A将类B作为友元时，只有类B被授予了友元关系。类B的基类或子类都没有A的特殊访问权限。

    // D2 has no access to protected or private members in Base
    class D2 : public Pal {
    public:
       int mem(Base b)
           { return b.prot_mem; } // error: friendship doesn't inherit
    };

>**注解**
>友元关系不能继承，每一个类控制自己成员的访问。

## 免除个别成员访问规则

有时候需要改变派生类中成员的访问级别，可以使用`using`声明。

    class Base {
    public:
        std::size_t size() const { return n; }
    protected:
        std::size_t n;
    };
    class Derived : private Base {    //  note: private inheritance
    public:
        // maintain access levels for members related to the size of the object
        using Base::size;
    protected:
        using Base::n;
    };

因为Derived使用`private`继承，则继承的成员默认为`private`成员。`using`声明调整了这些成员的
可访问级别。

`using`声明可以命名任何可访问的直接或间接基类的成员。访问由`using`声明的名字取决于`using`声明
之前的访问限定符。

>**注解**
>派生类只为允许访问的名字提供`using`声明。

## 默认继承保护级别

默认地，`class`关键字定义的派生类使用`private`继承，`struct`定义的派生类使用`public`继承。

    class Base { /* ...   */ };
    struct D1 : Base { /* ...   */ };   // public inheritance by default
    class D2 : Base { /* ...   */ };    // private inheritance by default

一个常见的误解是使用`struct`和`class`定义的类有深层次的不同。仅有的不同就是成员的默认访问级别
和派生类默认访问不同。没有其它差别。

>**最佳实践**
>类私有继承必须显式指定`private`而不是依赖默认行为。显式指定可以清晰的说明`private`继承是有意
的而不是疏忽。

# 继承下的类作用域

每一个类定义自己的作用域，作用域里面定义其成员。在继承下，派生类的作用域嵌套在基类作用域里面。如果
一个名字在派生类作用域找不到，则搜索外围基类作用域。正是这种分层嵌套的类作用域允许派生类成员使用
基类成员就好像这些成员是派生类的一部分一样。

    Bulk_quote bulk;
    cout << bulk.isbn();

isbn的名字查找如下：

*   因为isbn被Bulk_quote对象调用，查找从类Bulk_quote开始，没有找到。
*   因为Bulk_quote继承自Disc_quote，接下来查找Disc_quote，还是没找到。
*   因为Disc_quote继承自Quote，接下来查找Quote，找到了。

## 名字查找发生在编译时

对象，引用或指针的静态类型决定了对象的哪个成员可见，甚至当静态类型和动态类型不一样时。

    class Disc_quote : public Quote {
    public:
        std::pair<size_t, double> discount_policy() const
            { return {quantity, discount}; }
        // other members as before
    };

    Bulk_quote bulk;
    Bulk_quote *bulkP = &bulk; //  static and dynamic types are the same
    Quote *itemP = &bulk;      //  static and dynamic types differ
    bulkP->discount_policy();  //  ok: bulkP has type Bulk_quote*
    itemP->discount_policy();  //  error: itemP has type Quote*

>**关键概念**：名字查找和继承
>给定调用p->mem()或obj.mem()
>*  首先确定p的静态类型。因为调用了一个成员，p一定指向一个类类型。
>*  查找p静态类型相应的成员mem，如果没找到，则继续在继承层级中找。如果基类中都没找到，编译错误。
>*  一旦mem找到了，执行正常的类型检查以确认调用是否合法。
>*  假设调用合法，编译器再根据调用是否为虚函数来产生代码。

## 名字冲突和继承

和其它作用域一样，继承类可以重用其直接或间接基类中的一个名字。和往常一样，定义在内部作用域的名字，
隐藏了外部作用域名字的使用。

    struct Base {
        Base(): mem(0) { }
    protected:
        int mem;
    };
    struct Derived : Base {
        Derived(int i): mem(i) { } // initializes Derived::mem to i
                                   // Base::mem is default initialized
        int get_mem() { return mem; }  // returns Derived::mem
    protected:
        int mem;   // hides mem in the base
    };

>**注解**
>与基类成员同名的派生类成员隐藏了基类成员的直接使用。

## 使用作用域操作符使用隐藏成员

    struct Derived : Base {
        int get_base_mem() { return Base::mem; }
        // ...
    };

>**最佳实践**
>除了覆盖继承的虚函数，基类通常不应该重用已经在基类中定义过的名字。

## 名字查找发生在类型检查之前

定义在派生类的同名函数不重载基类的函数。基类成员被隐藏了，即使参数列表不一样。

    struct Base {
        int memfcn();
    };
    struct Derived : Base {
        int memfcn(int);   // hides memfcn in the base
    };
    Derived d; Base b;
    b.memfcn();       //  calls Base::memfcn
    d.memfcn(10);     //  calls Derived::memfcn
    d.memfcn();       //  error: memfcn with no arguments is hidden
    d.Base::memfcn(); //  ok: calls Base::memfcn

## 虚函数和作用域

我们现在可以理解为什么派生类和基类中的虚函数必须有一样的参数列表。如果基类和派生类中虚函数参数不
一样，那么没有办法通过基类的引用或指针调用派生类虚函数。

    class Base {
    public:
        virtual int fcn();
    };
    class D1 : public Base {
    public:
        // hides fcn in the base; this fcn is not virtual
        // D1 inherits the definition of Base::fcn()
        int fcn(int);      // parameter list differs from fcn in Base
        virtual void f2(); // new virtual function that does not exist in Base
    };
    class D2 : public D1 {
    public:
        int fcn(int); // nonvirtual function hides D1::fcn(int)
        int fcn();    // overrides virtual fcn from Base
        void f2();    // overrides virtual f2 from D1
    };

    Base bobj;  D1 d1obj; D2 d2obj;
    Base *bp1 = &bobj, *bp2 = &d1obj, *bp3 = &d2obj;
    bp1->fcn(); // virtual call, will call Base::fcn at run time
    bp2->fcn(); // virtual call, will call Base::fcn at run time
    bp3->fcn(); // virtual call, will call D2::fcn at run time
    D1 *d1p = &d1obj; D2 *d2p = &d2obj;
    bp2->f2(); // error: Base has no member named f2
    d1p->f2(); // virtual call, will call D1::f2() at run time
    d2p->f2(); // virtual call, will call D2::f2() at run time

    Base *p1 = &d2obj; D1 *p2 = &d2obj; D2 *p3 =  &d2obj;
    p1->fcn(42);  // error: Base has no version of fcn that takes an int
    p2->fcn(42);  // statically bound, calls D1::fcn(int)
    p3->fcn(42);  // statically bound, calls D2::fcn(int)

## 覆盖重载函数

派生类可以覆盖0个或多个继承的重载函数。如果派生类想要使所有重载版本可用，则必须覆盖所有的函数或
所有函数不覆盖。

有时候只想覆盖一些重载函数，派生类可以提供一个`using`声明给重载的成员。一个`using`声明指示指定
一个名字，不带参数列表。因此，`using`声明将基类的重载函数都加到派生类的作用域。派生类只需定义
那些想要覆盖的函数。

# 构造函数和复制控制

## 虚析构函数

继承给基类的复制控制主要的直接影响是基类通常应该定义一个虚析构函数。

    class Quote {
    public:
        // virtual destructor needed if a base pointer pointing to a derived object is deleted
        virtual ~Quote() = default; // dynamic binding for the destructor
    };

    Quote *itemP = new Quote;   //  same static and dynamic type
    delete itemP;               //  destructor for Quote called
    itemP = new Bulk_quote;     //  static and dynamic types differ
    delete itemP;               //  destructor for Bulk_quote called

>**警告**
>如果基类析构函数不是`virtual`的， `delete`基类指针指向派生类对象是`undefined`。

### 虚析构函数关闭了合成的Move

如果一个类定义了虚析构函数，即使是`= default`使用合成版本，则编译器不会合成`move`操作。

## 合成复制控制和继承

基类或派生类中合成的复制控制成员和任何其它合成的复制控制成员一样：逐个初始化，复制或销毁类成员。
另外，这些成员使用基类的相应操作初始化，复制或销毁其对象的直接基类部分。


