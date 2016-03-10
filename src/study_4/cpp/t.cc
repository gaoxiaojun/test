#include <iostream>
#include <string>
using namespace std;

int main()
{
    string array[] = {"Hello", "world", "my name is jianzhangyu", 
                      "nice to meet you"};
    cout << sizeof(array) / sizeof(array[0]) << endl;
    return 0;
}
