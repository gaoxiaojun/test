#include <iostream>
#include <string>
#include <stack>
using namespace std;

int main()
{
    stack<char> st;
    string s;

    while (1)
    {
        cout << "Enter parenthesis:" << endl;
        cin >> s;

        for (char c : s)
        {
            if (c == '(' || c == '{' || c == '[')
                st.push(c);
            else if (c == ')' || c == '}' || c == ']')
            {
                char t = st.top();
                if (c == ')' && t == '(')
                {
                    cout << "pop '(' " << endl;
                    st.pop();
                }
                else if (c == '}' && t == '{')
                {
                    cout << "pop '{' " << endl;
                    st.pop();
                }
                else if (c == ']' && t == '[')
                {
                    cout << "pop '['" << endl;
                    st.pop();
                }
                else
                {
                    cout << "Not match" << endl;
                    break;
                }
            }
        }
    }

    return 0;
}
