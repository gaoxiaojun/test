title: 第15章 面向对象编程
date: 2015-09-21 09:04:53
tags: [C++]
---

# 15.1 OOP概述

面向对象编程的关键思想是数据抽象，继承和动态绑定。使用数据抽象，我们可以定义接口和实现分离的类。通过继承，我们可以定义相似类之间的关系模型。通过动态绑定，我们可以使用这些类对象而忽略它们之间的差别。

## 继承

通过继承联系的类构成一个层级关系。典型地，有一个基类在层级的顶端，其它类直接或间接继承这个类。这些继承类被称为派生类。基类定义层级中共同的成员。每一个派生类定义它自己特定的成员。

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

派生类必须在类定义里面包含所有它想定义的虚函数的声明。派生类可以声明这些函数为**virtual**，但没有要求这么做。新标准让派生类可以显式指定想要覆盖的虚函数，在参数列表后面指定**override**就可以。

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

因为直到运行时才能根据对象的实际类型确定调用哪个函数，因此动态绑定又叫运行时绑定。

>**注解**
>在C++中，动态绑定发生在通过基类的引用或指针调用虚函数时。

# 15.2 定义基类和派生类

## 15.2.1 定义一个基类

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

基类将那些期望派生类覆盖的函数定义为虚函数。任何非**static**的成员函数，除了构造函数，都可以为虚函数。关键字**virtual**仅仅出现在类里面声明处，在类外面函数定义处不需要出现。基类中声明为**virtual**的函数，在派生类中隐式为**virtual**。

没有被声明为**virtual**的成员函数在编译的时候确定。

### 访问控制和继承

派生类继承基类的成员，但是派生类的成员函数不能访问基类的私有成员。有时候基类允许派生类访问而又禁止其他用户使用，我们指定这类成员为**protected**。

## 15.2.2 定义一个派生类

派生类必须指定继承自哪个类，子类在类继承列表中指定基类。类继承列表由逗号分隔的基类名组成，每个基类名前面可能有**public**，**protected**，**private**其中一个。派生类必须声明每一个想要覆盖的成员函数。

    class Bulk_quote : public Quote { // Bulk_quote继承自Quote
        Bulk_quote() = default;
        Bulk_quote(const std::string&, double, std::size_t, double);
        double net_price(std::size_t) const override;
    private:
        std::size_t min_qty = 0; // 打折需要的最小购买量
        double discount = 0.0;   // 折扣
    };

当继承为**public**时，基类的**public**成员成为派生类的接口。另外，我们能将**public**继承的派生类对象绑定到基类的指针或引用。

大多数类直接继承一个基类，这种继承称为单继承，继承超过一个基类称为多继承。

### 派生类中的虚函数

派生类经常但不总是覆盖它们继承的虚函数。如果派生类不覆盖虚函数，则像其它成员一样，继承基类的版本。在参数列表后面，**const**或引用限定符后面使用**override**表示覆盖虚函数。

### 派生类对象和派生类到基类的转换

派生类对象包含了多个部分：一个子对象包含派生类自己定义的非**static**成员，加上一个从基类继承而来的相应的子对象。

因为派生类对象包含对应于基类的子部分，我们可以使用子类就好像它是基类一样。特别地，我们可以将子类对象的基类部分绑定到基类的引用或指针。

    Quote item;        // 基类对象
    Bulk_quote bulk;   // 子类对象
    Quote *p = &item;  // 指向基类的指针
    p = &bulk;         // p指向bulk的Quote部分
    Quote &r = bulk;   // r绑定到bulk的Quote部分

这种转换称为派生类到基类的转换。和其它转换一样，编译器隐式应用派生类到基类的转换。

>**注解**
>派生类对象包含基类的子对象是继承如何工作的关键。

### 派生类构造函数

尽管派生类对象包含从基类继承的成员，但是派生类不能直接初始化这些成员。派生类必须使用基类的构造函数初始化基类部分。

