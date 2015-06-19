#include <iostream>
#include <string>
using namespace std;

void Replace(string &s, const string &oldVal, 
        const string &newVal)
{
    auto old_size = oldVal.size();
    auto new_size = newVal.size();

    decltype(old_size) i = 0;
    while (i != s.size())
    {
        string sub_str = s.substr(i, old_size);
        if (sub_str == oldVal)
        {
            s.erase(i, old_size);
            s.insert(i, newVal);
            i += new_size;
            continue;
        }
        ++i;
    }
}

int main()
{
    string s("I thought you are a good man though.");
    cout << s << endl;

    Replace(s, "tho", "though");
    cout << s << endl;

    return 0;
}
