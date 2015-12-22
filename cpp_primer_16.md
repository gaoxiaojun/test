title: 第16章 模板和泛型编程
date: 2015-12-14 10:34:42
tags: [C/C++]
---

面向对象编程和泛型编程都是处理程序编写时类型未知的情况。两者的区别是面向对象编程直到运行时类型才能知道，而泛型编程类型在编译期间知道。

容器，迭代器和算法都是泛型编程的例子。当编写一个泛型程序，我们用一种独立于任何类型的方式编写代码。当使用一个泛型程序，我们提供程序的实例将操作的类型或值。

模板是C++中泛型编程的基础。模板是一个创建类或函数的公式或蓝图。当我们使用一个泛型类型，我们提供将蓝图转换为指定类或函数所需的信息。这个转换发生在编译期间。

# 16.1 定义一个模板

想象我们要写一个函数比较两个值的大小。实际中，我们要定义多个这样的函数，每一个比较指定类型的值。

    // returns 0 if the values are equal, -1 if v1 is smaller, 1 if v2 is smaller
    int compare(const string &v1, const string &v2)
    {
        if (v1 < v2) return -1;
        if (v2 < v1) return 1;
        return 0;
    }
    int compare(const double &v1, const double &v2)
    {
        if (v1 < v2) return -1;
        if (v2 < v1) return 1;
        return 0;
    }

这样的函数几乎相同：唯一不同是参数的类型不同，每一个函数体都相同。

## 16.1.1 函数模板

我们定义一个函数模板而不是为每一种类型定义一个新函数。一个函数模板是一个公式，从这个公式我们能够生成指定类型的函数。

    template <typename T>
    int compare(const T &v1, const T &v2)
    {
        if (v1 < v2) return -1;
        if (v2 < v1) return 1;
        return 0;
    }

模板定义开始于关键字**template**后面跟着模板参数列表。模板参数列表由<>括起来，逗号分隔的一个或多个模板参数组成。

> **注释**
> 在模板定义中，模板参数列表不能为空。

模板参数列表的行为非常像函数的参数列表。模板参数表示用在类或函数定义中的类型或值。当我们使用一个模板，我们指定一个（隐式或显式）模板实参绑定到模板参数。

### 实例化函数模板

当我们调用一个函数模板，编译器使用函数调用的实参为我们推导模板实参。

    cout << compare(1, 0) << endl;       // T is int

编译器使用推导的模板参数来为我们实例化指定版本的函数。当编译器实例化一个模板，它使用实际的模板实参创建一个新的模板实例代替相应的模板参数。

    // instantiates int compare(const int&, const int&)
    cout << compare(1, 0) << endl;       // T is int
    // instantiates int compare(const vector<int>&, const vector<int>&)
    vector<int> vec1{1, 2, 3}, vec2{4, 5, 6};
    cout << compare(vec1, vec2) << endl; // T is vector<int>

### 模板类型参数

一般而言，我们可以和使用内置类型或类类型一样的方式使用类型参数作为类型说明符。特别地，类型参数可以用来命名一个返回类型，函数参数类型，变量声明或函数内类型转换。

    // ok: same type used for the return type and parameter
    template <typename T> T foo(T* p)
    {
        T tmp = *p; // tmp will have the type to which p points
        // ...
        return tmp;
    }

每一个类型参数之前必须有**class**或**typename**关键字。

    // error: must precede U with either typename or class
    template <typename T, U> T calc(const T&, const U&);

使用关键字**typename**指定模板类型参数比**class**似乎更直观。毕竟我们可以使用内置（非类类型）类型作为模板的类型实参。而且**typename**更加清晰地指示了跟在它后面的名字是一个类型名。但是，**typename**是在模板广泛使用之后才被加入C++的。

### 非类型模板参数

除了定义类型参数，我们还能定义非类型的模板参数。一个非类型参数代表一个值而不是类型。非类型参数由一个指定的类型名指定而不是**class**或**typename**关键字。

当模板实例化时，非类型参数被用户提供的或编译器推导的一个值替换。这些值必须是常量表达式，以允许编译器在编译期间实例化模板。

    template<unsigned N, unsigned M>
    int compare(const char (&p1)[N], const char (&p2)[M])
    {
        return strcmp(p1, p2);
    }

    compare("hi", "mom")
    int compare(const char (&p1)[3], const char (&p2)[4])

一个非类型参数可能是整型，对象或函数的指针或（左值）引用。绑定到非类型的整型参数的实参必须是常量表达式。绑定到指针或引用的非类型参数必须拥有静态生命周期。不能使用普通（非静态）局部对象或动态分配的对象作为模板实参传给指针或引用的非类型模板参数。指针参数也能用**nullptr**或0值常量表达式初始化。

