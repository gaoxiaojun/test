#include <iostream>
#include <string>
using namespace std;

typedef struct
{
    int age;
    string name;
    //int age;
    double salary;
} Employee;;

int main()
{
    cout << "sizeof(int): " << sizeof(int) << endl;
    cout << "sizeof(double): " << sizeof(double) << endl;
    cout << "sizeof(string): " << sizeof(string) << endl;
    cout << "sizeof(pointer): " << sizeof(void*) << endl;
    cout << "sizeof(Employee): " << sizeof(Employee) << endl;
    Employee a;
    a.name = "zhangsan";
    a.age = 18;
    a.salary = 3000.0;
    cout << "sizeof(a): " << sizeof(a) << endl;
    Employee b;
    b.name = "lisi";
    b.age = 40;
    b.salary = 20000.0;
    cout << "sizeof(b): " << sizeof(b) << endl;

    return 0;
}
