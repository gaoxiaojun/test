#include <iostream>
#include <stack>
using namespace std;

int main()
{
    stack<int> stack_int;

    for (size_t i = 0; i != 10; ++i)
        stack_int.push(i);

    while (!stack_int.empty())
    {
        int value = stack_int.top();
        cout << value << endl;
        stack_int.pop();
    }

    return 0;
}