> **注释**
> 用于非类型模板参数的模板实参必须是常量表达式。

### inline和constexpr函数模板

和非模板函数一样，函数模板可以被声明为**inline**或**constexpr**。**inline**或**constexpr**限定符跟在模板参数列表之后，函数返回类型之前。

    // ok: inline specifier follows the template parameter list
    template <typename T> inline T min(const T&, const T&);
    // error: incorrect placement of the inline specifier
    inline template <typename T> T min(const T&, const T&);

### 编写类型独立的代码

尽管简单，compare函数展示了编写泛型代码的两个重要原则：
* 模板中的函数参数是const引用
* 函数体中的比较只使用<

只使用<运算符，我们降低了对使用compare函数的类型的要求。这些类型只需要支持<，没有必要支持>。实际上，如果我们真正关心类型独立和可移植性，我们可能应该使用**less**定义函数。

    // version of compare that will be correct even if used on pointers; see § 14.8.2 (p.
    575)
    template <typename T> int compare(const T &v1, const T &v2)
    {
        if (less<T>()(v1, v2)) return -1;
        if (less<T>()(v2, v1)) return 1;
        return 0;
    }

> **最佳实践**
> 模板程序应该尽量减少作用在参数类型的要求的数量。

### 模板编译

当编译器看到模板的定义，并没有生成代码。只有当实例化一个指定的模板实例的时候生成代码。只有使用模板时生成代码的事实影响我们如何组织源代码和如何检测错误。

通常当我们调用一个函数，编译器只需要看到函数的声明。类似地，当我们使用类对象，类定义必须可用，但是成员函数的定义不需要提供。因此，我们把类定义和函数声明放在头文件而普通函数和成员函数放在源文件。

模板不一样：为了生成一个模板实例，编译器需要定义函数模板或类模板成员函数的代码。因此不像非模板代码，模板头文件既包含了定义也包含了声明。

> **注释**
> 函数模板和类模板成员函数的定义通常放进头文件。

> **关键概念：模板和头文件**
> 模板包含两种名字：
> * 那些不依赖模板参数的名字
> * 那些依赖模板参数的名字
> 
> 由模板的提供者保证当模板使用时所有不依赖模板参数的名字可见。而且，模板的提供者必须保证当模板实例化的时候模板的定义，包括类模板成员的定义可见。由模板的用户保证用来实例化模板所有的函数，类型和类型相关的操作符可见。
> 模板的作者应该提供一个头文件，包含模板定义，连同所有类模板或成员定义中使用的名字的声明。模板用户必须包含模板头文件和任意用来实例化模板的类型。

### 实例化期间最常报的编译错误

第一个阶段是编译模板自己的时候。编译器在这个阶段通常不能找到很多错误。编译器能查找出语法错误，比如忘记标点或变量拼写错误。

第二个阶段是编译器看到模板的使用的时候。这个阶段也没有很多需要检查。对于一个函数模板调用，编译器只是检查参数的个数是否合适。也可以检查两个类型应该一样的参数是否真的相同。对于类模板，编译器能检查正确的模板参数个数。

第三个阶段是实例化的时候。只有这个时候类型相关的错误才能被发现。取决于编译器如何处理实例化，这些错误可能会在链接时报告。

> **警告**
> 由调用者保证传递给模板的参数支持模板使用到的任意操作，且这些操作在模板使用的上下文环境表现正确。

## 16.1.2 类模板

类模板是产生类的蓝图。和函数模板不同，编译器不能为类模板推导模板参数。因此为了使用类模板，我们必须在模板名字后面的尖括号里面提供额外的信息。这些信息就是用来代替模板参数的模板实参列表。

### 定义一个类模板

和函数模板一样，类模板以**template**关键字开头，后跟一个模板参数列表。在类模板（和它的成员）的定义中，我们使用模板参数作为当模板使用时提供的类型或值的替身。

    template <typename T> class Blob {
    public:
        typedef T value_type;
        typedef typename std::vector<T>::size_type size_type;
        // constructors
        Blob();
        Blob(std::initializer_list<T> il);
        // number of elements in the Blob
        size_type size() const { return data->size(); }
        bool empty() const { return data->empty(); }
        // add and remove elements
        void push_back(const T &t) {data->push_back(t);}
        // move version; see § 13.6.3 (p. 548)
        void push_back(T &&t) { data->push_back(std::move(t)); }
        void pop_back();
        // element access
        T& back();
        T& operator[](size_type i); // defined in § 14.5 (p. 566)
    private:
        std::shared_ptr<std::vector<T>> data;
        // throws msg if data[i] isn't valid
        void check(size_type i, const std::string &msg) const;
    };