>**注解**
>每一个类控制它的成员如何初始化。基类成员先初始化，然后按声明的顺序初始化派生类的成员。

    Bulk_quote(const std::string& book, double p,
               std::size_t qty, double disc) :
               Quote(book, p), min_qty(qty), discount(disc) { }
        // as before
    };

### 在派生类中使用基类的成员

派生类可以使用基类的**public**和**protected**成员：

    // 如果购买了指定数量的商品，使用折扣价格
    double Bulk_quote::net_price(size_t cnt) const
    {
        if (cnt >= min_qty)
            return cnt * (1 - discount) * price;
        else
            return cnt * price;
    }

值得知道的是派生类的作用域嵌套在基类作用域里面。因此派生类使用基类和自己的成员没有差别。

>**关键概念**: 尊重基类的接口
>派生类构造函数可以对基类的**public**和**protected**成员赋值。尽管如此，通常不应该这么做。像基类的其他用户一样，派生类应该尊重基类的接口，使用基类的构造函数初始化基类部分成员。

### 继承和static成员

如果基类定义了**static**成员，那么在整个继承层次中只有一个这样的成员。不管有多少个派生类，每一个**static**成员只有一个实例。**static**成员遵守正常的访问控制。如果成员是**private**，则不能访问。

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

这个限制的原因很容易理解：每一个派生类包含从基类继承的成员。要使用这些成员，派生类必须知道它们是什么。这个规则暗示我们一个类不可能继承自己。一个基类本身也可以是派生类。

每一个类继承了其直接基类的所有成员。直接基类的成员包含了它的直接基类和继承链继承而来的成员。因此，最远的派生类包含了一个其直接基类的子对象和每一个间接基类的子对象。

### 阻止继承

在新标准下，我们可以在类名后面跟着**final**阻止类被继承：

    class NoDerived final { /*  */ }; // NoDerived不能作为基类
    class Base { /*  */ };
    class Last final : Base { /*  */ }; // Last不能作为基类
    class Bad : NoDerived { /*  */ };   // error: NoDerived is final
    class Bad2 : Last { /*  */ };       // error: Last is final

## 15.2.3 转换和继承

>**警告**
>理解基类和派生类之间的转换是理解C++面向对象编程如何工作的基础

通常我们只能绑定指针或引用到相同类型的对象上或者需要一个可接受的转换。可以将基类的引用或指针绑定到一个派生类对象的事实暗示：当我们使用基类的指针或引用时，我们不知道指针或引用绑定的对象的实际类型。这个对象可以是基类对象或派生类对象。

>**注解**
>像内置指针一样，智能指针支持派生类到基类的转换。

### 静态类型和动态类型

静态类型是变量声明的类型或表达式产生的类型，在编译期间就已经知道了。动态类型是变量或表达式表示的内存中对象的类型，直到运行时才知道。不是指针或引用的表达式的动态类型总是和静态类型一样。

>**注解**
>理解基类的指针或引用的静态类型与动态类型可能不一样很重要。

### 没有基类到派生类的隐式转换

    Quote base;
    Bulk_quote* bulkP = &base;  // error: can't convert base to derived
    Bulk_quote& bulkRef = base; // error: can't convert base to derived

令人惊讶地是，即使基类指针或引用绑定的是派生类对象，我们也不能将基类转换到派生类：

    Bulk_quote bulk;
    Quote *itemP = &bulk;        // ok: dynamic type is Bulk_quote
    Bulk_quote *bulkP = itemP;   // error: can't convert base to derived

编译器在编译期间无法知道这种转换在运行时是否安全。如果基类有一个或多个虚函数，我们可以使用**dynamic_cast**要求在运行时检查转换。或者如果我们知道这些从基类到派生类的转换是安全的，我们可以使用**static_cast**替换编译器的检查。

### 也没有对象间的转换

