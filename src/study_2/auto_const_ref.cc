#include <iostream>
using namespace std;

int main()
{
	/*
	int i = 0, &r = i;
	auto a = r;

	const int ci = i, &cr = ci;
	auto b = ci;
	auto c = cr;
	auto d = &i;
	auto e = &ci;
	const auto f = ci;

	auto &g = ci;
//	auto &h = 42;
	const auto &j = 42;

	//a = 42; b = 42; c = 42;
	//d = 42; e = 42; g = 42;*/
	
	/*
	const int i = 42;
	auto j = i;
	const auto &k = i;
	auto *p = &i;
	const auto j2 = i, &k2 = i;
	*/

	int a = 3, b = 4;
	cout << "a: " << a << " b: " << b << endl;

	decltype(a) c = a;
	cout << "c: " << c << endl;

	decltype(a = b) d = a;
	cout << "d: " << d << endl;

	++d;
	cout << "a: " << a << endl;
	cout << "d: " << d << endl;

	return 0;
}