### 实例化一个类模板

当我们使用一个类模板时，必须提供额外的信息。这些额外的信息是显式模板实参列表。编译器使用这些模板实参来实例化一个指定类。

    Blob<int> ia;                // empty Blob<int>
    Blob<int> ia2 = {0,1,2,3,4}; // Blob<int> with five elements

    template <> class Blob<int> {
        typedef typename std::vector<int>::size_type size_type;
        Blob();
        Blob(std::initializer_list<int> il);
        // ...
        int& operator[](size_type i);
    private:
        std::shared_ptr<std::vector<int>> data;
        void check(size_type i, const std::string &msg) const;
    };

当编译器从Blob模板实例化一个类时，它重写了Blob模板，用int替换掉每一个模板参数T。编译器为每一个我们指定的类型生成一个不同的类。

    // these definitions instantiate two distinct Blob types
    Blob<string> names; // Blob that holds strings
    Blob<double> prices;// different element type

> **注释**
> 每一个类模板的实例化构成了一个独立的类。类型Blob<string>和任何其它的Blob类型没有关系，也没有任何特殊权限访问其它Blob类型的成员。

### 模板作用域中模板类型的引用

类模板用来实例化一个类型，且被实例化的类型总是包含模板实参。类模板中的代码通常不使用实际类型（或值）的名字作为模板实参。相反，我们经常使用模板自己的参数作为模板实参。

    std::shared_ptr<std::vector<T>> data;
    shared_ptr<vector<string>>

### 类模板成员函数

和任意其它类一样，我们能够在类里面或外面定义类模板的成员函数。定义在类里面的成员函数默认为**inline**。类模板成员函数本身是个普通函数。但是类模板每一个实例都拥有自己版本的成员。因此类模板的成员函数拥有和类模板相同的模板参数。所以定义在类模板外面的成员函数以关键字**template**开头，后接类模板参数列表。

    template <typename T>
    ret-type Blob<T>::member-name(parm-list)

### check和元素访问成员

    template <typename T>
    void Blob<T>::check(size_type i, const std::string &msg)
    const
    {
        if (i >= data->size())
            throw std::out_of_range(msg);
    }

    template <typename T>
    T& Blob<T>::back()
    {
        check(0, "back on empty Blob");
        return data->back();
    }
    template <typename T>
    T& Blob<T>::operator[](size_type i)
    {
        // if i is too big, check will throw, preventing access to a nonexistent element
        check(i, "subscript out of range");
        return (*data)[i];
    }

    template <typename T> 
    void Blob<T>::pop_back()
    {
        check(0, "pop_back on empty Blob");
        data->pop_back();
    }

### Blob构造函数

和其它定义在类模板外面的成员函数一样，构造函数以声明模板参数开始。

    template <typename T>
    Blob<T>::Blob(): data(std::make_shared<std::vector<T>>()) { }

    template <typename T>
    Blob<T>::Blob(std::initializer_list<T> il):
                  data(std::make_shared<std::vector<T>>(il)) { }

### 类模板成员函数实例化

默认地，类模板的成员函数只有在程序使用这个函数时实例化。

    // instantiates Blob<int> and the initializer_list<int> constructor
    Blob<int> squares = {0,1,2,3,4,5,6,7,8,9};
    // instantiates Blob<int>::size() const
    for (size_t i = 0; i != squares.size(); ++i)
        squares[i] = i*i; // instantiates Blob<int>::operator[](size_t)

成员只有在使用时实例化这个事实使我们可以用一个可能不满足某些模板操作要求的类型实例化一个类。

> **注释**
> 默认地，一个实例化的类模板的成员只有当成员使用时才实例化。

### 类代码里面简化模板类名的使用

使用一个类模板类型必须提供一个模板实参有一个例外，在类模板作用域里面，我们可以不带模板实参使用模板名字。

    // BlobPtr throws an exception on attempts to access a nonexistent element
    template <typename T> class BlobPtr {
    public:
        BlobPtr(): curr(0) { }
        BlobPtr(Blob<T> &a, size_t sz = 0):
                wptr(a.data), curr(sz) { }
        T& operator*() const
        { auto p = check(curr, "dereference past end");
          return (*p)[curr];  // (*p) is the vector to which this object points
        }
        // increment and decrement
        BlobPtr& operator++();        // prefix operators
        BlobPtr& operator--();
    private:
        // check returns a shared_ptr to the vector if the check succeeds
        std::shared_ptr<std::vector<T>>
            check(std::size_t, const std::string&) const;
        // store a weak_ptr, which means the underlying vector might be destroyed
        std::weak_ptr<std::vector<T>> wptr;
        std::size_t curr;      // current position within the array
    };

