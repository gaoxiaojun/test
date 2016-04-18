#include <iostream>
using namespace std;

class Super
{
private:
    int i;
public:
    virtual ~Super() {}
    virtual void f() {}
    virtual void g() {}
};

class Child: public Super
{
private:
    int j;
    int *p;
};

int main()
{
    Super s;
    Child c;

    cout << "sizeof(Super): " << sizeof(s) << endl;
    cout << "sizeof(Child): " << sizeof(c) << endl;

    return 0;
}