派生类到基类的自动转换只发生在引用或指针类型的转换。派生类和基类之间没有这种转换。

    Bulk_quote bulk;   // object of derived type
    Quote item(bulk);  // uses the Quote::Quote(const Quote&) constructor
    item = bulk;       // calls Quote::operator=(const Quote&)

>**警告**
>当我们用一个派生类对象给一个基类对象初始化或赋值时，只有基类部分复制，移动或赋值了，派生类部分被忽略了。

>**关键概念**： 继承相关的类型转换
>*  继承类到基类的转换只发生在指针或引用类型
>*  没有基类到继承类的隐式转换
>*  像其它成员一样，继承类到基类的转换可能因为访问控制的原因不能访问。

# 15.3 虚函数

因为运行之前我们不知道哪个函数会调用，虚函数必须总是定义了的。

## 虚函数调用可能在运行时确定

动态绑定仅仅发生在通过指针或引用调用虚函数。

    base = derived;         // 从derived拷贝Quote部分到base 
    base.net_price(20);     // 调用Quote::net_price

当我们在一个表达式中用一个非引用且非指针的普通类型调用虚函数时，绑定发生在编译期间。

>**关键概念**： C++中的多态
>指针和引用的动态类型和静态类型可以不同是C++支持多态的基石。

>**注解**
>运行期间确定虚函数仅当通过指针或引用的调用。只有这些情况下，对象的动态类型才可能和静态类型不同。

## 派生类的虚函数

当派生类覆盖了一个虚函数时，声明时可以重复**virtual**关键字，但是没有要求这样做。一旦一个函数被声明为**virtual**，这个函数在所有继承类中都是**virtual**。

派生类中的虚函数必须和基类中虚函数参数一样。除了返回类型为和继承相关的指针或引用之外，派生类虚函数的返回类型也必须和基类一样。就是如果D继承自B，那么基类的虚函数可以返回B*，而派生类的虚函数可以返回D*。然而这种返回类型需要派生类到基类的转换可访问。

>**注解**
>基类中的虚函数在继承类中隐式为**virtual**。当派生类覆盖一个虚函数，参数必须和基类一样。

## final和override限定符

继承类定义一个和基类虚函数同名但是参数不同的函数是合法的。编译器认为这样的函数独立于基类的虚函数。

新标准下我们可以在继承类中指定虚函数为**override**。编译器会拒绝程序如果函数标注为**override**却没有覆盖一个已存在的虚函数。

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

我们也可以将一个函数指定为**final**。任何试图覆盖一个被定义为**final**的函数都被视为错误：

    struct D2 : B {
        // inherits f2() and f3() from B and overrides f1(int)
        void f1(int) const final; // 后续子类不能覆盖f1(int)
    };
    struct D3 : D2 {
        void f2(); // ok: overrides f2 inherited from the indirect base, B
        void f1(int) const; // error: D2 declared f2 as final
    };

**final**和**override**限定符出现在参数列表（包含const或引用修饰符）和尾返回之后。

## 虚函数和默认实参

如果一个函数调用使用默认实参，被使用的值是函数定义的静态类型的值。

>**最佳实践**
>包含默认实参的虚函数应该在基类和派生类中使用相同的默认实参。

## 避开虚机制

我们可以使用**::**操作符强制调用指定版本的虚函数。

    // 调用基类版本，不管baseP的动态类型
    double undiscounted = baseP->Quote::net_price(42);

>**注解**
>通常，只有成员函数或友元函数里面的代码应该使用作用域操作符来避开虚机制。

为什么想要避开虚机制？最常见的理由是一个派生类的虚函数调用基类的版本。这种情况下，基类版本做了继承层次所有类型的共同工作。派生类定义的版本只需做自己的额外工作。

>**警告**
>如果继承类虚函数想要调用基类版本的虚函数，却忽略了作用域操作符，将会导致无限递归。

# 15.4 虚基类

## 纯虚函数