### 在类模板外面使用类模板名字

当我们在类模板外面定义成员时，必须记住我们不在类的作用域直到类名被看见。

    // postfix: increment/decrement the object but return the unchanged value
    template <typename T>
    BlobPtr<T> BlobPtr<T>::operator++(int)
    {
        // no check needed here; the call to prefix increment will do the check
        BlobPtr ret = *this;  // save the current value
        ++*this;    // advance one element; prefix ++ checks the increment
        return ret;  // return the saved state
    }

当我们没有提供模板实参，编译器假设我们使用成员实例化一样的类型。因此，ret的定义就好像：

    BlobPtr<T> ret = *this;

> **注释**
> 在类模板作用域里面，我们可以使用模板，而不指定模板实参。

### 类模板和友元

当一个类包含友元声明时，类和友元各自可以是模板或非模板。一个有非模板的友元的类模板授予其友元访问模板所有实例。当友元本身也是模板时，授予友元关系的类控制友元模板所有实例或指定实例友元关系。

### 一对一友元关系

从一个类模板到另一个模板（类模板或函数模板）建立相应实例之间的友元关系是最常见友元关系形式。为了引用一个指定的模板实例（类模板或函数模板），我们必须先声明模板。一个模板声明包含模板参数列表。

    // forward declarations needed for friend declarations in Blob
    template <typename> class BlobPtr;
    template <typename> class Blob; // needed for parameters in operator==
    template <typename T>
        bool operator==(const Blob<T>&, const Blob<T>&);
    template <typename T> class Blob {
        // each instantiation of Blob grants access to the version of
        // BlobPtr and the equality operator instantiated with the same type
        friend class BlobPtr<T>;
        friend bool operator==<T>
               (const Blob<T>&, const Blob<T>&);
        // other members as in § 12.1.1 (p. 456)
    };

    Blob<char> ca; // BlobPtr<char> and operator==<char> are friends
    Blob<int> ia;  // BlobPtr<int> and operator==<int> are friends

### 通用和指定模板友元

一个类也可以指定另一个模板的每一个实例为友元，或限制指定实例为友元：

    // forward declaration necessary to befriend a specific instantiation of a template
    template <typename T> class Pal;
    class C {  //  C is an ordinary, nontemplate class
        friend class Pal<C>;  // Pal instantiated with class C is a friend to C
        // all instances of Pal2 are friends to C;
        // no forward declaration required when we befriend all instantiations
        template <typename T> friend class Pal2;
    };
    template <typename T> class C2 { // C2 is itself a class template
        // each instantiation of C2 has the same instance of Pal as a friend
        friend class Pal<T>;  // a template declaration for Pal must be in scope
        // all instances of Pal2 are friends of each instance of C2, prior declaration needed
        template <typename X> friend class Pal2;
        // Pal3 is a nontemplate class that is a friend of every instance of C2
        friend class Pal3;    // prior declaration for Pal3 not needed
    };

为了允许所有的实例都是友元，友元声明必须使用跟类不一样的模板参数。

### 模板类型参数作为友元

在新标准下，我们可以将模板类型参数作为友元：

    template <typename Type> class Bar {
    friend Type; // grants access to the type used to instantiate Bar
        //  ...
    };

值得注意的是尽管友元通常必须是一个类或函数，用内置类型实例化类Bar也可以，因此友元为内置类型在这种情况也允许。

### 模板类型别名

类模板的一个实例定义了一个类类型，和其它类类型一样，我们可以使用**typedef**引用它：

    typedef Blob<string> StrBlob;

因为模板不是一个类型，我们不能使用**typedef**引用一个模板，即没有办法用**typedef**引用Blob<T>。但是新标准允许我们为类模板定义一个类型别名：

    template<typename T> using twin = pair<T, T>;
    twin<string> authors; // authors is a pair<string, string>

模板类型别名是一个类家庭的同义词：

    twin<int> win_loss;  // win_loss is a pair<int, int>
    twin<double> area;   // area is a pair<double, double>

当我们定义一个模板类型别名，我们可以固定一个或多个模板参数：

    template <typename T> using partNo = pair<T, unsigned>;
    partNo<string> books;  // books is a pair<string, unsigned>
    partNo<Vehicle> cars;  // cars is a pair<Vehicle, unsigned>
    partNo<Student> kids;  // kids is a pair<Student, unsigned>

