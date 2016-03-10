#include <iostream>
#include <string>
using namespace std;

int main()
{
	string s("Hello, world!");
	cout << s << endl;

	//for (decltype(s.size()) i=0; i!=s.size(); ++i)
	//	s[i] = 'X';
	for (char i=0; i!=s.size(); ++i)
		s[i] = 'X';
	cout << s << endl;

	return 0;
}
