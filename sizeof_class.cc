#include <iostream>
using std::cout;
using std::endl;

class A
{
public:
    virtual void f();
    virtual void g();
    A();
    ~A();
    void h();
    void foo();
private:
    int i;
    char c;
};

int main()
{
    cout << sizeof(A) << endl;
}