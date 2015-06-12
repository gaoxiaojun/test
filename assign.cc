#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <cstring>
using namespace std;

int main()
{
    vector<char *> oldstyle;
    list<string> names;

    for (auto i = 0; i != 5; ++i)
    {
        char *p = new char[6];
        strcpy(p, "Hello");
        oldstyle.push_back(p);
    }

    names.assign(oldstyle.cbegin(), oldstyle.cend());
    for (auto s : names)
        cout << s << endl;

    cout << "Recycle resources:" << endl;
    for (auto p : oldstyle)
        delete [] p;

    for (auto s : names)
        cout << s << endl;

    return 0;
}