### 类模板的静态成员

和其它类一样，类模板可以声明静态成员：

    template <typename T> class Foo {
    public:
       static std::size_t count() { return ctr; }
       // other interface members
    private:
       static std::size_t ctr;
       // other implementation members
    };

每一个Foo的实例都有自己静态成员。

    // instantiates static members Foo<string>::ctr and Foo<string>::count
    Foo<string> fs;
    // all three objects share the same Foo<int>::ctr and Foo<int>::count members
    Foo<int> fi, fi2, fi3;

和其它静态数据成员一样，类模板的每一个静态数据成员必须只有一个定义。

    template <typename T>
    size_t Foo<T>::ctr = 0; // define and initialize ctr

必须引用指定的实例来访问静态成员。

    Foo<int> fi;                 // instantiates Foo<int> class
                                 // and the static data member ctr
    auto ct = Foo<int>::count(); // instantiates Foo<int>::count
    ct = fi.count();             // uses Foo<int>::count
    ct = Foo::count();           // error: which template instantiation?

和其它成员函数一样，静态成员函数只有在程序中使用时才实例化。

## 16.1.3 模板参数

和函数参数的名字一样，模板参数的名字没有实质意义。我们通常命名类型参数为T，也可以使用人用名字：

    template <typename Foo> Foo calc(const Foo& a, const Foo& b)
    {
        Foo tmp = a; // tmp has the same type as the parameters and return type
        // ...
        return tmp;  // return type and parameters have the same type
    }

### 模板参数和作用域

模板参数遵循正常的作用域规则。模板参数的名字在声明之后直到模板声明或定义结束之前都能使用。和其它名字一样，模板参数名隐藏任何外围声明的名字。但是一个被用来作为模板参数的名字在模板内不能被重复使用。

    typedef double A;
    template <typename A, typename B> void f(A a, B b)
    {
        A tmp = a; // tmp has same type as the template parameter A, not double
        double B;  // error: redeclares template parameter B
    }

因为模板参数名不能重复使用，模板参数列表中的每一个模板参数名只能出现一次：

    // error: illegal reuse of template parameter name V
    template <typename V, typename V> // ...

### 模板声明

模板声明必须包含模板参数：

    // declares but does not define compare and Blob
    template <typename T> int compare(const T&, const T&);
    template <typename T> class Blob;

和函数参数一样，模板参数名不需要和定义中的模板参数名一样：

    // all three uses of calc refer to the same function template
    template <typename T> T calc(const T&, const T&); // declaration
    template <typename U> U calc(const U&, const U&); // declaration
    // definition of the template
    template <typename Type>
    Type calc(const Type& a, const Type& b) { /* . . . */ }

当然，模板的每一个声明和定义参数的数量和种类（类型或非类型）必须一样。

> **最佳实践**
> 一个文件需要的所有模板的声明通常应该一起出现在文件开头，在任何代码使用这些名字之前。

### 使用为类型的类成员

我们可以使用作用域操作符（::）访问**static**成员和类型成员。假设T是一个模板类型参数，当编译器看到T::mem时，在实例化前它不知道mem是一个类型还是静态数据成员。但是为了处理模板，编译器必须知道一个名字是否代表类型。比如下面这个例子：

    T::size_type * p;

编译器需要知道这是定义一个变量p还是静态数据成员与p相乘。语言默认假设通过作用域操作符访问的名字不是一个类型。因此如果我们想要使用模板类型参数的类型成员，我们必须显式告诉编译器这个名字是一个类型。我们使用关键字**typename**来做这件事：

    template <typename T>
    typename T::value_type top(const T& c)
    {
        if (!c.empty())
            return c.back();
        else
            return typename T::value_type();
    }

> **注释**
> 当我们通知编译器一个名字代表类型时，必须使用关键字**typename**，而不是**class**。

### 默认模板实参

就像我们能够给函数参数提供默认实参，我们也能提供默认模板实参。新标准下，我们能给函数模板和类模板提供默认实参。早先的版本只允许类模板有默认实参。

    // compare has a default template argument, less<T>
    // and a default function argument, F()
    template <typename T, typename F = less<T>>
    int compare(const T &v1, const T &v2, F f = F())
    {
        if (f(v1, v2)) return -1;
        if (f(v2, v1)) return 1;
        return 0;
    }

    bool i = compare(0, 42); // uses less; i is -1
    // result depends on the isbns in item1 and item2
    Sales_data item1(cin), item2(cin);
    bool j = compare(item1, item2, compareIsbn);

### 模板默认实参和类模板

