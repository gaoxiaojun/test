#include <iostream>
#include <vector>
using namespace std;

int main()
{
    vector<bool> bv;
    bv.push_back(false);
    bv.push_back(true);

    cout << bv.size() << endl;
    cout << bv.capacity() << endl;

    cout << sizeof(bv) << endl;
    cout << sizeof(bv[0]) << endl;

    return 0;
}
