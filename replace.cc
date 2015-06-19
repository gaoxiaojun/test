#include <iostream>
#include <string>
using namespace std;

void replace_by_index(string &s, const string &oldVal, 
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

void replace_by_iterator(string &s, const string &oldVal,
        const string &newVal)
{
    auto old_size = oldVal.size();
    auto new_size = newVal.size();

    auto it = s.begin();
    while (it != s.end())
    {
        string sub_str(it, it + old_size);
        if (sub_str == oldVal)
        {
            it = s.erase(it, it + old_size);
            it = s.insert(it, newVal.cbegin(), newVal.cend());
            continue;
        }
        ++it;
    }
}

int main()
{
    string s("I thought you are a good man though.");
    cout << s << endl;

    replace_by_iterator(s, "tho", "though");
    cout << s << endl;

    return 0;
}