不管什么时候使用类模板，我们必须在模板名后面跟尖括号。尖括号指示类必须从模板实例化。特别地，如果一个类模板为所有模板参数提供了默认实参，且我们使用这些默认值，我们必须在模板名后面放一对空的尖括号：

    template <class T = int> class Numbers {   // by default T is int
    public:
        Numbers(T v = 0): val(v) { }
        // various operations on numbers
    private:
        T val;
    };
    Numbers<long double> lots_of_precision;
    Numbers<> average_precision; // empty <> says we want the default type

## 16.1.4 成员模板

一个类（不管是普通类还是类模板）可能有本身是模板的成员函数。这种成员被称为成员模板。成员模板不能是虚函数。

### 普通类的成员模板

    // function-object class that calls delete on a given pointer
    class DebugDelete {
    public:
        DebugDelete(std::ostream &s = std::cerr): os(s) { }
        // as with any function template, the type of T is deduced by the compiler
        template <typename T> void operator()(T *p) const
          { os << "deleting unique_ptr" << std::endl; delete p;
    }
    private:
        std::ostream &os;
    };

和其他模板一样，一个成员模板以它自己的模板参数列表开始。

    double* p = new double;
    DebugDelete d;    // an object that can act like a delete expression
    d(p); // calls DebugDelete::operator()(double*), which deletes p
    int* ip = new int;
    // calls operator()(int*) on a temporary DebugDelete object
    DebugDelete()(ip);

    // destroying the the object to which p points
    // instantiates DebugDelete::operator()<int>(int *)
    unique_ptr<int, DebugDelete> p(new int, DebugDelete());
    // destroying the the object to which sp points
    // instantiates DebugDelete::operator()<string>(string*)
    unique_ptr<string,DebugDelete> sp(new string, DebugDelete());

**unique_ptr**的析构函数会调用DebugDelete的调用操作符。因此，当**unique_ptr**析构函数实例化时，DebugDelete的调用操作符也会实例化。

// sample instantiations for member templates of DebugDelete
void DebugDelete::operator()(int *p) const { delete p; }
void DebugDelete::operator()(string *p) const { delete p; }

### 类模板的成员模板

我们也可以定义一个类模板的成员模板。这种情况下，类和成员都有它们自己的独立的模板参数。

    template <typename T> class Blob {
        template <typename It> Blob(It b, It e);
        // ...
    };

不同于类模板的普通函数成员，成员模板是函数模板。当我们在类模板外面定义一个成员模板，我们必须为类模板和函数模板提供模板参数列表。类模板参数列表在前，后面跟成员模板参数列表：

    template <typename T>     // type parameter for the class
    template <typename It>    // type parameter for the constructor
        Blob<T>::Blob(It b, It e):
                  data(std::make_shared<std::vector<T>>(b, e)) {
    }

### 成员模板的实例化

为了实例化一个类模板的成员模板，我们必须为类模板和成员模板的模板参数提供实参。和以前一样，类模板参数的实参由调用成员模板的对象类型决定。同样，编译器通过传递给成员模板的实参推导出成员模板参数的实参。

    int ia[] = {0,1,2,3,4,5,6,7,8,9};
    vector<long> vi = {0,1,2,3,4,5,6,7,8,9};
    list<const char*> w = {"now", "is", "the", "time"};
    // instantiates the Blob<int> class
    // and the Blob<int> constructor that has two int* parameters
    Blob<int> a1(begin(ia), end(ia));
    // instantiates the Blob<int> constructor that has
    // two vector<long>::iterator parameters
    Blob<int> a2(vi.begin(), vi.end());
    // instantiates the Blob<string> class and the Blob<string>
    // constructor that has two (list<const char*>::iterator parameters
    Blob<string> a3(w.begin(), w.end());

## 16.1.5 控制实例化

当模板被使用时才生成实例意味着同一个实例可能出现在多个对象文件。当两个或更多单独编译的源文件用相同的模板实参使用同一个模板时，在每一个文件中有一个模板实例。

在大型系统中，在多个文件中实例化同一个模板的开销将变得非常明显。在新标准下，我们可以使用显式实例化避免这种开销。显式实例化具有形式：

    extern template declaration; // instantiation declaration
    template declaration;        // instantiation definition

declaration是一个类或函数声明，其模板参数被模板实参替换。

    // instantion declaration and definition
    extern template class Blob<string>;             // declaration
    template int compare(const int&, const int&);   // definition

当编译器看到一个**extern**模板声明，它不会在那个文件生成实例化代码。将一个模板实例声明为**extern**承诺程序的其它地方存在一个**nonextern**的实例。对于一个指定的实例可以有多个**extern**声明但是只能存在一个实例的定义。

