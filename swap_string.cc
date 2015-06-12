#include <iostream>
#include <string>
using namespace std;

int main()
{
    string s1 = "Hello, world";
    string s2 = "my name is jianzhangyu";

    auto it1 = s1.cbegin();
    auto it1_end = s1.cend();
    auto it2 = s2.cbegin();
    auto it2_end = s2.cend();

    cout << "Before:" << endl;
    cout << s1 << endl;
    cout << s2 << endl;

    cout << "Use saved iterator:" << endl;
    while (it1 != it1_end)
        cout << *it1++;
    cout << endl;
    while (it2 != it2_end)
        cout << *it2++;
    cout << endl;

    swap(s1, s2);

    cout << "After:" << endl;
    cout << s1 << endl;
    cout << s2 << endl;

    return 0;
}
