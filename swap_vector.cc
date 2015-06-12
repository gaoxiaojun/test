#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main()
{
    vector<string> svec1 = {"Hello", "world"};
    vector<string> svec2 = {"my", "name", "is", "jianzhangyu"};

    cout << "Before:" << endl;
    for (const auto &s : svec1)
        cout << s << " ";
    cout << endl;
    
    for (const auto &s : svec2)
        cout << s << " ";
    cout << endl;

    auto it1 = svec1.cbegin();
    auto it1_end = svec1.cend();
    auto it2 = svec2.cbegin();
    auto it2_end = svec2.cend();

    //svec1.swap(svec2);
    swap(svec1, svec2);

    cout << "Use saved iterator:" << endl;
    while (it1 != it1_end)
        cout << *it1++ << " ";
    cout << endl;

    while (it2 != it2_end)
        cout << *it2++ << " ";
    cout << endl;

    cout << "After:" << endl;
    for (const auto &s : svec1)
        cout << s << " ";
    cout << endl;

    for (const auto &s : svec2)
        cout << s << " ";
    cout << endl;

    return 0;
}