因为编译器使用模板时自动实例化，**extern**声明必须出现在任何使用实例的代码之前：

    // Application.cc
    // these template types must be instantiated elsewhere in the program
    extern template class Blob<string>;
    extern template int compare(const int&, const int&);
    Blob<string> sa1, sa2; // instantiation will appear elsewhere
    // Blob<int> and its initializer_list constructor instantiated in this file
    Blob<int> a1 = {0,1,2,3,4,5,6,7,8,9};
    Blob<int> a2(a1);  // copy constructor instantiated in this file
    int i = compare(a1[0], a2[0]); // instantiation will appear elsewhere定义

> **警告**
> 对于每一个实例声明，在程序的其它地方必须有一个显式实例定义。

### 实例定义实例化所有成员

一个类模板的实例定义实例化其所有成员包括内联成员函数。当编译器看到一个实例定义，它不知道哪一个成员函数会被用到，因此编译器实例化所有成员。

> **注释**
> 实例定义只能用在类模板所有成员函数都能使用的类型。

## 16.1.6 效率和灵活性

标准库的智能指针提供了一个关于模板设计者面临的设计选择的很好说明。

# 16.2 模板实参推导

在模板实参推导期间，编译器使用实参的类型查找模板参数并生成最符合的函数。

## 16.2.1 转换和模板类型参数

使用模板类型参数的函数参数具有特殊的初始化规则。只有非常有限数量的转换被自动应用到这种参数。编译器生成一个新的实例而不是转换实参。

和以往一样，不管是形参还是实参的上层const都被忽略。在一个函数模板调用中执行的唯一其它转换有：
* const转换：引用（或指针）const对象的函数参数可以传递引用（或指针）非const对象。
* 数组到指针或函数到指针的转换：如果函数参数不是引用类型，则普通指针转换将被应用到参数为数组或函数类型上。数组参数被转换为指向第一个元素的指针。类似地，函数参数被转换为指向函数类型的指针。

其它转换比如算术转换，派生类到基类转换和用户自定义的转换都不会执行。

    template <typename T> T fobj(T, T); // arguments are copied
    template <typename T> T fref(const T&, const T&); // references
    string s1("a value");
    const string s2("another value");
    fobj(s1, s2); // calls fobj(string, string); const is ignored
    fref(s1, s2); // calls fref(const string&, const string&)
                  // uses premissible conversion to const on s1
    int a[10], b[42];
    fobj(a, b); // calls f(int*, int*)
    fref(a, b); // error: array types don't match

> **注释**
> const转换和数组或函数到指针的转换是模板类型唯一的实参到形参自动转换。

### 使用相同模板参数类型的函数参数

一个模板类型参数可以被多个函数参数使用。因为转换非常有限，这种参数的实参必须具有一样的类型。如果推导的参数类型不匹配，那么调用出错。

    long lng;
    compare(lng, 1024); // error: cannot instantiate compare(long, int)

    // argument types can differ but must be compatible
    template <typename A, typename B>
    int flexibleCompare(const A& v1, const B& v2)
    {
        if (v1 < v2) return -1;
        if (v2 < v1) return 1;
        return 0;
    }
    long lng;
    flexibleCompare(lng, 1024); // ok: calls flexibleCompare(long, int)

### 应用到普通实参的正常转换

函数模板可以有普通类型的参数，就是与模板类型参数无关的参数。这些参数不需要特殊处理，它们的转换规则和以前一样。

    template <typename T> ostream &print(ostream &os, const T
    &obj)
    {
        return os << obj;
    }
    print(cout, 42); // instantiates print(ostream&, int)
    ofstream f("output");
    print(f, 10);    // uses print(ostream&, int); converts f to ostream&

## 16.2.2 函数模板显式参数

在某些情况下，编译器不可能推导出模板参数的类型。另一些情况，我们想要允许用户控制模板实例化。当一个函数返回类型不同于参数列表中的类型时，这两种情况经常出现。

### 指定一个显式模板参数

    // T1 cannot be deduced: it doesn't appear in the function parameter list
    template <typename T1, typename T2, typename T3>
    T1 sum(T2, T3);

这个例子中，没有用来推导T1类型的实参，调用者必须提供一个显式模板实参。我们可以像定义类模板实例那样，给一个函数调用提供显式模板实参。显式模板实参在尖括号中指定，在函数名之后，参数列表之前：

    // T1 is explicitly specified; T2 and T3 are inferred from the argument types
    auto val3 = sum<long long>(i, lng); // long long sum(int, long)

