title: 面向对象编程
date: 2015-09-21 09:04:53
tags:
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