和普通的虚函数不一样的是，纯虚函数没有必要定义。虚函数写上**= 0**替代函数体成为纯虚函数。**= 0**只出现在类里面函数声明处。

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

虽然我们不能直接定义Disc_quote对象，但是Disc_quote的派生类会使用Disc_quote的构造函数构造Disc_quote部分。

值得注意的是我们可以提供纯虚函数的定义，但是必须定义在类外面。

## 有纯虚函数的类为虚基类

包含纯虚函数的类（或继承而没有覆盖）是虚基类。虚基类为后续的子类定义了可覆盖的接口。不能直接创建虚基类的对象。

    // Disc_quote定义了Bulk_quote将要覆盖的纯虚函数
    Disc_quote discounted; // error: can't define a Disc_quote object
    Bulk_quote bulk;       // ok: Bulk_quote has no pure virtual functions

继承自虚基类的类必须覆盖纯虚函数，否则这个类也是虚基类。

## 派生类构造函数只初始化其直接基类

    // the discount kicks in when a specified number of copies of the same book are sold
    // the discount is expressed as a fraction to use to reduce the normal price
    class Bulk_quote : public Disc_quote {
    public:
        Bulk_quote() = default;
        Bulk_quote(const std::string& book, double price,
                  std::size_t qty, double disc):
              Disc_quote(book, price, qty, disc) { }
        // overrides the base version to implement the bulk purchase discount policy
        double net_price(std::size_t) const override;
    };

每一个类控制自己类对象的初始化。

>**关键概念：重构**
>添加Disc_quote到Quote继承层次是重构的一个例子。重构包含重新设计类层次，从一个类移动操作或数据到另一个类。重构在面向对象程序里面很常见。
>值得注意的是尽管我们改变了继承层次，使用Bulk_quote或Quote的代码不需要改变。

# 15.5 访问控制和继承

就像每一个类控制自己成员的初始化，每一个类也控制其成员对派生类是否可访问。

## protected成员

**protected**限定符可以被认为是**private**和**public**的混合：

*   像**private**，**protected**成员对类用户是不可访问的。
*   像**public**，**protected**成员对派生类的成员和友元是可访问的。

另外，**protected**有一个重要性质：
*   派生类成员或友元只能通过派生类对象访问基类的**protected**成员。派生类没有权限访问基类**protected**成员。


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

派生类的访问控制符对派生类访问其直接基类的成员没有影响。基类成员的访问只由基类的访问控制符控制。派生类的访问控制符是用来控制派生类使用者及子类对其成员的使用。

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

## 派生类到基类转换的可访问性

假设D继承B：
*   用户代码可以使用派生类到基类的转换当D公开继承B，私有继承或保护继承都不行。
*   D的成员函数和友元可以使用派生类到基类的转换而不管是如何继承的。派生类到直接基类的转换对于派生类的成员和友元总是可以访问的。
*   从D派生的成员函数和友元能使用派生类到基类的转换当D公开或保护继承B，私有继承不行。

>**提示**
>在代码的任何地方，如果基类的**public**成员能够访问，则派生类到基类的转换可以访问，反过来不行。

## 友元和继承

就像友元不能传递，友元也不能继承。基类的友元对派生类没有特殊访问权限，派生类的友元对基类也没有特殊访问权限。

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

f3的合法看似惊人，但是它直接遵循每一个类控制其成员的访问这条规则。类Pal是基类Base的友元，因此Pal可以访问基类对象的成员。这种访问包含嵌套在派生类对象中基类部分。

当一个类将另一个类作为友元，只有这个类被赋予了友元关系，其基类或者子类都没有友元关系。

    // D2 has no access to protected or private members in Base
    class D2 : public Pal {
    public:
       int mem(Base b)
           { return b.prot_mem; } // error: friendship doesn't inherit
    };

>**注解**
>友元关系不能继承，每一个类控制自己成员的访问。

## 免除个别成员访问规则

有时候需要改变派生类中成员的访问级别，可以使用**using**声明。

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

