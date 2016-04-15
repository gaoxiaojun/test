#include <iostream>
#include <string>
using namespace std;

int main()
{
    string *p = new string("Hello");
    string *q = new string[1];
    q[0] = "world";

    cout << *p << endl;
    cout << q[0] << endl;

    delete p;
    delete[] q;

    return 0;
}
