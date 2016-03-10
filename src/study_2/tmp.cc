#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <cctype>
using namespace std;

int main()
{
	/*
	string s;
	while (cin >> s)
	{
		string t;
		for (decltype(s.size()) i=0; i!=s.size(); ++i)
			if (!ispunct(s[i]))
				t += s[i];
		cout << t << endl;
	}
	*/

	/*
	const string s = "Keep out!";
	for (auto &c : s)
	{
		cout << c;
	}
	cout << endl;
	*/

	/*
	vector<string> svec = {"Hello", "world", "my", "name", "is", "xiao"};
	for (auto s : svec)
		cout << s << " ";
	cout << endl;

	svec.push_back("yu");
	for (auto s : svec)
		cout << s << " ";
	cout << endl;
	*/

	/*
	vector<int> ivec{1, 2, 3, 4, 5};
	for (auto &i : ivec)
		i *= 2;
	for (auto i : ivec)
		cout << i << " ";
	cout << endl;
	*/
	/*
	int a[10]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	for (auto i : a)
		cout << i << " ";
	cout << endl;
	*/
	int a[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	int *b = begin(a);
	int *e = end(a);

	while (b != e)
	{
		cout << *b << " ";
		++b;
	}
	cout << endl;

	return 0;
}