因为Derived使用**private**继承，则继承的成员默认为**private**成员。**using**声明调整了这些成员的可访问级别。

**using**声明可以命名任何可访问的直接或间接基类的成员。访问由**using**声明的名字取决于**using**声明之前的访问限定符。

>**注解**
>派生类只为允许访问的名字提供**using**声明。

## 默认继承保护级别

默认地，**class**关键字定义的派生类使用**private**继承，**struct**定义的派生类使用**public**继承。

    class Base { /* ...   */ };
    struct D1 : Base { /* ...   */ };   // public inheritance by default
    class D2 : Base { /* ...   */ };    // private inheritance by default

一个常见的误解是使用**struct**和**class**定义的类有深层次的不同。唯一的不同就是成员的默认访问级别和默认继承访问限定符，没有其它差别。

>**最佳实践**
>类私有继承必须显式指定**private**而不是依赖默认行为。显式指定可以清晰的说明**private**继承是有意的而不是疏忽。

# 15.6 继承下的类作用域

每一个类定义自己的作用域，作用域里面定义其成员。在继承下，派生类的作用域嵌套在基类作用域里面。如果一个名字在派生类作用域找不到，则搜索外围基类作用域。正是这种分层嵌套的类作用域允许派生类成员使用基类成员就好像这些成员是派生类的一部分一样。

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

