#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

bool has_asc_or_dec(const string &s)
{
    string asc("bdfhiklt");
    string dec("gpqy");
    string asc_and_dec = asc + dec;

    auto pos = s.find_first_of(asc_and_dec);
    return pos == string::npos ? false : true;
}

int main()
{
    string longest;

    ifstream ifile("dec_or_asc.cc");
    if (!ifile)
    {
        return -1;
    }

    string line;
    while (getline(ifile, line))
    {
        istringstream stream(line);
        string word;
        while (stream >> word)
        {
            if (has_asc_or_dec(word))
            {
                if (word.size() > longest.size())
                {
                    longest = word;
                }
            }
        }
    }

    cout << longest << endl;

    ifile.close();

    return 0;
}
