#include <iostream>
using namespace std;

int main()
{
	long ld = 3.1415926;
	//int a{ld}, b = {ld};
	int c(ld), d = (ld);

	cout << "That's all" << endl;

	//int *p = nullptr;
	//int zero = 0;
	//int *p = zero;
	double *p;
	int i = 0;
	p = &i;

	return 0;
}