显式模板实参从左到右匹配相应的模板参数，

    // poor design: users must explicitly specify all three template parameters
    template <typename T1, typename T2, typename T3>
    T3 alternative_sum(T2, T1);

    // error: can't infer initial template parameters
    auto val3 = alternative_sum<long long>(i, lng);
    // ok: all three parameters are explicitly specified
    auto val2 = alternative_sum<long long, int, long>(i, lng);

### 显式指定参数应用普通转换

和普通类型参数允许普通转换的原因一样，显式指定参数也可以应用其实参的普通转换：

    long lng;
    compare(lng, 1024);       // error: template parameters don't match
    compare<long>(lng, 1024); // ok: instantiates compare(long, long)
    compare<int>(lng, 1024);  // ok: instantiates compare(int, int)

## 16.2.3 尾返回类型和类型转换

使用显式模板参数来表示模板函数的返回类型工作得很好。另外一些情况下，需要有一个显式模板参数给用户强加了负担。

    template <typename It>
    ??? &fcn(It beg, It end)
    {
        // process the range
        return *beg;  // return a reference to an element from the range
    }

    vector<int> vi = {1,2,3,4,5};
    Blob<string> ca = { "hi", "bye" };
    auto &i = fcn(vi.begin(), vi.end()); // fcn should return int&
    auto &s = fcn(ca.begin(), ca.end()); // fcn should return string&

    // a trailing return lets us declare the return type after the parameter list is seen
    template <typename It>
    auto fcn(It beg, It end) -> decltype(*beg)
    {
        // process the range
        return *beg;  // return a reference to an element from the range
    }

### 类型转换标准库模板类

有时候我们不能直接访问到我们需要的类型。比如我们可能想要写一个类似fcn的函数返回一个元素的值而不是引用。编写这样的函数我们面临的问题是我们几乎不了解我们传递的参数类型。为了获取元素类型，我们可以使用库类型转换模板。这些模板定义在**type_traits**头文件。通常头文件**type_traits**中的类被用来所谓的元编程。

|For Mod&lt;T&gt;, Mod is  |If T is  |Then Mod&lt;T&gt;::type is|
|------------|:----------|:----------|
|remove_reference|X& or X&&<br>otherwise|X<br>T|
|add_const|X&, const X, or function<br>otherwise|T<br>const T|
|add_lvalue_reference|X&<br>X&&<br>otherwise|T<br>X&<br>T&|
|add_rvalue_reference|X& or X&&<br>otherwise|T<br>T&&|
|remove_pointer|X*<br>otherwise|X<br>const T|
|add_pointer|X& or X&&<br>otherwise|X\*<br>T\*|
|make_signed|unsigned X<br>otherwise|X<br>T|
|make_unsigned|signed type<br>otherwise|unsigned T<br>T|
|remove_extent|X[n]<br>otherwise|X<br>T|
|remove_all_extents|X[n1][n2]...<br>otherwise|X<br>T|

在这个例子中，我们可以使用**remove_reference**获取元素类型。

    remove_reference<decltype(*beg)>::type

    // must use typename to use a type member of a template parameter; see § 16.1.3 (p.
    670)
    template <typename It>
    auto fcn2(It beg, It end) ->
        typename remove_reference<decltype(*beg)>::type
    {
        // process the range
        return *beg;  // return a copy of an element from the range
    }

注意type是一个依赖模板参数的类成员。因此我们必须在返回类型的声明中使用**typename**来告诉编译器type代表一个类型。

每一个上表中描述的类型转换模板和**remove_reference**类似工作。每一个模板都有一个公开的type成员代表一个类型。如果不可能（或没必要）转换一个模板参数，则type成员就是模板参数本身。

## 16.2.4 函数指针和实参推导

当我们从一个函数模板初始化或赋值一个函数指针时，编译器使用函数指针的类型推导模板实参。

    template <typename T> int compare(const T&, const T&);
    // pf1 points to the instantiation int compare(const int&, const int&)
    int (*pf1)(const int&, const int&) = compare;

如果从函数指针类型不能决定模板实参则出错：

    // overloaded versions of func; each takes a different function pointer type
    void func(int(*)(const string&, const string&));
    void func(int(*)(const int&, const int&));
    func(compare); // error: which instantiation of compare?

我们可以使用显式模板实参使得调用无二义性：

    // ok: explicitly specify which version of compare to instantiate
    func(compare<int>);  // passing compare(const int&, const int&)

> **注释**
> When the address of a function-template instantiation is taken, the context
must be such that it allows a unique type or value to be determined for each
template parameter.

## 16.2.5 模板实参推导和引用

