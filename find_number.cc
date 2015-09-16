#include <iostream>
#include <string>
using namespace std;

void find_first_of(const string &s, const string &numbers, 
        const string &letters)
{
    cout << "find_first_of:" << endl;
    string::size_type pos = 0;
    while ((pos = s.find_first_of(numbers, pos)) != string::npos)
    {
        cout << pos << " " << s[pos] << endl;
        ++pos;
    }

    pos = 0;
    while ((pos = s.find_first_of(letters, pos)) != string::npos)
    {
        cout << pos << " " << s[pos] << endl;
        ++pos;
    }
}

void find_first_not_of(const string &s, const string &numbers, 
        const string &letters)
{
    cout << "find_first_not_of:" << endl;
    string::size_type pos = 0;
    while ((pos = s.find_first_not_of(numbers, pos)) != string::npos)
    {
        cout << pos << " " << s[pos] << endl;
        ++pos;
    }

    pos = 0;
    while ((pos = s.find_first_not_of(letters, pos)) != string::npos)
    {
        cout << pos << " " << s[pos] << endl;
        ++pos;
    }
}

int main()
{
    string s("ab2c3d7R4E6");
    string numbers("0123456789");
    string letters("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");

    find_first_of(s, numbers, letters);
    find_first_not_of(s, numbers, letters);

    return 0;
}
