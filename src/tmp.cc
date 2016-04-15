#include <iostream>
#include <string>
using namespace std;

string s = "abcdef";

void printR(string s, string &result)
{
    if (s.empty())
    {
        cout << result << endl;
        result = result.substr(0, result.length() - 1);
        return;
    }

    for (int i = 0; i < s.length(); ++i)
    {
        result += s[i];
        string tmp = s.substr(0, i) + s.substr(i + 1);
        printR(tmp, result);
    }
}

int main()
{
    string result;
    printR(s, result);
    return 0;
}
