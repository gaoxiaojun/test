#include <iostream>
#include <forward_list>
#include <string>
using namespace std;

void Insert(forward_list<string> &f, const string &s1,
        const string &s2)
{
    auto prev = f.before_begin();
    auto curr = f.begin();

    while (curr != f.end())
    {
        if (*curr == s1)
        {
            f.insert_after(curr, s2);
            break;
        }
        prev = curr;
        ++curr;
    }

    if (curr == f.end())
    {
        f.insert_after(prev, s2);
    }
}

int main()
{
    forward_list<string> f = {"hello", "world"};
    Insert(f, "hello", "my");
    Insert(f, "hi", "yeah!");

    for (auto it = f.begin(); it != f.end(); ++it)
        cout << *it << " ";
    cout << endl;

    return 0;
}
