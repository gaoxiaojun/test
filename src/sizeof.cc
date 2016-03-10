#include <iostream>
using namespace std;

int main()
{
    char *p = new char[8];
    cout << "len: " << sizeof(p) << endl;
    delete p;
    return 0;
}
