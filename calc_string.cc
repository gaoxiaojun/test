#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main()
{
    vector<string> svec = {"1", "2", "3", "4", "5"};

    int sum = 0;
    for (string s : svec)
        sum += stoi(s);

    cout << sum << endl;

    vector<string> sfvec = {"1.0", "2.0", "3.0", "4.0", "5.1"};
    
    double dsum = 0.0;
    for (string s : sfvec)
        dsum += stod(s);

    cout << dsum << endl;

    return 0;
}