和其它作用域一样，继承类可以重用其直接或间接基类中的一个名字。和往常一样，定义在内部作用域(派生类）的名字，隐藏了外部作用域（基类）名字的使用。

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

## 使用作用域操作符使用被隐藏成员

我们可以使用作用域操作符来使用被隐藏的基类成员。

    struct Derived : Base {
        int get_base_mem() { return Base::mem; }
        // ...
    };

作用域操作符覆盖正常的名字查找，并指引编译器从指定的作用域开始查找。

>**最佳实践**
>除了覆盖继承的虚函数，基类通常不应该重用已经在基类中定义过的名字。

## 名字查找发生在类型检查之前

定义在派生类的同名函数不重载基类的函数。基类成员被隐藏了，即使参数列表不一样。一旦名字被找到，编译器不再继续查找。

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

我们现在可以理解为什么派生类和基类中的虚函数必须有一样的参数列表。如果基类和派生类中虚函数参数不一样，那么没有办法通过基类的引用或指针调用派生类虚函数。

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

派生类可以覆盖0个或多个继承的重载函数。如果派生类想要使所有重载版本可用，则必须覆盖所有的函数或所有函数不覆盖。

有时候只想覆盖一些重载函数，派生类可以提供一个**using**声明给重载的成员。一个**using**声明指定一个名字，不带参数列表。因此，**using**声明将基类的重载函数都加到派生类的作用域。派生类只需定义那些想要覆盖的函数。

# 15.7 构造函数和复制控制

像其它类一样，继承层次中的类控制其对象的创建，复制，移动，赋值和销毁。如果一个类没有定义自己的复制控制操作，编译器将合成这些操作。同样这些合成的操作可能是**deleted**的函数。

## 15.7.1 虚析构函数

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
>如果基类析构函数不是**virtual**的， **delete**基类指针指向派生类对象是**undefined**。

对于如果一个类需要析构函数，它也需要复制和赋值操作符这个规则，基类的析构函数是个重要例外。

### 虚析构函数关闭了合成的Move

如果一个类定义了虚析构函数，即使是**= default**使用合成版本，则编译器不会合成**move**操作。

## 15.7.2 合成复制控制和继承

基类或派生类中合成的复制控制成员和任何其它合成的复制控制成员一样：逐个初始化，复制或销毁类成员。另外，这些成员使用基类的相应操作初始化，复制或销毁其对象的直接基类部分。

值得注意的是基类的成员是否是合成的或用户定义的并没有关系。重要的是相应的成员是可访问的且不是一个被删除的函数。

### 基类和派生类中被删除的复制控制成员

基类或者派生类中合成的构造函数或任何复制控制成员，跟其它类的原因一样，都可能被定义为被删除的。另外基类定义的方式会导致派生类的成员被定义为被删除的：

* 如果基类的默认构造函数，复制构造函数，赋值操作符或析构函数是被删除的或不能访问，则派生类中相应的成员被定义为被删除的，因为编译器无法使用基类的成员来构造，赋值或销毁派生类对象的基类部分。
* 如果基类有一个不能访问的或被删除的析构函数，则派生类中合成的默认构造函数和复制构造函数被定义为被删除的，因为没有办法销毁派生类对象的基类部分。
* 编译器不会合成一个被删除的移动操作。如果基类的移动操作是被删除的或不能访问，则派生类中的移动操作是被删除的，因为基类部分不能被移动。如果基类的析构函数是被删除的或不能访问，则派生类的移动构造函数是被删除的。


    class B {
    public:
        B();
        B(const B&) = delete;
        // other members, not including a move constructor
    };
    class D : public B {
        // no constructors
    };
    D d;     // ok: D's synthesized default constructor uses B's default constructor
    D d2(d); // error: D's synthesized copy constructor is deleted
    D d3(std::move(d)); // error: implicitly uses D's deleted copy constructor

### 移动操作和继承

我们已经看到，大多数基类定义了一个虚析构函数。因此基类通常没有合成的移动操作，同样派生类也没有合成的移动操作。由于基类缺少移动操作而限制了派生类合成移动操作，如果合理基类一般应该定义移动操作。如果显式定义了移动操作，基类一般也要显式定义复制操作。

    class Quote {
    public:
        Quote() = default;             // memberwise default initialize
        Quote(const Quote&) = default; // memberwise copy
        Quote(Quote&&) = default;      // memberwise copy
        Quote& operator=(const Quote&) = default; // copy assign
        Quote& operator=(Quote&&) = default;      // move assign
        virtual ~Quote() = default;
        // other members as before
    };

## 15.7.3 派生类复制控制成员

初始化阶段，派生类的构造函数初始化派生类对象的基类部分和自己的成员。因此派生类的复制和移动构造函数必须复制和移动派生类对象的基类部分和自己的成员。类似地，派生类的赋值操作符必须赋值基类部分给派生类对象。

不像构造函数和赋值操作符，析构函数只对派生类自己的申请的资源负责。

> **警告**
> 当派生类定义了复制和移动操作，这些操作负责整个派生类对象的复制和移动，包括基类成员。

### 定义派生类复制和移动构造函数

当我们为派生类定义一个复制或移动构造函数，我们通常使用相应的基类构造函数来初始化对象的基类部分：

    class Base { /* ...    */ } ;
    class D: public Base {
    public:
        // by default, the base class default constructor initializes the base part of an object
        // to use the copy or move constructor, we must explicitly call that
        // constructor in the constructor initializer list
        D(const D& d): Base(d)      // copy the base members
                     /* initializers for members of D */ { /* ...  */ }
        D(D&& d): Base(std::move(d)) // move the base members
                     /* initializers for members of D */ { /* ...  */ }
    };

> **警告**
> 默认地，基类默认构造函数初始化派生类对象的基类部分。如果我们想要复制或移动基类部分，必须在派生类构造函数的初始化列表中显式使用基类的复制或移动构造函数。

### 派生类赋值操作符

和复制或移动构造函数一样，派生类的赋值操作符必须显式赋值其基类部分：

    // Base::operator=(const Base&) is not invoked automatically
    D &D::operator=(const D &rhs)
    {
        Base::operator=(rhs); // assigns the base part
        // assign the members in the derived class, as usual,
        // handling self-assignment and freeing existing resources as appropriate
        return *this;
    }

值得注意的是派生类构造函数和赋值操作符可以使用基类的相应操作，不管基类是否定义了这些操作还是使用合成的版本。

### 派生类析构函数

一个对象的数据成员在析构函数执行完毕之后被隐式销毁。类似地，对象的基类部分也是隐式销毁。因此和构造函数和赋值操作符不同的是，派生类析构函数只对销毁派生类申请的资源负责。

    class D: public Base {
    public:
        // Base::~Base invoked automatically
        ~D() { /* do what it takes to clean up derived members   */ }
    };

### 在构造函数和析构函数调用虚函数

我们已经知道，派生类对象的基类部分先被构造。当基类构造函数在执行时，派生类部分没有初始化。同样，派生类对象以相反的顺序被销毁，因此当基类的析构函数运行时，派生类部分已经被销毁。因此当这些基类成员在执行时，对象是不完整的。

为了适应这种不完整性，编译器在构造和析构之间将对象的类型看成是变化的。当对象被构造时，将它的类型看成和构造函数一样的类型，虚函数的调用会绑定到和构造函数一样的类型。对于析构函数也一样。

> **注解**
> 如果构造函数和析构函数调用一个虚函数，虚函数运行版本是相对于构造函数或析构函数自己类型的版本。

## 15.7.4 继承的构造函数

新标准下，派生类可以重用基类定义的构造函数。和只能初始化直接基类的原因一样，一个类只能继承直接基类的构造函数，不能继承默认的复制和移动构造函数。使用**using**声明指定继承基类的构造函数。

    class Bulk_quote : public Disc_quote {
    public:
        using Disc_quote::Disc_quote; // inherit Disc_quote's constructors
        double net_price(std::size_t) const;
    };

**using**声明只是使一个名字在当前作用域可见。当应用到构造函数，一个**using**声明引起编译器生成代码。编译器生成对应于每一个基类构造函数的派生类构造函数。这些编译器生成的构造函数具有以下形式：

    derived(parms) : base(args) { }

在Bulk_quote类，继承的构造函数如下：

    Bulk_quote(const std::string& book, double price,
              std::size_t qty, double disc):
          Disc_quote(book, price, qty, disc) { }

### 继承构造函数的特征

不像对普通成员使用**using**声明，构造函数**using**声明不会改变继承构造函数的访问级别。而且，**using**声明不能指定**explicit**或**constexpr**。如果构造函数在基类中是**explicit**或**constexpr**，则继承构造函数有一样的属性。

如果基类构造函数有默认实参，这些参数不会被继承。相反，派生类得到多个继承构造函数，每一个有默认实参的参数被相继忽略。比如，如果基类有一个2个参数的构造函数，第2个参数有默认实参，则派生类获得2个构造函数：一个带2个参数（没有默认实参）和一个带单一参数对应于基类最左边，非默认的参数的构造函数。

如果基类有多个构造函数，则除了两个例外，派生类继承基类每一个构造函数。第一个例外是派生类继承一些构造函数并定义其它构造函数自己的版本。如果派生类定义了一个与基类构造函数参数一样的构造函数，那么这个构造函数不会被继承。第二个例外是默认，复制和移动构造函数不会被继承。一个继承构造函数不会被看作用户定义的构造函数。因此，一个只有继承构造函数的类会合成默认构造函数。

# 15.8 容器和继承

当我们使用一个容器来存储来自继承层次的对象，通常我们必须间接存储这些对象。我们不能把继承相关的类型对象直接放进容器，因为没有办法定义一个容器存放不同类型的元素。

vector不能存放Quote类型可能不明显。这种情况下，我们能把Bulk_quote对象放进容器，但是这些对象不再是Bulk_quote对象了：

    vector<Quote> basket;
    basket.push_back(Quote("0-201-82470-1", 50));
    // ok, but copies only the Quote part of the object into basket
    basket.push_back(Bulk_quote("0-201-54848-8", 50, 10, .25));
    // calls version defined by Quote, prints 750, i.e., 15 * $50
    cout << basket.back().net_price(15) << endl;

> **警告**
> 因为派生类对象赋值给基类对象被切掉，容器和继承相关的类型不能很好的融合。

## 将（智能）指针而不是对象放进容器

当我们需要容器存放继承相关的对象，定义容器存放基类的指针。和往常一样，指针指向的对象的动态类型可能是基类类型或派生类类型：

    vector<shared_ptr<Quote>> basket;
    basket.push_back(make_shared<Quote>("0-201-82470-1", 50));
    basket.push_back(
        make_shared<Bulk_quote>("0-201-54848-8", 50, 10, .25));
    // calls the version defined by Quote; prints 562.5, i.e., 15 * $50 less the discount
    cout << basket.back()->net_price(15) << endl;

就像我们能够转换指向派生类对象的普通指针为指向基类的普通指针，我们也能转换指向派生类的智能指针为指向基类的智能指针。

## 15.8.1 编写Basket类

C++中的面向对象编程的一个讽刺是我们不能使用对象直接支持它。反而，我们必须使用指针和引用。由于指针强加给程序的复杂性，我们通常定义辅助的类来帮助管理复杂性。

    class Basket {
    public:
        // Basket uses synthesized default constructor and copy-control members
        void add_item(const std::shared_ptr<Quote> &sale)
            { items.insert(sale); }
        // prints the total price for each book and the overall total for all items in the
    basket
        double total_receipt(std::ostream&) const;
    private:
        // function to compare shared_ptrs needed by the multiset member
        static bool compare(const std::shared_ptr<Quote> &lhs,
                            const std::shared_ptr<Quote> &rhs)
        { return lhs->isbn() < rhs->isbn(); }
        // multiset to hold multiple quotes, ordered by the compare member
        std::multiset<std::shared_ptr<Quote>, decltype(compare)*>
                      items{compare};
    };

### 定义Basket的成员

    double Basket::total_receipt(ostream &os) const
    {
        double sum = 0.0;   // holds the running total
        // iter refers to the first element in a batch of elements with the same ISBN
        // upper_bound returns an iterator to the element just past the end of that batch
        for (auto iter = items.cbegin();
                  iter != items.cend();
                  iter = items.upper_bound(*iter)) {
            // we know there's at least one element with this key in the Basket
            // print the line item for this book
            sum += print_total(os, **iter, items.count(*iter));
        }
        os << "Total Sale: " << sum << endl; // print the final overall
    total
        return sum;
    }

### 隐藏指针

Basket的用户仍然需要处理动态分配的内存，因为add_item带一个shared_ptr参数。

    Basket bsk;
    bsk.add_item(make_shared<Quote>("123", 45));
    bsk.add_item(make_shared<Bulk_quote>("345", 45, 3, .15));

我们下一步将重定义add_item来接收Quote对象而不是shared_ptr。

    void add_item(const Quote& sale);  // copy the given object
    void add_item(Quote&& sale);       // move the given object

### 模拟虚复制

我们将通过给Quote类一个虚成员分配自己的拷贝来解决这个问题。

    class Quote {
    public:
        // virtual function to return a dynamically allocated copy of itself
        // these members use reference qualifiers; see §13.6.3 (p. 546)
        virtual Quote* clone() const & {return new Quote(*this);}
        virtual Quote* clone() && {return new Quote(std::move(*this));}
        // other members as before
    };
    class Bulk_quote : public Quote {
        Bulk_quote* clone() const & {return new Bulk_quote(*this);}
        Bulk_quote* clone() && { return new Bulk_quote(std::move(*this));}
        // other members as before
    };

    class Basket {
    public:
        void add_item(const Quote& sale) // copy the given object
          { items.insert(std::shared_ptr<Quote>(sale.clone())); }
        void add_item(Quote&& sale)      // move the given object
          { items.insert(
              std::shared_ptr<Quote>(std::move(sale).clone())); }
        // other members as before
    };

# 15.9 文本查询重温
